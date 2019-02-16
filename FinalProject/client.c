/*  MUSTAFA BİNGÜL 141044077    */
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
void signalHandler(int signo){
    if(signo == SIGINT) {
        printf("SERVER SHUTDOWN, Bye Byee...\n");
        exit(1);
    }
}
int main(int argc, char* argv[]) {

    if(argc<6){
        fprintf(stderr,"Usage:: ./client [name][Q-C-T][degree][address][port]");
    }

    int socket_fd;
    struct sockaddr_in server;
    char buffer[2000], server_reply[2000];;

    signal(SIGINT,signalHandler);
    //create socket
    socket_fd = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_fd == -1)
    {
        printf("Could not create socket");
    }
    //puts("Socket created");
    server.sin_addr.s_addr = inet_addr(argv[4]);
    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(argv[5]) );
    //Connect to remote server
    if (connect(socket_fd , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    //puts("Connected\n");

    sprintf(buffer,"%s %s %s %s %s %d",argv[1],argv[2],argv[3],argv[4],argv[5],getpid());
    //write(socket_fd , buffer , strlen(buffer));
    fprintf(stderr,"Provider %s is requesting %s %s from server %s:%s\n",argv[1],argv[2],argv[3],argv[4],argv[5]);
    //Send some data
    if( send(socket_fd , buffer , strlen(buffer) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    //keep communicating with server
    while(1){

        //Receive a reply from the server
        if( recv(socket_fd , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        puts(server_reply);

    }
    close(socket_fd);


    return 0;
}