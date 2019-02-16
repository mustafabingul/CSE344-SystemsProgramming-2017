/*

SystemV semaphoru kullanılmıştır, Ödev 5 semaphore ile implement edilmiştir.

    Mustafa BİNGÜL 141044077

 */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>
#include<fcntl.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#define EGGS 1
#define FLOUR 2
#define BUTTER 3
#define SUGAR 4
#define PERMS (S_IRUSR | S_IWUSR)
struct cheff {
    char name[10];
    int ownIngredients[2];
    int noneIngredients[2];
}chef;

struct wholesaler{
    char name[10];
    int ingredients[2];
};
char* ingredients(int id);
int r_semop(int semid, struct sembuf *sops, int nsops);
int initelement(int semid, int semnum, int semvalue);
int removesem(int semid);
int get_sem_value(int semid,int semnum);
void setsembuf(struct sembuf *s, int num, int op, int flg);
void allCheffs();
void workChef();
void deliverIngredients();
void statusCheff(struct cheff c);
int *sharedPlece;
int sem;
int semEgg;
int semFlour;
int semButter;
int semSugar;
pid_t p;
struct cheff c1,c2,c3,c4,c5,c6;
struct wholesaler w;
struct sembuf semsignal;
struct sembuf semwait;
int shm_fd;
const char *shm_name = "common";
const int SIZE = 2;
pid_t killProccessArray[6];
void signal_handler(int signo) {
    fprintf(stdout, "\nAll processes killed.\n");
    for(int i = 0; i < 6; ++i)
        kill(killProccessArray[i],SIGTERM);
    shm_unlink(shm_name);
    removesem(sem);
    removesem(semEgg);
    removesem(semFlour);
    removesem(semButter);
    removesem(semSugar);
    exit(0);
}
int main() {

    struct sigaction act;
    act.sa_handler = signal_handler;
    act.sa_flags = 0;

    if ((sem = semget(IPC_PRIVATE, 1, PERMS)) == -1)
        perror("Warning SEMget");
    if ((semEgg = semget(IPC_PRIVATE, 1, PERMS)) == -1)
        perror("Warning SEMEGGget");
    if ((semFlour = semget(IPC_PRIVATE, 1, PERMS)) == -1)
        perror("Warning SEMFLOURget");
    if ((semButter = semget(IPC_PRIVATE, 1, PERMS)) == -1)
        perror("Warning SEMBUTTERget");
    if ((semSugar = semget(IPC_PRIVATE, 1, PERMS)) == -1)
        perror("Warning SEMSUGARget");

    setsembuf(&semwait, 0, -1, 0);
    setsembuf(&semsignal, 0, 1, 0);

    if (initelement(sem, 0, 1) == -1) {
        removesem(sem);
        perror("Warning SEM");
    }
    if (initelement(semButter, 0, 0) == -1) {
        removesem(semButter);
        perror("Warning SEMBUTTER");
    }
    if (initelement(semSugar, 0, 0) == -1) {
        removesem(semSugar);
        perror("Warning SEMSUGAR");
    }
    if (initelement(semFlour, 0, 0) == -1) {
        removesem(semFlour);
        perror("Warning SEMFLOUR");
    }if (initelement(semEgg, 0, 0) == -1) {
        removesem(semEgg);
        perror("Warning SEMEGG");
    }
    if ((sigemptyset(&act.sa_mask) == -1) ||
        (sigaction(SIGINT, &act, NULL) == -1)) {
        perror("Failed to set Signal handler.");
        exit(1);
    }
    for (int i = 0; i < 1; ++i) {
        p=fork();
        killProccessArray[0]=p;
        if(p==0){
            strcpy(c1.name,"chef1");
            c1.ownIngredients[0]=1;
            c1.ownIngredients[1]=2;
            c1.noneIngredients[0]=3;
            c1.noneIngredients[1]=4;
            break;
        }

        p=fork();
        killProccessArray[1]=p;
        if(p==0){
            strcpy(c2.name,"chef2");
            c2.ownIngredients[0]=1;
            c2.ownIngredients[1]=3;
            c2.noneIngredients[0]=2;
            c2.noneIngredients[1]=4;
            break;
        }
        p=fork();
        killProccessArray[2]=p;
        if(p==0){
            strcpy(c3.name,"chef3");
            c3.ownIngredients[0]=1;
            c3.ownIngredients[1]=4;
            c3.noneIngredients[0]=3;
            c3.noneIngredients[1]=2;
            break;
        }
        p=fork();
        killProccessArray[3]=p;
        if(p==0){
            strcpy(c4.name,"chef4");
            c4.ownIngredients[0]=2;
            c4.ownIngredients[1]=3;
            c4.noneIngredients[0]=1;
            c4.noneIngredients[1]=4;
            break;
        }
        p=fork();
        killProccessArray[4]=p;
        if(p==0){
            strcpy(c5.name,"chef5");
            c5.ownIngredients[0]=2;
            c5.ownIngredients[1]=4;
            c5.noneIngredients[0]=1;
            c5.noneIngredients[1]=3;
            break;
        }p=fork();
        killProccessArray[5]=p;
        if(p==0){
            strcpy(c6.name,"chef6");
            c6.ownIngredients[0]=3;
            c6.ownIngredients[1]=4;
            c6.noneIngredients[0]=1;
            c6.noneIngredients[1]=2;
            break;
        }

    }

    if(p==0){
        sigset_t newsigset;
        if ((sigemptyset(&newsigset) == -1) ||
            (sigaddset(&newsigset, SIGINT) == -1))
            perror("Failed to initialize the signal set");
        else if (sigprocmask(SIG_BLOCK, &newsigset, NULL) == -1)
            perror("Failed to block SIGINT");
        while(1){
            allCheffs();
            r_semop(sem, &semwait, 1);
            workChef();
            r_semop(sem, &semsignal, 1);
        }
    }

    if(p>0){

        while(1){
            r_semop(sem, &semwait, 1);
            deliverIngredients();
            r_semop(sem,&semsignal,1);
        }

    }

    return 0;
}
void deliverIngredients()
{
    void* pointer;
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd,SIZE);
    //pointer = mmap(0, sizeof(SIZE)*2, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    //sharedPlece = (int *)pointer;
    time_t t;
    srand((unsigned) time(&t));
    int a,b;
    strcpy(w.name,"wholesaler");
    while(!((a=rand()%4+1)!=(b=rand()%4+1)));
    //sharedPlece[0]=a;
    //sharedPlece[1]=b;
    w.ingredients[0]=a;
    w.ingredients[1]=b;
    if((w.ingredients[0]==EGGS && w.ingredients[1]==FLOUR) || (w.ingredients[1]==EGGS && w.ingredients[0]==FLOUR)){
        r_semop(semEgg,&semsignal,1);
        r_semop(semFlour,&semsignal,1);
        if (initelement(semEgg, 0, 1) == -1) {
            removesem(semEgg);
            perror("Warning SEM");
        }
        if (initelement(semFlour, 0, 1) == -1) {
            removesem(semFlour);
            perror("Warning SEM");
        }
    }
    if((w.ingredients[0]==EGGS && w.ingredients[1]==BUTTER) || (w.ingredients[1]==EGGS && w.ingredients[0]==BUTTER)){
        r_semop(semEgg,&semsignal,1);
        r_semop(semButter,&semsignal,1);
        if (initelement(semEgg, 0, 1) == -1) {
            removesem(semEgg);
            perror("Warning SEM");
        }
        if (initelement(semButter, 0, 1) == -1) {
            removesem(semButter);
            perror("Warning SEM");
        }
    }
    if((w.ingredients[0]==EGGS && w.ingredients[1]==SUGAR) || (w.ingredients[1]==EGGS && w.ingredients[0]==SUGAR)){
        r_semop(semEgg,&semsignal,1);
        r_semop(semSugar,&semsignal,1);
        if (initelement(semEgg, 0, 1) == -1) {
            removesem(semEgg);
            perror("Warning SEM");
        }
        if (initelement(semSugar, 0, 1) == -1) {
            removesem(semSugar);
            perror("Warning SEM");
        }
    }
    if((w.ingredients[0]==FLOUR && w.ingredients[1]==BUTTER) || (w.ingredients[1]==FLOUR && w.ingredients[0]==BUTTER)){
        r_semop(semFlour,&semsignal,1);
        r_semop(semButter,&semsignal,1);
        if (initelement(semFlour, 0, 1) == -1) {
            removesem(semFlour);
            perror("Warning SEM");
        }
        if (initelement(semButter, 0, 1) == -1) {
            removesem(semButter);
            perror("Warning SEM");
        }
    }
    if((w.ingredients[0]==FLOUR && w.ingredients[1]==SUGAR) || (w.ingredients[1]==FLOUR && w.ingredients[0]==SUGAR)){
        r_semop(semFlour,&semsignal,1);
        r_semop(semSugar,&semsignal,1);
        if (initelement(semFlour, 0, 1) == -1) {
            removesem(semFlour);
            perror("Warning SEM");
        }
        if (initelement(semSugar, 0, 1) == -1) {
            removesem(semSugar);
            perror("Warning SEM");
        }
    }
    if((w.ingredients[0]==BUTTER && w.ingredients[1]==SUGAR) || (w.ingredients[1]==BUTTER && w.ingredients[0]==SUGAR)){
        r_semop(semButter,&semsignal,1);
        r_semop(semSugar,&semsignal,1);
        if (initelement(semButter, 0, 1) == -1) {
            removesem(semButter);
            perror("Warning SEM");
        }
        if (initelement(semSugar, 0, 1) == -1) {
            removesem(semSugar);
            perror("Warning SEM");
        }
    }
    printf("%s delivers %s and %s\n",w.name,ingredients(w.ingredients[0]),ingredients(w.ingredients[1]));

}
void prepareCheff(struct cheff c){
    printf("%s has taken the %s\n",c.name,ingredients(c.noneIngredients[0]));
    printf("%s has taken the %s\n",c.name,ingredients(c.noneIngredients[1]));
    printf("%s is preparing the dessert\n",c.name);
    printf("%s has delivered the dessert to the wholesaler\n",c.name);
    if(strcmp(c.name,"chef1")==0){
        if (initelement(semButter, 0, 0) == -1) {
            removesem(semButter);
            perror("Warning SEM");
        }
        if (initelement(semSugar, 0, 0) == -1) {
            removesem(semSugar);
            perror("Warning SEM");
        }
    }
    if(strcmp(c.name,"chef2")==0){
        if (initelement(semFlour, 0, 0) == -1) {
            removesem(semFlour);
            perror("Warning SEM");
        }
        if (initelement(semSugar, 0, 0) == -1) {
            removesem(semSugar);
            perror("Warning SEM");
        }
    }
    if(strcmp(c.name,"chef3")==0){
        if (initelement(semFlour, 0, 0) == -1) {
            removesem(semFlour);
            perror("Warning SEM");
        }
        if (initelement(semButter, 0, 0) == -1) {
            removesem(semButter);
            perror("Warning SEM");
        }
    }
    if(strcmp(c.name,"chef4")==0){
        if (initelement(semEgg, 0, 0) == -1) {
            removesem(semEgg);
            perror("Warning SEM");
        }
        if (initelement(semSugar, 0, 0) == -1) {
            removesem(semSugar);
            perror("Warning SEM");
        }
    }
    if(strcmp(c.name,"chef5")==0){
        if (initelement(semEgg, 0, 0) == -1) {
            removesem(semEgg);
            perror("Warning SEM");
        }
        if (initelement(semButter, 0, 0) == -1) {
            removesem(semButter);
            perror("Warning SEM");
        }
    }
    if(strcmp(c.name,"chef6")==0){
        if (initelement(semEgg, 0, 0) == -1) {
            removesem(semEgg);
            perror("Warning SEM");
        }
        if (initelement(semFlour, 0, 0) == -1) {
            removesem(semFlour);
            perror("Warning SEM");
        }
    }

}
void workChef(){
    strcpy(w.name,"wholesaler");
    if(get_sem_value(semButter,0)==1 && get_sem_value(semSugar,0)==1){
        //....
        if(c1.noneIngredients[0]==BUTTER && c1.noneIngredients[1]==SUGAR){
            prepareCheff(c1);
            fprintf(stdout,"%s has obtained the dessert and left to sell it\n",w.name);
        }
    }
    if(get_sem_value(semFlour,0)==1 && get_sem_value(semSugar,0)==1){
        if(c2.noneIngredients[0]==FLOUR && c2.noneIngredients[1]==SUGAR){
            prepareCheff(c2);
            fprintf(stdout,"%s has obtained the dessert and left to sell it\n",w.name);
        }
    }
    if(get_sem_value(semButter,0)==1 && get_sem_value(semFlour,0)==1){
        if(c3.noneIngredients[0]==BUTTER && c3.noneIngredients[1]==FLOUR){
            prepareCheff(c3);
            fprintf(stdout,"%s has obtained the dessert and left to sell it\n",w.name);
        }
    }
    if(get_sem_value(semEgg,0)==1 && get_sem_value(semSugar,0)==1){
        if(c4.noneIngredients[0]==EGGS && c4.noneIngredients[1]==SUGAR){
            prepareCheff(c4);
            fprintf(stdout,"%s has obtained the dessert and left to sell it\n",w.name);
        }
    }
    if(get_sem_value(semEgg,0)==1 && get_sem_value(semButter,0)==1){
        if(c5.noneIngredients[0]==EGGS && c5.noneIngredients[1]==BUTTER){
            prepareCheff(c5);
            fprintf(stdout,"%s has obtained the dessert and left to sell it\n",w.name);
        }
    }
    if(get_sem_value(semEgg,0)==1 && get_sem_value(semFlour,0)==1){
        if(c6.noneIngredients[0]==EGGS && c6.noneIngredients[1]==FLOUR){
            prepareCheff(c6);
            fprintf(stdout,"%s has obtained the dessert and left to sell it\n",w.name);
        }
    }

}
void allCheffs(){
    statusCheff(c1);
    statusCheff(c2);
    statusCheff(c3);
    statusCheff(c4);
    statusCheff(c5);
    statusCheff(c6);
}
void statusCheff(struct cheff c){
    if(c.noneIngredients[0]==1 || c.noneIngredients[0]==2 || c.noneIngredients[0]==3 || c.noneIngredients[0]==4){
        if(c.noneIngredients[1]==1 || c.noneIngredients[1]==2 || c.noneIngredients[1]==3 || c.noneIngredients[1]==4)
            printf("%s is waiting for %s and %s\n",c.name,ingredients(c.noneIngredients[0]),ingredients(c.noneIngredients[1]));
    }

}
char* ingredients(int id){
    if(id==1)
        return "EGGS";
    else if(id==2)
        return "FLOUR";
    else if(id==3)
        return "BUTTER";
    else if(id==4)
        return "SUGAR";
    else
        return "NULL...O";
}
int get_sem_value(int semid,int semnum){
    return semctl(semid,semnum,GETVAL,0);
}
int r_semop(int semid, struct sembuf *sops, int nsops) {
    while (semop(semid, sops, nsops) == -1)
        if (errno != EINTR)
            return -1;
    return 0;
}
int initelement(int semid, int semnum, int semvalue) {
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.val = semvalue;
    return semctl(semid, semnum, SETVAL, arg);
}
void setsembuf(struct sembuf *s, int num, int op, int flg) {
    s->sem_num = (short)num;
    s->sem_op = (short)op;
    s->sem_flg = (short)flg;
    return;
}
int removesem(int semid) {
    return semctl(semid, 0, IPC_RMID);
}

