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

struct cheff {
    char name[10];
    int neededIngredients[2];
};

struct wholesaler{
    char name[10];
    int ingredients[2];
};
void cheffs();
void aa();
void bb();
void allChef();
void chefStat(struct cheff c);
void prepareChef(struct cheff c);
char* ingredients(int id);
#define EGGS 1
#define FLOUR 2
#define BUTTER 3
#define SUGAR 4
#define NOPE -1

pid_t p=-1;
pid_t w=-1;
struct cheff c1,c2,c3,c4,c5,c6;
struct wholesaler w1;
sem_t *s;
sem_t *egg;
sem_t *flour;
sem_t *butter;
sem_t *sugar;
const char *sharemem = "common";
int *sharedPlace;

int main() {

    int valEgg,valFlour,valButter,valSugar;
    const int SIZE = 2;
    int shm_fd;
    void* pointer;
    shm_fd = shm_open(sharemem, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    pointer = mmap(0, sizeof(SIZE)*2, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    sharedPlace = (int *)pointer;

    s=sem_open("sem1",O_CREAT|O_EXCL,0644,0);
    if(sem_init(s,1,1)==-1)
        perror("HATA SEM.");

    egg=sem_open("egg1",O_CREAT|O_EXCL,0644,0);
    if(sem_init(egg,1,0)==-1)
        perror("HATA SEM.");

    flour=sem_open("flour1",O_CREAT|O_EXCL,0644,0);
    if(sem_init(flour,1,0)==-1)
        perror("HATA SEM.");

    butter=sem_open("butter1",O_CREAT|O_EXCL,0644,0);
    if(sem_init(butter,1,0)==-1)
        perror("HATA SEM.");

    sugar=sem_open("sugar1",O_CREAT|O_EXCL,0644,0);
    if(sem_init(sugar,1,0)==-1)
        perror("HATA SEM.");
    sem_unlink("sem1");
    sem_unlink("egg1");
    sem_unlink("flour1");
    sem_unlink("butter1");
    sem_unlink("sugar1");
    for (int i = 0; i < 1; ++i) {
        p=fork();
        if(p==0){
            strcpy(c1.name,"chef1");
            c1.neededIngredients[0]=3;
            c1.neededIngredients[1]=4;
            break;
        }
        p=fork();
        if(p==0){
            strcpy(c2.name,"chef2");
            c2.neededIngredients[0]=2;
            c2.neededIngredients[1]=4;
            break;
        }
        p=fork();
        if(p==0){
            strcpy(c3.name,"chef3");
            c3.neededIngredients[0]=2;
            c3.neededIngredients[1]=3;
            break;
        }
        p=fork();
        if(p==0){
            strcpy(c4.name,"chef4");
            c4.neededIngredients[0]=1;
            c4.neededIngredients[1]=4;
            break;
        }
        p=fork();
        if(p==0){
            strcpy(c5.name,"chef5");
            c5.neededIngredients[0]=1;
            c5.neededIngredients[1]=3;
            break;
        }p=fork();
        if(p==0){
            strcpy(c6.name,"chef6");
            c6.neededIngredients[0]=1;
            c6.neededIngredients[1]=2;
            break;
        }
        w=fork();
        if(w==0){
            strcpy(w1.name,"wholesaler");
            break;
        }
    }

    if(p==0){
        /*while(1){
            /*sem_getvalue(egg,&valEgg);
            sem_getvalue(flour,&valFlour);
            sem_getvalue(butter,&valButter);
            sem_getvalue(sugar,&valSugar);*/
            allChef();
            /*if((valEgg == 0) && (valFlour == 0) && (valButter == 0) && (valSugar == 0)){
                sem_wait(s);
                aa();
                sem_post(s);
            }

            //aa();

        }*/
        //printf("CHEFFSS %d \n",getpid());
    }
    if(w==0){
        while(1){

            //bb();

        }
        //printf("SALER %d \n",getpid());
    }


    if(p>0 && w>0){
        while(wait(NULL)>0);
        printf("%d\n", strlen(sharedPlace));
        printf("ANAA\n");

    }
    sem_destroy(s);
    sem_destroy(egg);
    sem_destroy(flour);
    sem_destroy(butter);
    sem_destroy(sugar);
    sem_unlink("sem1");
    sem_unlink("egg1");
    sem_unlink("flour1");
    sem_unlink("butter1");
    sem_unlink("sugar1");

    return 0;
}
void aa(){
    sem_wait(s);
    /*int eggV,flourV,butterV,sugarV;
    sem_getvalue(egg,&eggV);
    sem_getvalue(flour,&flourV);
    sem_getvalue(butter,&butterV);
    sem_getvalue(sugar,&sugarV);*/
    if((sharedPlace[0]==BUTTER && sharedPlace[1]==SUGAR) ||
            (sharedPlace[1]==BUTTER && sharedPlace[0]==SUGAR)){
        prepareChef(c1);

    }
    if((sharedPlace[0]==FLOUR && sharedPlace[1]==SUGAR) ||
       (sharedPlace[1]==FLOUR && sharedPlace[0]==SUGAR)){
        prepareChef(c2);

    }
    if((sharedPlace[0]==FLOUR && sharedPlace[1]==BUTTER) ||
       (sharedPlace[1]==FLOUR && sharedPlace[0]==BUTTER)){
        prepareChef(c3);

    }
    if((sharedPlace[0]==EGGS && sharedPlace[1]==SUGAR) ||
       (sharedPlace[1]==EGGS && sharedPlace[0]==SUGAR)){
        prepareChef(c4);

    }
    if((sharedPlace[0]==EGGS && sharedPlace[1]==BUTTER) ||
       (sharedPlace[1]==EGGS && sharedPlace[0]==BUTTER)){
        prepareChef(c5);

    }
    if((sharedPlace[0]==EGGS && sharedPlace[1]==FLOUR) ||
       (sharedPlace[1]==EGGS && sharedPlace[0]==FLOUR)){
        prepareChef(c6);

    }
    /*
    if((butterV == 1) && (sugarV == 1)){
        prepareChef(c1);
        sem_wait(butter);
        sem_wait(sugar);
    }
    if((flourV == 1) && (sugarV == 1)){
        prepareChef(c2);
        sem_wait(flour);
        sem_wait(sugar);
    }
    if((flourV == 1) && (butterV == 1)){
        prepareChef(c3);
        sem_wait(flour);
        sem_wait(butter);
    }
    if((eggV == 1) && (sugarV == 1)){
        prepareChef(c4);
        sem_wait(sugar);
        sem_wait(egg);
    }
    if((eggV == 1) && (butterV == 1)){
        prepareChef(c5);
        sem_wait(butter);
        sem_wait(egg);
    }
    if((eggV == 1) && (flourV == 1)){
        prepareChef(c6);
        sem_wait(flour);
        sem_wait(egg);
    }*/
    fprintf(stdout,"%s has obtained the dessert and left to sell it\n",w1.name);
    sharedPlace[0]=NOPE;
    sharedPlace[1]=NOPE;
    sem_post(s);
}
void prepareChef(struct cheff c){
    printf("%d,%d,%d\n",c.neededIngredients[0],c.neededIngredients[1],getpid());
    printf("%s has taken the %s\n",c.name,ingredients(c.neededIngredients[0]));
    printf("%s has taken the %s\n",c.name,ingredients(c.neededIngredients[1]));
    printf("%s is preparing the dessert\n",c.name);
    printf("%s has delivered the dessert to the wholesaler\n",c.name);
}
void allChef(){
    chefStat(c1);
    chefStat(c2);
    chefStat(c3);
    chefStat(c4);
    chefStat(c5);
    chefStat(c6);
}
void chefStat(struct cheff c){
    if(c.neededIngredients[0]==1 || c.neededIngredients[0]==2 || c.neededIngredients[0]==3 || c.neededIngredients[0]==4){
        if(c.neededIngredients[1]==1 || c.neededIngredients[1]==2 || c.neededIngredients[1]==3 || c.neededIngredients[1]==4)
            printf("%s is waiting for %s and %s\n",c.name,ingredients(c.neededIngredients[0]),ingredients(c.neededIngredients[1]));
    }}
void bb(){
    sem_wait(s);
    time_t t;
    srand((unsigned) time(&t));
    int a,b;
    while(!((a=rand()%3+1)!=(b=rand()%3+1)));
    w1.ingredients[0]=a;
    w1.ingredients[1]=b;
    sharedPlace[0]=a;
    sharedPlace[1]=b;
    /*if((a==BUTTER && b==SUGAR) || (b==BUTTER && a==SUGAR)){
        sem_post(butter);
        sem_post(sugar);
    }
    if((a==FLOUR && b==SUGAR) || (b==FLOUR && a==SUGAR)){
        sem_post(flour);
        sem_post(sugar);
    }
    if((a==FLOUR && b==BUTTER) || (b==FLOUR && a==BUTTER)){
        sem_post(flour);
        sem_post(butter);
    }
    if((a==EGGS && b==SUGAR) || (b==EGGS && a==SUGAR)){
        sem_post(egg);
        sem_post(sugar);
    }
    if((a==EGGS && b==BUTTER) || (b==EGGS && a==BUTTER)){
        sem_post(egg);
        sem_post(butter);
    }
    if((a==EGGS && b==FLOUR) || (b==EGGS && a==FLOUR)){
        sem_post(egg);
        sem_post(flour);
    }*/
    printf("%s delivers %s and %s\n",w1.name,ingredients(w1.ingredients[0]),ingredients(w1.ingredients[1]));
    sem_post(s);
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
