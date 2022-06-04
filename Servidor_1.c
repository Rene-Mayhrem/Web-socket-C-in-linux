/***************************************************************************************/
/* @file    server_secuencial.c                                                        */
/* @brief   Secuencial server. TCP sockets                                             */
/***************************************************************************************/

/*standard symbols */
#include <unistd.h>  

/* sockets */
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>

/* strings / errors*/
#include <errno.h>
#include <stdio.h> 
#include <string.h> 

/*Conversacion*/
#include <netinet/ip.h>  
#include <time.h> 
#include <sys/types.h> 
#include <sys/socket.h> 


/* server parameters */
#define SERV_PORT       8080              /* port */
#define SERV_HOST_ADDR "192.168.93.128"     /* IP, only IPV4 support  */
#define BUF_SIZE        100               /* Buffer rx, tx max size  */
#define BACKLOG         5                 /* Max. numero de clientes en espera  */

int main(int argc, char* argv[])          /* input arguments are not used */
{ 
    //sockfd:descriptor de archivo de socket que se crear para escuchar conexiones de clientes
    //sockfd:descriptor de archivo de socket que se crea cuando un cliente ya se conecta (con este descriptor podemos leer y escribir datos al cliente)
    int sockfd, connfd ;  /* listening socket and connection socket file descriptors */
    unsigned int len;     /* length of client address */
    struct sockaddr_in servaddr, client; 
    
    int  len_rx, len_tx = 0;                     /* received and sent length, in bytes */
    char buff_tx[BUF_SIZE] = "Hello client, I am the server";
    char buff_rx[BUF_SIZE];   /* buffers for reception  */

    /*Variables para conversacion*/
    char cadena[BUF_SIZE];
    //int listen_fd, comm_fd; //SI
    //struct sockaddr_in servaddr; //SI
    FILE *myf = fopen("conversacion_servidor.txt", "a"); //Apuntador de tipo File para guardar la conversacion en un archivo txt
    /*Variables de la libreria Time*/
    time_t t;
    struct tm *tm;
    char hora[100];
    char *tmp;
    //sendline-->Guarda los mensajes que queremos enviar
    char sendline[100] = "Usando el Puerto 22000\n";

    
     
    /* socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) 
    { 
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Socket successfully created..\n"); 
    }
    
    /* clear structure */
    memset(&servaddr, 0, sizeof(servaddr));
  
    /* assign IP, SERV_PORT, IPV4 */
    servaddr.sin_family      = AF_INET; //TIPO TCP -->IMPLICA INTERNET
    servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); 
    servaddr.sin_port        = htons(SERV_PORT); 
    
    
    /* Bind socket */ //Asignar el Socket a una direccion Ip y un  puerto
    /*Llmada al sistema bind
    1-. sockdf: descriptor de archivo-->es el que ha creado el socket
    2-. (struct sockaddr *)&servaddr: Puntero a la estructura definida en los campos del servidor
    3-. sizeof(servaddr): Tamano en Bytes de esa estructura*/
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) 
    { 
        fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Socket successfully binded \n");
    }
  
    /* Listen */ //Hace que el socket se vuelva pasivo, Es decir esta pendiente de las peticiones de los clientes 
   /*1-. sockdf: descriptor de archivo-->es el que ha creado el socket
     2-.BACKLOG: numero maximo de conexiones pendientes  */
    if ((listen(sockfd, BACKLOG)) != 0) 
    { 
        fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n", errno, strerror( errno ));
        return -1;
    } 
    else
    {
        printf("[SERVER]: Listening on SERV_PORT %d \n\n", ntohs(servaddr.sin_port) ); 
    }
    
    len = sizeof(client); //Tamano en Bytes de la estructura del cliente para el formato de la direccion 
  
      /* Accept the data from incoming sockets in a iterative way */
      while(1) //ciclo infinito--> Que va ir acceptando datos de los sockets que van entrando
      {
        /*accept: Va a permitir que los clientes se conecten, y se va quedar en espera, hasta que un cliente trate de conectarse 
        y cuando ese cliente se conecte se creara un nuevo socket, y se administrara por medio del descriptor "connfd"
        estructura de accept:
        1-. sockdf: descriptor de archivo-->es el que ha creado el socket 
        2-. (struct sockaddr *)&client: Puntero a la estructura definida en los campos del cliente--> 
                                        cuando la funcion retorne-->se cargaran dichos descriptores
        3-. Tamano de la estructura sockaddr del cliente*/
        connfd = accept(sockfd, (struct sockaddr *)&client, &len); 
        if (connfd < 0) 
        { 
            fprintf(stderr, "[SERVER-error]: connection not accepted. %d: %s \n", errno, strerror( errno ));
            return -1;
        } 
        else
        {   
            fprintf(stderr, "[SERVER]: connection accepted.");           
            while(1) /* leer datos del cliente hasta que este cierre */ 
            {  
                /* read client message, copy it into buffer */
                /*Parametros de read
                1-. connfd: descriptor de archivo
                2-. buff_rx: aqui se cargan los datos leidos 
                3-. sizeof(buff_rx): Tamano del buffer*/
                //buff_rx = strstr(cadena);
                
                //len_rx = read(connfd,cadena, sizeof(cadena));
                len_rx=0;
                //len_rx = read(connfd, buff_rx, sizeof(buff_rx));    
                
                if(len_rx == -1)
                {
                    fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror( errno ));
                }
                //else if(len_rx == 0) /* if length is 0 client socket closed, then exit */
                else if(strstr(cadena, "adios") || strstr(sendline, "adios"))
                {
                    printf("[SERVER]: client socket closed \n\n");
                    close(connfd); //Srvidor cierra el descriptor correcpondiente al socket que estaba conectado con el cliente      
                    bzero(cadena, 100);
                    break; 
                }
                else
                {
                    //Se comeinza una conversacion 
                    //write(connfd, buff_tx, strlen(buff_tx));
                    //printf("[SERVER]: %s \n", buff_rx);
                    while(!strstr(cadena, "adios") && !strstr(sendline, "adios")){
                        bzero(cadena, 100);
                        t = time(NULL);
                        tm = localtime(&t);
                        strftime(hora, 100, "\n otro usuario (%H:%M) -> ", tm);
                        read(connfd, cadena, sizeof(cadena));
                        tmp = strcat(hora, cadena);
                        printf("%s", tmp);
                        fputs(tmp, myf);
                        if(!strstr(cadena, "adios")){
                            strftime(hora, 100, "\n yo (%H:%M) -> ", tm);
                            printf("%s", hora);
                            gets(sendline);
                            tmp = strcat(hora, sendline);
                            write(connfd, sendline, strlen(sendline)+1);
                            fputs(tmp, myf);
                        }
    
                    } 
                }            
            }  
        }                      
    }    
} 