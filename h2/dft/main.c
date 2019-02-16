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
#include <sys/types.h>
#define PI2 6.2832
FILE* consumeLog;
FILE* filee;
int child=0;
double *data;
int F=0;
void childHandler(int sig){}
void parentHandler(int sig){}
void CTRLCHandle(int signo){


    if(child){
        fclose(consumeLog);
        fclose(filee);
        free(data);
    }
    else{
        wait(NULL);
    }
    exit(1);
}
int LineCount(){

    int pos=ftell(filee);
    fseek(filee,0,SEEK_SET);
    char ch;
    int lines=0;

    while(!feof(filee))
    {
        ch = fgetc(filee);
        if(ch == '\n')
        {
            lines++;
        }
    }
    fseek(filee,pos,SEEK_SET);
    return lines;
}
double getRealNumber(){
    return (double)rand()/(double)RAND_MAX+(rand()%10);
}
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
void produceLine(int _n, int _m, pid_t _child) {

    unlink("producerLOG.txt");
    int producedLine=1;
    double _number;
    sigset_t sigset;
    sigset_t sigsetOld;
    sigaddset(&sigset,SIGINT);

    struct sigaction parentAction;
    memset(&parentAction,0, sizeof(parentAction));
    parentAction.sa_handler=parentHandler;


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
        if(LineCount() == _m){
            sigsuspend(&parentAction.sa_mask);

        }

        FILE* produceLog = fopen("producerLOG.txt","a+");
        if(produceLog==NULL){
            perror("Cannot Open PRODUCE LOG.");
            exit(1);
        }

        //LOCKING..
        lock.l_type = F_WRLCK;
        fcntl(fileno(filee),F_SETLKW,&lock);

        producedLine= LineCount();
        producedLine++;
        //LOG FILE and FILE WRITING..
        printf("Process A: I am producing a random sequence for line %d:  ",producedLine);
        fprintf(produceLog,"Process A: I am producing a random sequence for line %d:  ",producedLine);
        for (int i = 0; i < _n; ++i) {
            _number = getRealNumber();
            printf("%.2lf  ",_number);
            fprintf(filee,"%.2lf ",_number);
            fprintf(produceLog,"%.2lf ",_number);
        }
        fprintf(filee,"\n");
        fprintf(produceLog,"\n");
        printf("\n");

        //LOCK RELEASE..
        lock.l_type = F_UNLCK;
        fcntl(fileno(filee),F_SETLKW,&lock);

        //currentLine counter..
        kill(_child,SIGUSR1);


        fclose(produceLog);//
        sigprocmask(SIG_UNBLOCK,&sigsetOld,NULL);
    }

}
int oneLineByte() {

    int pos=ftell(filee);
    fseek(filee,0,SEEK_SET);

    char ch;
    int lineByte=0;

    while(!feof(filee))
    {
        ch = fgetc(filee);
        if(ch == '\n')
        {
            lineByte++;
            break;
        }
        lineByte++;
    }
    fseek(filee,pos,SEEK_SET);
    return lineByte;
}
void foo(double *data, int n) {


    int value= oneLineByte();
    int lastline= LineCount();

    fseek(filee,0,SEEK_END);
    fseek(filee,-value,SEEK_CUR);
    int nPos=ftell(filee);

    for (int i = 0; i < n; ++i) {
        fscanf(filee,"%lf",&data[i]);
    }
    //lastline++;
    dft(data, n, lastline);

    fseek(filee,nPos,SEEK_SET);
    truncate("filE.txt",nPos);


}
void consumeLine(int _n, int _m) {


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
        if(LineCount()==0){
            sigsuspend(&(childAction.sa_mask));
        }

        lock.l_type = F_WRLCK;//OR RDLOCK
        fcntl(fileno(filee),F_SETLKW,&lock);

        foo(data, _n);

        lock.l_type = F_UNLCK;
        fcntl(fileno(filee),F_SETLKW,&lock);

        kill(getppid(),SIGUSR2);

        sigprocmask(SIG_UNBLOCK,&sigsetOldC,NULL);
    }
}
int main(int argc,char* argv[]) {

    unlink("consumerLOG.txt");

    int N=5;
    int M=20;

    pid_t child;

    filee=fopen("filE.txt","r+");
    if(filee==NULL){
        perror("ILK HATAAAAAAAA");
    }
    consumeLog= fopen("consumerLOG.txt","a+");
    if(consumeLog==NULL){
        perror("Cannot Open CONSUME LOG????.");
        exit(1);
    }
    child=fork();


    if(child==0){

        consumeLine(N, M);

    }
    else{


        produceLine(N, M, child);

        wait(NULL);
    }
    /*data=(double*)calloc(N, sizeof(double));
    foo(data,N);*/
    return 0;
}