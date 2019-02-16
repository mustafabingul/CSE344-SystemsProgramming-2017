#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <errno.h>

#define PI2 6.2832
FILE* consumeLog;
int child=0;
double *data;

void childHandler(int sig){}
void CTRLCHandle(int signo){

    if(child){
        fclose(consumeLog);
        free(data);
    }
    else{
        wait(NULL);
    }
    exit(1);
}
int findLineCount(const char* filename){
    FILE* file=fopen(filename,"r+");
    if(file==NULL){
        perror("LINECOUNT WWARNINGG..");
    }

    char ch;
    int lines=0;

    while(!feof(file))
    {
        ch = fgetc(file);
        if(ch == '\n')
        {
            lines++;
        }
    }
    fclose(file);
    return lines;
}
//https://stackoverflow.com/questions/33058848/generate-a-random-double-between-1-and-1
double getRealNumber(){
    return (double)rand()/(double)RAND_MAX+(rand()%10);
}
//https://batchloaf.wordpress.com/2013/12/07/simple-dft-in-c/
void dft(double *_data, int _n, int _lastline) {

    //DFT calculation...
    double *Xre = (double*)calloc(_n,sizeof(double));
    double *Xim = (double*)calloc(_n,sizeof(double));
    int k,n,i;
    for (k=0 ; k<_n ; ++k)
    {
        for (n=0 ; n<_n; ++n) Xre[k] += _data[n] * cos(n * k * PI2 / _n);
        for (n=0 ; n<_n; ++n) Xim[k] -= _data[n] * sin(n * k * PI2 / _n);
    }

    printf("Process B: the dft of line %d (",_lastline);      //...............CURRENT
    for (i=0; i<_n; ++i)
    {
        printf(" %.2lf",_data[i]);
    }
    printf(") is: ");
    for (i=0; i<_n; ++i)
    {
        printf(" (%.2lf)+(%.2lf)i ",Xre[i],Xim[i]);
    }

    fprintf(consumeLog,"Process B: the dft of line %d (",_lastline);
    for (i=0; i<_n; ++i)
    {
        fprintf(consumeLog," %.2lf ",data[i]);
    }
    fprintf(consumeLog,") is: ");
    for (i=0; i<_n; ++i)
    {
        fprintf(consumeLog," (%.2lf)+(%.2lf)i ",Xre[i],Xim[i]);
    }
    printf("\n");
    fprintf(consumeLog,"\n");
    free(Xre);
    free(Xim);

}
void produceLine(const char *filename, int _n, int _m, pid_t _child) {

    unlink("producerLOG.txt");
    int producedLine=1;
    double _number;
    sigset_t sigset;
    sigset_t sigsetOld;
    sigaddset(&sigset,SIGINT);

    struct sigaction parentAction;
    memset(&parentAction,0, sizeof(parentAction));
    parentAction.sa_handler=childHandler;
    if(sigaction(SIGUSR2,&parentAction,NULL)==-1)
        perror("Failed to get old handler for SIGUSR2");

    struct flock lock;
    memset(&lock,0, sizeof(lock));

    struct sigaction produceActionC;
    memset(&produceActionC,0, sizeof(produceActionC));
    produceActionC.sa_handler=CTRLCHandle;
    if(sigaction(SIGINT,&produceActionC,NULL) == -1)
        perror("Failed to get old handler for SIGINT");

    while(1){
        sigprocmask(SIG_BLOCK,&sigset,&sigsetOld);
        if(findLineCount(filename) == _m){
            sigsuspend(&parentAction.sa_mask);

        }

        FILE* fp=fopen(filename,"a+");
        if(fp==NULL){
            perror("Cannot Open file.M");
            exit(1);
        }
        FILE* produceLog = fopen("producerLOG.txt","a+");
        if(produceLog==NULL){
            perror("Cannot Open PRODUCE LOG.");
            exit(1);
        }

        //LOCKING..
        lock.l_type = F_WRLCK;
        fcntl(fileno(fp),F_SETLKW,&lock);

        producedLine=findLineCount(filename);
        producedLine++;
        //LOG FILE and FILE WRITING..
        printf("Process A: I am producing a random sequence for line %d:  ",producedLine);
        fprintf(produceLog,"Process A: I am producing a random sequence for line %d:  ",producedLine);
        for (int i = 0; i < _n; ++i) {
            _number = getRealNumber();
            printf("%.2lf  ",_number);
            fprintf(fp,"%.2lf ",_number);
            fprintf(produceLog,"%.2lf ",_number);
        }
        fprintf(fp,"\n");
        fprintf(produceLog,"\n");
        printf("\n");

        //LOCK RELEASE..
        lock.l_type = F_UNLCK;
        fcntl(fileno(fp),F_SETLKW,&lock);

        //currentLine counter..
        kill(_child,SIGUSR1);

        fclose(fp);
        fclose(produceLog);//
        sigprocmask(SIG_UNBLOCK,&sigsetOld,NULL);
    }

}
int oneLineByte(const char* filename){
    FILE* file = fopen(filename,"r");

    if(file==NULL){
        perror("ONE_LINE WWARNINGG..");
    }

    char ch;
    int lineByte=0;
    while(!feof(file))
    {
        ch = fgetc(file);
        if(ch == '\n')
        {
            lineByte++;
            return lineByte;
        }
        lineByte++;
    }
    fclose(file);
    return -1;
}

//https://stackoverflow.com/questions/40899979/deleting-last-line-from-file-in-c-creates-empty-line
void deleteline2(const char *filename,unsigned int linetodelete){
    FILE *file=fopen(filename,"r");
    if(file==NULL){
        perror("ACILAMADı");
        return;
    }
    FILE *filetemp=fopen("__tempfile__","w");
    if(filetemp==NULL){
        perror("ACILAMADI.K");
        return;
    }

    char line[255];
    unsigned int countline=0;

    while(fgets(line,255,file)!=NULL){
        if(countline!=linetodelete)
            fputs(line,filetemp);
        countline++;
    }
    fclose(file);
    fclose(filetemp);

    rename("__tempfile__",filename);/*
    FILE* file=fopen(filename,"r+");
    int last;
    fseek(file,0,SEEK_END);
    last=ftell(file);
    int byte=oneLineByte(filename);
    int b=last-byte;

    truncate(filename,b);

    fclose(file);*/
}
void foo(double *data,const char* filename,int n){

    int value=oneLineByte(filename);
    int lastline=findLineCount(filename);

    FILE* file=fopen(filename,"r");

    fseek(file,0,SEEK_END);
    fseek(file,-value,SEEK_CUR);
    int lPOS=ftell(file);
    for (int i = 0; i < n; ++i) {
        fscanf(file,"%lf",&data[i]);
    }
    lastline++;
    dft(data, n, lastline);
    //truncate("filE.txt",lPOS);
    fclose(file);
    deleteline2(filename,lastline);


}

void consumeLine(const char *filename, int _n, int _m) {


    child=1;

    data=(double *)calloc(_n, sizeof(double));
    int i=0;
    sigset_t sigsetC;
    sigset_t sigsetOldC;
    sigaddset(&sigsetC,SIGINT);

    struct sigaction consumeActionC;
    memset(&consumeActionC,0, sizeof(consumeActionC));
    consumeActionC.sa_handler=CTRLCHandle;
    if(sigaction(SIGINT,&consumeActionC,NULL) == -1)
        perror("Failed to get old handler for SIGINT");


    struct sigaction childAction;
    memset(&childAction,0, sizeof(childAction));
    childAction.sa_handler=childHandler;
    if(sigaction(SIGUSR1,&childAction,NULL)==-1)
        perror("Failed to get old handler for SIGUSR1");

    struct flock lock;
    memset(&lock,0, sizeof(lock));


    while(1){
        sigprocmask(SIG_BLOCK,&sigsetC,&sigsetOldC);
        if(findLineCount(filename)==0){
            sigsuspend(&(childAction.sa_mask));
        }

        FILE* _fp=fopen(filename,"r+");
        if(_fp==NULL){
            perror("Cannot Open fileC");
            exit(1);
        }

        lock.l_type = F_WRLCK;//OR RDLOCK
        fcntl(fileno(_fp),F_SETLKW,&lock);

        foo(data,filename,_n);

        lock.l_type = F_UNLCK;
        fcntl(fileno(_fp),F_SETLKW,&lock);

        kill(getppid(),SIGUSR2);

        fclose(_fp);

        sigprocmask(SIG_UNBLOCK,&sigsetOldC,NULL);
    }

}
int main(int argc,char* argv[]) {

    int N=5;
    int M=20;
    char *FILENAME;
    if(argc!=7){
        fprintf(stderr,"Usage :: ./multiprocess_DFT -N _ -X ___.txt -M _ ");
        exit(0);
    }

    if(strcmp(argv[1],"-N")){
        N=atoi(argv[2]);
    }
    if(strcmp(argv[5],"-M")){
        M=atoi(argv[6]);
    }

    unlink(argv[4]);
    unlink("consumerLOG.txt");
    pid_t child;

    child=fork();


    if(child==0){

        consumeLog= fopen("consumerLOG.txt","a+");
        if(consumeLog==NULL){
            perror("Cannot Open CONSUME LOG.");
            exit(1);
        }

        consumeLine(argv[4], N, M);

    }
    else{

        FILE* file_ptr = fopen(argv[4], "w+");
        fclose(file_ptr);

        produceLine(argv[4], N, M, child);

        wait(NULL);
    }
    return 0;
}