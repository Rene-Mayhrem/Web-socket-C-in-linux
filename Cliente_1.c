/*************************************************************************************/
/* @file    client_1.c                                                               */
/* @brief   This clients connects,                                                   */
/*          sends a text, reads what server and disconnects                          */
/*************************************************************************************/

#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>

/*Conversacion*/
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/ip.h> 
#include <time.h> 

#define SERVER_ADDRESS  "192.168.93.128"     /* server IP */
#define PORT            8080 

/* Test sequences */
char buf_tx[] = "Hello server. I am a client";     //Meaje de transmision 
char buf_rx[];                     /* receive buffer */ //Mensaje de recepcion
char cadena[];
 
/* This clients connects, sends a text and disconnects */
int main() 
{ 
    int sockfd; 
    struct sockaddr_in servaddr; 

    /*Conversacion*/
    //char cadena[100];
    //int sockfd;
    //struct sockaddr_in servaddr;
    FILE *myf = fopen("conversacion_cliente.txt", "a");
    time_t t;
    struct tm *tm;
    char hora[100];
    char *tmp;
    char sendline[100] = "Usando el Puerto 8080\n";
    
    /* Socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) 
    { 
        printf("CLIENT: socket creation failed...\n"); 
        return -1;  
    } 
    else
    {
        printf("CLIENT: Socket successfully created..\n"); 
    }
    
    //Se inicializa la estructura del servidor con 0 
    memset(&servaddr, 0, sizeof(servaddr));

    /* assign IP, PORT */
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr( SERVER_ADDRESS ); 
    servaddr.sin_port = htons(PORT); 
  
    /* try to connect the client socket to server socket */
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) 
    { 
        printf("connection with the server failed...\n");  
        return -1;
    }
    
    printf("connected to the server..\n"); 
  
    /* send test sequences*/
    //write(sockfd, buf_tx, sizeof(buf_tx));     
    //read(sockfd, buf_rx, sizeof(buf_rx));
    //printf("CLIENT:Received: %s \n", buf_rx);
    while(!strstr(cadena, "adios") || !strstr(sendline, "adios")){
        bzero(cadena, 100);
        t = time(NULL);
        tm = localtime(&t);
        strftime(hora, 100, "\n yo (%H:%M) -> ", tm);
        printf("%s", hora);
        gets(sendline);
        tmp = strcat(hora, sendline);
        fputs(tmp, myf);
        write(sockfd, sendline, strlen(sendline)+1);
        //write(sockfd, sendline, sizeof(sendline));
        if(!strstr(cadena, "adios")){
            strftime(hora, 100, "\n otro usuario (%H:%M) -> ", tm);
            read(sockfd, cadena, 100);
            tmp = strcat(hora, cadena);
            printf("%s", tmp);
            fputs(tmp, myf);
        }
    }  
    /* close the socket */
    bzero(cadena, 100);
    close(sockfd);
} 