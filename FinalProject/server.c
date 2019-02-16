/*  MUSTAFA BİNGÜL 141044077    */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include<unistd.h>    //write
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <signal.h>
#define PROVIDERLIMIT 1000
#define PI 3.1415
typedef struct type{

    char name[15];
    char type[2];
    int taylorDegree;
    int socket_fd;
    int port;
    struct type *p;
}NODE;

typedef struct queue{
    int size;
    NODE *head;
    int limit;
    NODE *tail;
} queue;

typedef struct provider{
    int id;
    char name[15];
    int performance;
    int price;
    int duration;
    queue *providerQ;
    int numberofService;
    pthread_mutex_t mutex;
    pthread_cond_t conditionV;
    FILE * fp;
}p;
int port;
pthread_cond_t common_condition;
void killClients();
void signalHandler(int signo);
int rightQueue(const char *type);
void findRightSort(int *feature,int *index, int point);
long getTimeDif(struct timeval start, struct timeval end);
double coss(double x);
double factorial(double x);
queue *CQueue(int startlimit);
void DQueue(queue *que);
int addQueue(NODE *val, queue *que);
NODE *PQueue(queue *que);
int isempty(queue *que);
void *providerWork(void *arg);
void providerInfo(FILE * file);
void providerSet(char *filename);
void *post_Handler(void *arg);
void statistic();
void writePidToList(int num);
FILE * logFile;
struct provider myProviders[PROVIDERLIMIT];
pthread_t myProThreads[PROVIDERLIMIT];
int providerCount=0;
int main(int argc, char *argv[]) {

    if (argc < 4){
        fprintf(stderr,"Usage: %s [port] [providerFile] [logFile]\n", argv[0]);
        return 0;
    }

    //TODO LOOG FILEEe........
    logFile = fopen(argv[3],"w+");
    if(logFile==NULL){
        fprintf(stderr,"Cannot open File");
    }
    fprintf(stderr,"Logs kept at %s\n",argv[3]);

    signal(SIGINT,signalHandler);

    providerSet(argv[2]);
    providerInfo(logFile);

    for(int i=0;i<providerCount;i++){
        myProviders[i].id=i;
        myProviders[i].providerQ=CQueue(0);
        pthread_mutex_init(&(myProviders[i].mutex),0);
        pthread_cond_init(&(myProviders[i].conditionV),0);
        myProviders[i].fp=logFile;
        if(pthread_create(&myProThreads[i],NULL,providerWork,&myProviders[i])<0){
            perror("could not create PROVİDER thread");
            return 1;
        }
        pthread_detach(myProThreads[i]);
    }

    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    port=atoi(argv[1]);
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Could not create socket");
    }
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons((unsigned short) port);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    //Listen
    listen(socket_desc , PROVIDERLIMIT);
    c = sizeof(struct sockaddr_in);

    fprintf(stderr,"Server is waiting for client connections at port %d\n",port);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if(pthread_create(&sniffer_thread, NULL, post_Handler, (void *) new_sock) < 0){
            perror("could not create YÖNthread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        //free(new_sock);
    }

    if (client_sock < 0){
        perror("accept failed");
        return 1;
    }
    for (int j = 0; j < providerCount; ++j) {
        pthread_join(myProThreads[j],NULL);
    }
    for (int k = 0; k < providerCount; ++k) {
        DQueue(myProviders[k].providerQ);
        pthread_mutex_destroy(&(myProviders[k].mutex));
        pthread_cond_destroy(&(myProviders[k].conditionV));
    }
    //close(socket_desc);
    fclose(logFile);
    //free(new_sock);
    return 0;
}
void statistic(){
    fprintf(stderr,"Statictics\n");
    fprintf(stderr,"Name        # of clients served\n");
    for (int i = 0; i < providerCount; ++i) {
        fprintf(stderr,"%s              %d\n",myProviders[i].name,myProviders[i].numberofService);
    }
    fprintf(stderr,"Goodbyee..\n");
}
void *post_Handler(void *arg){
    //Get the socket descriptor
    int sock = *(int*)arg;
    int read_size;
    char client_message[2000];
    char s0[15];
    char s1[15];
    char s2[15];
    char s3[15];
    char s4[15];
    NODE *node;
    int pidd;
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ){
        //Send the message back to client
        //write(sock , client_message , strlen(client_message));
        sscanf(client_message,"%s %s %s %s %s %d",s0,s1,s2,s3,s4,&pidd);
        node=(NODE * )malloc(sizeof(NODE));
        strcpy(node->name,s0);
        strcpy(node->type,s1);
        node->taylorDegree=atoi(s2);
        node->socket_fd=sock;

        writePidToList(pidd);

        //TODO QUEUEE ADD...
        int p = rightQueue(s1);
        if(p==-1){
            write(sock,"NO PROVIDER IS AVAILABLE", strlen("NO PROVIDER IS AVAILABLE"));
            return 0;
        }
        addQueue(node,myProviders[p].providerQ);
        fprintf(stderr, "Client %s (%s %s) connected, forwarded to provider %s\n",s0,s1,s2,myProviders[p].name);
        for (int i = 0; i < providerCount; ++i) {
            pthread_cond_signal(&(myProviders[i].conditionV));
        }
        //free(node);
    }

    if(read_size == 0){
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1){
        perror("recv failed");
    }

    //Free the socket pointer
    free(arg);

    return 0;
}
void writePidToList(int num){
    FILE *fp = fopen("pidLog","a+");
    fprintf(fp,"%d\n",num);
    fclose(fp);
}
void signalHandler(int signo){
    if(signo == SIGINT) {
        fprintf(stderr,"Termination signal received.\n");
        fprintf(stderr,"Terminating all clients\n");
        fprintf(stderr,"Terminating all providers\n");
        printf("SERVER SHUTDOWN...\n");
        statistic();
        close(port);
        fclose(logFile);
        killClients();
        for (int i = 0; i < providerCount; ++i) {
            pthread_cancel(myProThreads[i]);
        }
        remove("pidLog");
        exit(1);
    }
}
void killClients(){
    FILE *fp = fopen("pidLog","r");
    int num;
    while(!feof(fp)){
        fscanf(fp,"%d",&num);
        kill(num,SIGINT);
    }
    fclose(fp);
}
void *providerWork(void *arg){

    struct timeval start, end0,end1;
    double t0,t1;
    struct provider *provide = (struct provider*) arg;
    double calc=0;
    int randomNumber;
    NODE *nod;
    char ToClientResMsg[2000];
    unsigned int myTime = time(0) + provide->duration;
    //while(1)...
    //TODO DURATIONN.....
    while(time(0)<myTime){

        pthread_mutex_lock(&(provide->mutex));

        while(isempty(provide->providerQ)) {
            fprintf(stderr, "Provider %s is waiting a for task\n", provide->name);
            pthread_cond_wait(&(provide->conditionV), &(provide->mutex));
        }

        //TODO CALCULATİONNNNNNN...
        provide->numberofService += 1;
        nod=PQueue(provide->providerQ);
        gettimeofday(&start, NULL);
        fprintf(stderr,"Provider %s is processing task number %d: %d\n", provide->name,(provide->numberofService),nod->taylorDegree);
        //fprintf(provide->fp,"Provider %s is processing task number %d: %d\n", provide->name,(provide->numberofService),nod->taylorDegree);
        randomNumber=rand() % 10+5;
        sleep(randomNumber);
        calc=coss((nod->taylorDegree) * PI / 180);
        gettimeofday(&end0, NULL);
        t0 = getTimeDif(start,end0)/1000.0;
        fprintf(stderr,"Provider %s completed task number %d: cos(%d)=%.3lf in %.3lf seconds.\n", provide->name,(provide->numberofService),nod->taylorDegree,calc,t0);
        //fprintf(provide->fp,"Provider %s completed task number %d: cos(%d)=%.3lf in ??? seconds.\n", provide->name,(provide->numberofService),nod->taylorDegree,calc);
        gettimeofday(&end1, NULL);
        t1 = getTimeDif(start,end1)/1000.0;
        sprintf(ToClientResMsg,"%s's task completed by %s in %.3lf seconds, cos(%d)=%.3lf, cost is %d TL, total time spent %.3lf seconds.\n"
                ,nod->name,provide->name,t0,nod->taylorDegree,calc,provide->price,t1);
        write(nod->socket_fd,ToClientResMsg,strlen(ToClientResMsg));

        pthread_mutex_unlock(&(provide->mutex));
    }

    return 0;

}
int rightQueue(const char *type){
    int *properties;
    int *index;
    properties = (int*)malloc(sizeof(int)*providerCount);
    index = (int*)malloc(sizeof(int)*providerCount);

    if(strcmp(type,"Q")==0){
        for (int i = 0; i < providerCount; ++i) {
            index[i]=i;
            properties[i]=myProviders[i].performance;
        }
        findRightSort(properties,index,0);
    }
    if(strcmp(type,"C")==0){
        for (int i = 0; i < providerCount; ++i) {
            index[i]=i;
            properties[i]=myProviders[i].price;
        }
        findRightSort(properties,index,1);
    }
    if(strcmp(type,"T")==0){
        for (int i = 0; i < providerCount; ++i) {
            index[i]=i;
            properties[i]=myProviders[i].duration;
        }
        findRightSort(properties,index,1);
    }

    /*for (int j = 0; j <providerCount; ++j) {
        fprintf(stderr,"PROPERTYS: > %d VALUE: > %d\n",properties[j],index[j]);
    }*/
    int rightQ=-1;

    for (int k = 0; k < providerCount; ++k) {
        if(myProviders[index[k]].providerQ->size!=2){
            rightQ=index[k];
            break;
        }
    }
    free(index);
    free(properties);

    return rightQ;

}
void findRightSort(int *feature,int *index, int point){
    int x=0;
    int y=0;
    //1-küçükten büyüğe
    if(point==1){
        for (int i = 0; i < providerCount; ++i) {
            for (int j = i+1; j < providerCount; ++j) {
                if(feature[i]>feature[j]){
                    x=feature[i];
                    feature[i]=feature[j];
                    feature[j]=x;

                    y=index[i];
                    index[i]=index[j];
                    index[j]=y;
                }
            }
        }
    }
    else{
        for (int i = 0; i < providerCount; ++i) {
            for (int j = i+1; j < providerCount; ++j) {
                if(feature[i]<feature[j]){
                    x=feature[i];
                    feature[i]=feature[j];
                    feature[j]=x;

                    y=index[i];
                    index[i]=index[j];
                    index[j]=y;
                }
            }
        }
    }
}


void providerSet(char *filename){
    int i=0;
    char firstLine[1024];
    FILE * fileP = fopen(filename,"r");
    if(fileP==NULL)
    {
        perror("FİLE CANNOT OPEN..");
    }
    fgets(firstLine, sizeof(firstLine),fileP);
    while(!feof(fileP)){

        fscanf(fileP, "%s %d %d %d",myProviders[i].name,&myProviders[i].performance,&myProviders[i].price,&myProviders[i].duration);
        ++i;

    }
    providerCount = i;
    fclose(fileP);

}
void providerInfo(FILE * file){
    int a=0;
    printf("%d provider threads created\n",providerCount);
    printf("  NAME       PERFORMANCE PRICE DURATION\n");
    //fprintf(file,"%d provider threads created\n",providerCount);
    //fprintf(file,"  NAME       PERFORMANCE PRICE DURATION\n");
    while(a<providerCount){
        printf("%7s           %d       %d      %d \n",myProviders[a].name,myProviders[a].performance,myProviders[a].price,myProviders[a].duration);
        //fprintf(file,"%7s           %d       %d      %d \n",myProviders[a].name,myProviders[a].performance,myProviders[a].price,myProviders[a].duration);
        ++a;
    }
}
double factorial(double x) {
    if(x == 0) return 1;
    if(x == 1) return 1;

    return x * factorial(x-1);
}

double coss(double x) {
    int n = 20; // precision coefficient
    double res = 0.0;
    for(int i = 0 ; i<n ; i++) {
        res += pow(-1.0, i) * pow(x, 2.0 * i)/factorial(2.0*i);
    }
    return res;
}
queue *CQueue(int startlimit){
    queue *que=(queue *)malloc(sizeof(queue));
    if(que==NULL)return NULL;
    if(startlimit<=0)startlimit=1000;
    que->limit=startlimit;
    que->tail=NULL;
    que->head=NULL;
    que->size=0;

}
void DQueue(queue *que){
    NODE *nod;
    while(!isempty(que)){
        nod= PQueue(que);
        free(nod);
    }
    free(que);
}
int addQueue(NODE *val, queue *que){
    if(que==NULL || val==NULL)return 0;
    if(que->size>=que->limit)return 0;
    val->p=NULL;
    if(que->size==0){
        que->head=val;
        que->tail=val;
    }else{
        que->tail->p=val;
        que->tail=val;
    }
    que->size++;
    return 1;
}
NODE *PQueue(queue *que){
    NODE *temp;
    if(isempty(que))return NULL;
    temp=que->head;
    que->head=(que->head)->p;
    que->size--;
    return temp;
}
int isempty(queue *que){
    if(que==NULL)return 0;
    if(que->size==0)return 1;
    else return 0;
}
long getTimeDif(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000.0f + (end.tv_usec - start.tv_usec)/* / 1000.0f*/;
}