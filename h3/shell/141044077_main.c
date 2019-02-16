/*  MUSTAFA BINGUL 141044077    */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

#define BUF 1024
#define HIST 50
char historyRepo[HIST][BUF];
int historyindex=0;

void shellPromptPrint(){
    char path[1024];
    getcwd(path, sizeof(path));
    printf("-mb-> %s # ",path);
}
void readCommandLine(char *bufferLine){
    if(!fgets(bufferLine,BUF,stdin)){
        fprintf(stderr,"Cannot read command..!");
        exit(-1);
    }
}
void parseCommandLine(char *parseLine,char *commandTokens[],int *tokenCount){

    int i=0;
    char *token;
    token = strtok(parseLine," \n\t");

    while(token != NULL){
        //printf("%s\n",token);
        commandTokens[i]=token;
        i++;
        token = strtok(NULL," \n\t");
    }
    *tokenCount=i;

}
void help()
{
    printf("- ls; which will list file type, access rights, file size and file name of all files in the present working\n"
                   "directory\n");
    printf("- pwd; which will print the present working directory\n") ;
    printf("- cd; which will change the present working directory to the location provided as argument\n");
    printf("- help; which will print the list of supported commands\n");
    printf("- cat; which will print on standard output the contents of the file provided to it as argument\n");
    printf("- wc; which will print on standard output the number of lines in the file provided to it as argument\n");
    printf("- history; which will print on standard output all previously typed command\n");
    printf("- history n^th; which will print on standard output nth previously typed command\n");
    printf("- exit; which will exit the shell\n");
}

int fileWrite(int fd,char *filename){

    FILE * file = fdopen(fd,"r");
    if(file==NULL)
        return -1;
    FILE * fileNew = fopen(filename,"w");
    char buffer[BUF];

    while(fgets(buffer,BUF,file)!=NULL){
        fprintf(fileNew,"%s",buffer);
    }

    fclose(file);
    fclose(fileNew);
    return 1;
}
int fileRead(char *filename){

    //perror("READ my.y");
    FILE * file = fopen(filename,"r");
    if(file==NULL)
        return -1;
    char buffer[BUF];

    while(fgets(buffer,BUF,file)!=NULL){
        printf("%s",buffer);
        fflush(stdout);
    }
    fclose(file);
    return 1;
}
int operate(char *tokens[], int count, char *_systemFilePath) {

    int pipe1[2];
    int pipe2[2];
    int pipe3[2];
    int pipeFd[2];
    char path[BUF];

    if(strcmp(tokens[0],"exit")==0){
        exit(0);
    }
    else if(strcmp(tokens[0],"pwd")==0){
        getcwd(path,sizeof(path));
        fprintf(stdout,"%s\n",path);
    }
    else if(strcmp(tokens[0],"help")==0){
        help();
    }
    else if(strcmp(tokens[0],"cd")==0){
        if(chdir(tokens[1])==-1)
        {
            fprintf(stdout,"there is not such a directory.\n");
        }
    }
    if(count == 1){
        ///// **************
        if((strcmp(tokens[0],"ls")==0)){
            /*cd den sonra path deişiyor ve ls cekemiyor...!*/

            if(fork()==0){
                //printf("TEKLİ LSSS \n\n");

                strcat(_systemFilePath,"/");
                strcat(_systemFilePath,tokens[0]);
                //printf("%s <\n",_systemFilePath);
                execl(_systemFilePath,tokens[0],NULL);
                exit(0);
            }
            /*else
                waitpid(p,0,0);*/
        }
        else if((strcmp(tokens[0],"wc")==0)){
            if(fork()==0){
                strcat(_systemFilePath,"/");
                strcat(_systemFilePath,tokens[0]);
                //printf("%s <\n",_systemFilePath);
                //fileRead(tokens[1]);
                execl(_systemFilePath,tokens[0],NULL);
                exit(0);
            }
        }
        else if((strcmp(tokens[0],"cat")==0)){
            if(fork()==0){
                strcat(_systemFilePath,"/");
                strcat(_systemFilePath,tokens[0]);
                //printf("%s <\n",_systemFilePath);
                //fileRead(tokens[1]);
                execl(_systemFilePath,tokens[0],NULL);
                exit(0);
            }
        }
    }
    if(count==2){

        if((strcmp(tokens[0],"cat")==0) && (tokens[1]!=NULL)){
            //perror(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
            pipe(pipeFd);
            if(fork()==0){

                dup2(pipeFd[1],STDOUT_FILENO);
                close(pipeFd[0]);
                //close(pipeFd[0]);

                if(fileRead(tokens[1])==-1){
                    fprintf(stdout,"there is not a file: %s\n",tokens[1]);
                }
                exit(0);
            }
            if(fork()==0){
                dup2(pipeFd[0],STDIN_FILENO);
                close(pipeFd[0]);
                close(pipeFd[1]);

                strcat(_systemFilePath,"/");
                strcat(_systemFilePath,tokens[0]);

                execl(_systemFilePath,tokens[0],NULL);
                exit(0);
            }
            close(pipeFd[0]);
            close(pipeFd[1]);
        }
        if((strcmp(tokens[0],"wc")==0) && (tokens[1]!=NULL)){
            //perror(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
            pipe(pipeFd);
            if(fork()==0){

                dup2(pipeFd[1],STDOUT_FILENO);
                close(pipeFd[0]);
                //close(pipeFd[0]);

                if(fileRead(tokens[1])==-1){
                    fprintf(stdout,"there is not a file: %s\n",tokens[1]);
                }
                exit(0);
            }
            if(fork()==0){
                dup2(pipeFd[0],STDIN_FILENO);
                close(pipeFd[0]);
                close(pipeFd[1]);

                strcat(_systemFilePath,"/");
                strcat(_systemFilePath,tokens[0]);

                execl(_systemFilePath,tokens[0],NULL);
                exit(0);
            }
            close(pipeFd[0]);
            close(pipeFd[1]);
        }
    }
    if(count==3){
        if((strcmp(tokens[1],">")==0) && (tokens[2]!=NULL)){
            //perror(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
            pipe(pipeFd);
            if(fork()==0){

                //dup2(pipeFd[0],STDIN_FILENO);
                close(pipeFd[1]);
                //close(pipeFd[0]);

                fileWrite(pipeFd[0],tokens[2]);
                exit(0);
            }
            if(fork()==0){
                dup2(pipeFd[1],STDOUT_FILENO);
                close(pipeFd[0]);
                close(pipeFd[1]);

                strcat(_systemFilePath,"/");
                strcat(_systemFilePath,tokens[0]);

                execl(_systemFilePath,tokens[0],NULL);
                exit(0);
            }
            close(pipeFd[0]);
            close(pipeFd[1]);
        }
        if((strcmp(tokens[1],"|")==0) && (tokens[2]!=NULL)){
            //perror(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
            pipe(pipeFd);
            if(fork()==0){

                dup2(pipeFd[0],STDIN_FILENO);
                close(pipeFd[1]);
                close(pipeFd[0]);

                strcat(_systemFilePath,"/");
                strcat(_systemFilePath,tokens[2]);

                execl(_systemFilePath,tokens[2],NULL);
                exit(0);
            }
            if(fork()==0){
                dup2(pipeFd[1],STDOUT_FILENO);
                close(pipeFd[0]);
                close(pipeFd[1]);

                strcat(_systemFilePath,"/");
                strcat(_systemFilePath,tokens[0]);

                execl(_systemFilePath,tokens[0],NULL);
                exit(0);
            }
            close(pipeFd[0]);
            close(pipeFd[1]);
        }
        if((strcmp(tokens[1],"<")==0) && (tokens[2]!=NULL)){
            //perror(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
            pipe(pipeFd);
            if(fork()==0){

                dup2(pipeFd[1],STDOUT_FILENO);
                close(pipeFd[0]);
                //close(pipeFd[0]);

                fileRead(tokens[2]);
                exit(0);
            }
            if(fork()==0){
                dup2(pipeFd[0],STDIN_FILENO);
                close(pipeFd[0]);
                close(pipeFd[1]);

                strcat(_systemFilePath,"/");
                strcat(_systemFilePath,tokens[0]);

                execl(_systemFilePath,tokens[0],NULL);
                exit(0);
            }
            close(pipeFd[0]);
            close(pipeFd[1]);
        }
    }
    if(count==5) {

        if ((strcmp(tokens[1], "|") == 0) && (strcmp(tokens[3], "|") == 0)) {
            //printf(" TWO (| |) \n");
            pipe(pipe1);
            if (fork() == 0) {
                dup2(pipe1[1], STDOUT_FILENO);
                close(pipe1[0]);
                close(pipe1[1]);

                strcat(_systemFilePath, "/");
                strcat(_systemFilePath, tokens[0]);

                execl(_systemFilePath, tokens[0], NULL);
                exit(0);
            }
            pipe(pipe2);
            if (fork() == 0) {
                dup2(pipe1[0], STDIN_FILENO);
                dup2(pipe2[1], STDOUT_FILENO);

                close(pipe1[0]);
                close(pipe1[1]);
                close(pipe2[0]);
                close(pipe2[1]);

                strcat(_systemFilePath, "/");
                strcat(_systemFilePath, tokens[2]);

                execl(_systemFilePath, tokens[2], NULL);
                exit(0);
            }
            /*anaproceste kapalılar!! kullanılmıyorlar.*/
            close(pipe1[0]);
            close(pipe1[1]);
            if (fork() == 0) {
                dup2(pipe2[0], STDIN_FILENO);
                close(pipe2[0]);
                close(pipe2[1]);

                strcat(_systemFilePath, "/");
                strcat(_systemFilePath, tokens[4]);

                execl(_systemFilePath, tokens[4], NULL);
                exit(0);
            }
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);


        }
        if ((strcmp(tokens[1], "|") == 0) && (strcmp(tokens[3], ">") == 0)) {

            pipe(pipe1);
            if (fork() == 0) {
                dup2(pipe1[1], STDOUT_FILENO);
                close(pipe1[0]);
                close(pipe1[1]);

                strcat(_systemFilePath, "/");
                strcat(_systemFilePath, tokens[0]);

                execl(_systemFilePath, tokens[0], NULL);
                exit(0);
            }
            pipe(pipe2);
            if (fork() == 0) {
                dup2(pipe1[0], STDIN_FILENO);
                dup2(pipe2[1], STDOUT_FILENO);

                close(pipe1[0]);
                close(pipe1[1]);
                close(pipe2[0]);
                close(pipe2[1]);

                strcat(_systemFilePath, "/");
                strcat(_systemFilePath, tokens[2]);

                execl(_systemFilePath, tokens[2], NULL);
                exit(0);
            }
            /*anaproceste kapalılar!! kullanılmıyorlar.*/
            close(pipe1[0]);
            close(pipe1[1]);
            if (fork() == 0) {
                //dup2(pipe2[0], STDIN_FILENO);
                //close(pipe2[0]);
                close(pipe2[1]);

                if(fileWrite(pipe2[0],tokens[4])==-1){
                    fprintf(stdout,"there is not a file: %s\n",tokens[4]);
                }
                exit(0);
            }
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);
        }
    }
    if(count==7){
        if((strcmp(tokens[1], "|") == 0) && (strcmp(tokens[3], "|") == 0) && (strcmp(tokens[5], ">") == 0)){
            //perror("ENUZUNNNNNNNNNN \n");
            pipe(pipe1);
            if (fork() == 0) {
                dup2(pipe1[1], STDOUT_FILENO);
                close(pipe1[0]);
                close(pipe1[1]);

                strcat(_systemFilePath, "/");
                strcat(_systemFilePath, tokens[0]);

                execl(_systemFilePath, tokens[0], NULL);
                exit(0);
            }
            pipe(pipe2);
            if (fork() == 0) {
                dup2(pipe1[0], STDIN_FILENO);
                dup2(pipe2[1], STDOUT_FILENO);

                close(pipe1[0]);
                close(pipe1[1]);
                close(pipe2[0]);
                close(pipe2[1]);

                strcat(_systemFilePath, "/");
                strcat(_systemFilePath, tokens[2]);

                execl(_systemFilePath, tokens[2], NULL);
                exit(0);
            }
            close(pipe1[0]);
            close(pipe1[1]);
            pipe(pipe3);
            if (fork() == 0) {
                dup2(pipe2[0], STDIN_FILENO);
                dup2(pipe3[1],STDOUT_FILENO);

                close(pipe1[0]);
                close(pipe1[1]);
                close(pipe2[0]);
                close(pipe2[1]);
                close(pipe3[0]);
                close(pipe3[1]);

                strcat(_systemFilePath, "/");
                strcat(_systemFilePath, tokens[4]);

                execl(_systemFilePath, tokens[4], NULL);
                exit(0);

            }
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);
            if(fork()==0){

                /*close(pipe1[0]);
                close(pipe1[1]);
                close(pipe2[0]);
                close(pipe2[1]);*/
                close(pipe3[1]);

                if(fileWrite(pipe3[0],tokens[6])==-1){
                    fprintf(stdout,"there is not a file: %s\n",tokens[6]);
                }
                exit(0);
            }
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);
            close(pipe3[0]);
            close(pipe3[1]);

        }
    }

    for (int j = 0; j < count; ++j) {
            wait(NULL);
    }

    return 1;
}
void allHistory(){
    for (int i = 0; i < historyindex; ++i) {
        printf("%d %s",i,historyRepo[i]);
    }
}
void historyRecord(char *_command){
    strcpy(historyRepo[historyindex++],_command);
}
void handler(int signo){
    fprintf(stdout,"\nHave a nice day..! Good Studiess.\n");
    exit(0);
}

int main() {

    char line[BUF];
    char *TOKENS[BUF];
    int tokCount;
    char oldPath[BUF];
    getcwd(oldPath, sizeof(oldPath));
    struct sigaction sa;
    memset(&sa,0, sizeof(sa));
    sa.sa_handler=&handler;
    sigaction(SIGINT,&sa,NULL);
    sigset_t proc,prevProc;
    sigemptyset(&proc);
    sigaddset(&proc,SIGINT);
    while(1){


        shellPromptPrint();

        readCommandLine(line);

        historyRecord(line);
        parseCommandLine(line,TOKENS,&tokCount);
        if(strcmp("history",TOKENS[0])==0 &&(tokCount==2)){
            fprintf(stdout,"%s",historyRepo[atoi(TOKENS[1])]);
            continue;
        }
        if(strcmp("history",TOKENS[0])==0 &&(tokCount==1)){
            allHistory();
            continue;
        }

        sigprocmask(SIG_BLOCK,&proc,&prevProc);
        operate(TOKENS, tokCount, oldPath);
        sigprocmask(SIG_SETMASK,&prevProc,NULL);
    }

    return 0;
}