#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include "myQueue.h"
#include <unistd.h>
#define orchid "orchid"
#define rose "rose"
#define violet "violet"
#define clove "clove"
#define daffodil "daffodil"
#define NUM_THREADS 3
/*typedef struct location{
    int x;
    int y;
}loc;*/
typedef struct client{
    char Cname[15];
    struct location Clocation;
    char Cflower[15];
}clnt;
typedef struct florist{
    int floristID;
    char Fname[15];
    struct location Flocation;
    double Fspeed;
    char Fflower[3][15];
}flor;
void *workers(void * arguman);
void mainThe(char *filename);
void cleanFlowerName();
double EuclideanDistance(double x1, double y1, double x2, double y2);
int WhichWillQinsert(int x,int y,char *Cflower,struct florist flor[3]);
void statistic();
Queue *mainQ;
Queue *q0;
Queue *q1;
Queue *q2;

double time0=0;
double time1=0;
double time2=0;
int number0=0;
int number1=0;
int number2=0;
pthread_mutex_t m0,m1,m2;
pthread_cond_t c0,c1,c2;

pthread_t workerThreads[NUM_THREADS];
struct florist florists[NUM_THREADS];

int main(int argc, char *argv[]) {

    if(argc!=2){
        printf("Usage:: ./florist [filename] ::\n");
        return 0;
    }
    mainThe(argv[1]);

    return 0;
}

void mainThe(char *filename){

    pthread_mutex_init(&m0,0);
    pthread_mutex_init(&m1,0);
    pthread_mutex_init(&m2,0);
    pthread_cond_init(&c0,0);
    pthread_cond_init(&c1,0);
    pthread_cond_init(&c2,0);

    NODE *empty;
    empty = (NODE*) malloc(sizeof (NODE));
    strcpy(empty->data.Cname,"NULL");

    mainQ= concreateQue(0);
    q0= concreateQue(0);
    q1= concreateQue(0);
    q2= concreateQue(0);

    NODE *node;
    char s1[10],s2[10];
    int m,n;

    char foo[10];
    FILE* f= fopen(filename,"r");
    if(f==NULL){
        printf("Cannot Open File...!!! There is not such a file...\n");
        exit(0);
    }
    printf("Florist application initializing from file:: %s\n",filename);
    printf("3 florists have been created\n");
    for (int k = 0; k <3; ++k) {
        fscanf(f,"%s (%d,%d; %lf) %s %s %s %s\n",
        florists[k].Fname,&florists[k].Flocation.x,&florists[k].Flocation.y,&florists[k].Fspeed,foo,florists[k].Fflower[0],florists[k].Fflower[1],florists[k].Fflower[2]);
    }
    cleanFlowerName();
    /*for (int i = 0; i < 3; ++i) {
        //printf("%s %d - %d  -  %lf  -  %s  %s  %s\n",florists[i].Fname,florists[i].Flocation.x,florists[i].Flocation.y,florists[i].Fspeed,florists[i].Fflower[0],florists[i].Fflower[1],florists[i].Fflower[2]);
    }*/
    printf("Processing requests...\n");
    while(!feof(f)){
        fscanf(f,"%s (%d,%d): %s\n",s1,&m,&n,s2);
        node=(NODE *)malloc(sizeof(NODE));
        node->data.Clocation.x=m;
        node->data.Clocation.y=n;
        strcpy(node->data.Cname,s1);
        strcpy(node->data.Cflower,s2);
        enq(mainQ, node);
    }

    NODE *var;
    int ind=0;
    while(!emptyQ(mainQ)){

        var= deq(mainQ);
        ind = WhichWillQinsert(var->data.Clocation.x,var->data.Clocation.y,var->data.Cflower,florists);
        if(ind==0){
            //printf("1--) IND %d\n",ind);
            enq(q0, var);
            pthread_cond_signal(&c0);
        }
        if(ind==1){
            enq(q1, var);
            pthread_cond_signal(&c1);
        }
        if(ind==2){
            enq(q2, var);
            pthread_cond_signal(&c2);
        }
        else if(ind!=0 && ind!=1 && ind!=2)
            printf("0-1-2 DEĞİLL\n");
    }

    enq(q0, empty);
    enq(q1, empty);
    enq(q2, empty);
    /*printf("/////////    %d\n",q0->size);
    printf("/////////    %d\n",q1->size);
    printf("/////////    %d\n",q2->size);
    */
    for (int i = 0; i < 3; ++i){
        florists[i].floristID=i;
        pthread_create(&workerThreads[i],NULL,workers,&florists[i]);
    }



    for (int j = 0; j < 3; ++j) {
        pthread_join(workerThreads[j], NULL);
    }
    printf("All request processed.\n");
    for (int l = 0; l < 3; ++l) {
        printf("%s closing shop.\n", florists[l].Fname);
    }
    statistic();
    pthread_cond_destroy(&c0);
    pthread_cond_destroy(&c1);
    pthread_cond_destroy(&c2);
    pthread_mutex_destroy(&m0);
    pthread_mutex_destroy(&m1);
    pthread_mutex_destroy(&m2);

    fclose(f);
    free(empty);
    free(node);
    destroyQue(mainQ);
    destroyQue(q0);
    destroyQue(q1);
    destroyQue(q2);

    pthread_exit(NULL);

}
void *workers(void * arguman){
    int y=0;
    struct florist *f = (struct florist *)arguman;

    if(f->floristID==0){
        double d0,t0;
        NODE *nod0;
        while(1){

            nod0= deq(q0);
            if(strcmp(nod0->data.Cname,"NULL")==0){
                //fprintf(stdout,"GÜLEGÜLE thereaddd__1\n");
                return 0;
            }
            pthread_mutex_lock(&m0);
            while(emptyQ(q0))pthread_cond_wait(&c0,&m0);

            //TO  DOO... CALCULATİON...
            d0=EuclideanDistance(f->Flocation.x,f->Flocation.y,nod0->data.Clocation.x,nod0->data.Clocation.y);
            t0=d0/f->Fspeed;
            usleep(t0);
            printf("Florist %s has delivered a %s to %s in %.2lf ms\n",
                   f->Fname,nod0->data.Cflower,nod0->data.Cname,d0);
            number0++;
            time0+=d0;
            pthread_mutex_unlock(&m0);
        }

    }
    if(f->floristID==1){
        double d1,t1;
        NODE *nod1;
        while(1){

            nod1= deq(q1);
            if(strcmp(nod1->data.Cname,"NULL")==0){
                //fprintf(stdout,"GÜLEGÜLE thereaddd__2\n");
                return 0;
            }
            pthread_mutex_lock(&m1);
            while(emptyQ(q1))pthread_cond_wait(&c1,&m1);

            //TO  DOO... CALCULATİON...
            d1=EuclideanDistance(f->Flocation.x,f->Flocation.y,nod1->data.Clocation.x,nod1->data.Clocation.y);
            t1=d1/f->Fspeed;
            usleep(t1);
            printf("Florist %s has delivered a %s to %s in %.2lf ms\n",
                   f->Fname,nod1->data.Cflower,nod1->data.Cname,d1);
            number1++;
            time1+=d1;
            pthread_mutex_unlock(&m1);
        }
    }
    if(f->floristID==2){
        double d2,t2;
        NODE *nod2;
        while(1){

            nod2= deq(q2);
            if(strcmp(nod2->data.Cname,"NULL")==0){
                //fprintf(stdout,"GÜLEGÜLE thereaddd__3\n");
                return 0;
            }
            pthread_mutex_lock(&m2);
            while(emptyQ(q2))pthread_cond_wait(&c2,&m2);

            //TO  DOO... CALCULATİON...
            d2=EuclideanDistance(f->Flocation.x,f->Flocation.y,nod2->data.Clocation.x,nod2->data.Clocation.y);
            t2=d2/f->Fspeed;
            usleep(t2);
            printf("Florist %s has delivered a %s to %s in %.2lf ms\n",
                   f->Fname,nod2->data.Cflower,nod2->data.Cname,d2);
            number2++;
            time2+=d2;
            pthread_mutex_unlock(&m2);
        }
    }
    else
        printf("WORKERSS __ 333333333333\n");
}

double EuclideanDistance(double x1, double y1, double x2, double y2)
{
    double x = x1 - x2;
    double y = y1 - y2;
    double EucDist;
    EucDist = pow(x, 2) + pow(y, 2);
    EucDist = sqrt(EucDist);
    return EucDist;
}
int WhichWillQinsert(int x,int y,char *Cflower,struct florist flor[3]){
    int index=0;
    int U = 9999999;
    double newM,old,m;
    for (int i = 0; i < 3; ++i) {
        newM = EuclideanDistance(x,y,flor[i].Flocation.x,flor[i].Flocation.y);
        if((newM<U) && (strcmp(flor[i].Fflower[0],Cflower)==0 || strcmp(flor[i].Fflower[1],Cflower)==0 || strcmp(flor[i].Fflower[2],Cflower)==0)){
            U=newM;
            index=i;
        }
    }
    //printf("INDEX > %d \n",index);
    return index;
}
void statistic(){
    printf("Sale statistics for today:\n");
    printf("----------------------------------------------------------\n");
    printf("Florist                 # of sales         Total Time     \n");
    printf("%s                      %d                %.2lf ms\n",florists[0].Fname,number0,time0);
    printf("%s                      %d                %.2lf ms\n",florists[1].Fname,number1,time1);
    printf("%s                      %d                %.2lf ms\n",florists[2].Fname,number2,time2);
    printf("----------------------------------------------------------\n");
}
void cleanFlowerName(){
    florists[0].Fflower[0][strlen(florists[0].Fflower[0])-1]='\0';
    florists[0].Fflower[1][strlen(florists[0].Fflower[1])-1]='\0';
    florists[1].Fflower[0][strlen(florists[1].Fflower[0])-1]='\0';
    florists[1].Fflower[1][strlen(florists[1].Fflower[1])-1]='\0';
    florists[2].Fflower[0][strlen(florists[2].Fflower[0])-1]='\0';
    florists[2].Fflower[1][strlen(florists[2].Fflower[1])-1]='\0';
}