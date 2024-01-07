#include <stdio.h>      
#include <stdlib.h>
#include <float.h>
#include <unistd.h>
#include <getopt.h>     
#include <string.h>
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/msg.h>
#include "handler.h"
#include "performConnection.h"
#include "config.h"

#define GAMEKINDNAME "Checkers"
#define PORTNUMBER "1357"
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define BUFFER 256
#define SIZE 1024

struct config{
    char Hostname[256];
    int PortNumer;
    char GameKindName[256];
};

struct shmdata{
    char GameKindName[256];
    int Spielernummer;
    int Player_number;
    pid_t pid_parent;
    pid_t pid_child;

};

int main(int argc, char* argv[]){
    signal(SIGPIPE, handler);
    

    // init Game-id and Spielernummer
    char game_id[13]={};
    int spielernummer = 0;
   // char parameterName[256];
    struct config config_server = {"un know",0,"un know"};
    char fileName[256]="client.conf\0";

    //analyse Command Parameters as Game-Id, Spielnummer and configuration
    int parameters;
    while((parameters = getopt(argc,argv,"g:p:c:")) != -1){
        int i = 0;
        int count = 0;
        switch(parameters){
            case 'g':
                //Determines whether game id is 13 digits long.
                while(*(optarg+i) != '\0'){
                    i++;
                    count++;
                }                 
                if(count != 13){
                    printf("Game ID should be 13 digits lang.\n");
                }else{
                    i = 0;
                    while(*(optarg+i) != '\0'){
                        game_id[i] = *(optarg+i);
                        i++;
                    }  
                }                        
                break;
            case 'p':
                //Determines whether spielernummer is 1 or 2.
                while(*(optarg+i) != '\0'){
                    i++;
                    count++;
                }                 
                if(count != 1){
                    printf("Spielernummer should be 1 or 2.\n");
                }else{
                   if((atoi(optarg) != 1) && (atoi(optarg) != 2)){
                        printf("Spielernummer should be 1 or 2.\n");
                   }else{
                    spielernummer = atoi(optarg);
                   }                  
                }  
                break;    
            case 'c':   //analysis configuration 
                strcpy(fileName,optarg);
                break;       
        }
    }

    //check game ID,Spielernummer and configuration
    printf("game id is:");
    for (int i = 0; i < 13; i++){
        printf("%c",game_id[i]);
    }
    puts("");   
    printf("Spielernummer is:%d\n",spielernummer);
    

    printf("before, Hostname is: %s\n",config_server.Hostname);
    printf("before, Posrtnumber is: %d\n",config_server.PortNumer); 
    printf("before, GameKindName is: %s\n\n",config_server.GameKindName);

    config(fileName,&config_server);

    printf("after, Hostname is: %s\n",config_server.Hostname);
    printf("after, Posrtnumber is: %d\n",config_server.PortNumer); 
    printf("after, GameKindNameis :%s\n",config_server.GameKindName); 
    

    printf("<< Dame Client start! >>\n\n");

    /* add share memory */
    int shmid ;
    char *shmaddr ;
    struct shmid_ds shm_buf;
    int flag = 0 ;

    shmid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT|0600 ) ;
    if ( shmid < 0 ){
    perror("get shm  ipc_id error") ;
    return -1 ;
    }


    
    pid_t pid  = fork();
    if (pid< 0) {
        perror ("Fehler bei fork().");
        exit(EXIT_FAILURE);
    } 
    // parentprocess
    if (pid > 0) {
        printf("Parent process beginn.\n");

        flag = shmctl( shmid, IPC_STAT, &shm_buf) ;
    if ( flag == -1 ){
        perror("shmctl shm error") ;
        return -1 ;
    }
    printf("shm_segsz =%zu bytes\n", shm_buf.shm_segsz ) ;
    printf("parent pid=%d, shm_cpid = %d \n", getpid(), shm_buf.shm_cpid ) ;
    printf("chlid pid=%d, shm_lpid = %d \n",pid , shm_buf.shm_lpid ) ;

    shmaddr = (char *) shmat(shmid, NULL, 0 ) ;


    printf("%s", shmaddr) ;

    shmdt( shmaddr ) ;

    shmctl(shmid, IPC_RMID, NULL) ;



        //wait for childprocess
        pid = waitpid(pid, NULL, 0);
        printf("parent process end.\n");
        if (pid < 0) {
        perror ("Fehler beim Warten auf Kindprozess.");
        exit(EXIT_FAILURE);
        }
    }
    /* Child process */
    else{

    printf("Child process beginn.\n");
    
    /* attach shm to an address */
    shmaddr = (char *)shmat( shmid, NULL, 0 ) ;

    /* some operation to shm */
    strcpy( shmaddr, "Hi, in child process shm attached sucessful.\n") ;

    /* detach shm */
    shmdt( shmaddr ) ;

    // connect with Gameserver

    //get server ip address_server

    struct addrinfo hints;          
    struct addrinfo* results;        
                           
    memset(&hints, 0, sizeof(struct addrinfo)); 
        hints.ai_flags = AI_PASSIVE;            
        hints.ai_family = AF_INET;              
        hints.ai_socktype = SOCK_STREAM;         
        hints.ai_protocol = 0;         
    int return_server_ip;          
    return_server_ip = getaddrinfo(HOSTNAME, PORTNUMBER,&hints,&results); 
    if (return_server_ip != 0) {
        fprintf(stderr, "%s\n", gai_strerror(return_server_ip));
        return -1;
    }   
    
    struct sockaddr_in *addr;       
    addr = (struct sockaddr_in*)results->ai_addr;
    printf("server ip is : %s\n",inet_ntoa(addr->sin_addr));
    
    struct sockaddr_in address_server;                 
    address_server.sin_family = AF_INET;
    address_server.sin_port = htons(atoi(PORTNUMBER));
    address_server.sin_addr = addr->sin_addr; 
    //prepare socket
    int socket_client = socket(PF_INET,SOCK_STREAM,0);  
     if  (socket_client == -1){
        perror("Error by creating socket");
    }   
    
    // It should not be necessary to use a for loop.
    //struct addrinfo* record = NULL;
    /* for (record = results; record != NULL; record = record->ai_next) {
        socket_client = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
         
        if (connect(socket_client, record->ai_addr, record->ai_addrlen) != -1) {

            printf(" Successfully connected to the server: %s \n"，,inet_ntoa(address_server.sin_addr));
         //break;
        } else {
            perror("Connection failed. \n");
            close(socket_client);
            exit(EXIT_FAILURE);
        }    
    } */

    // connect with server：
    if(connect(socket_client,(struct sockaddr*) &address_server, sizeof(address_server)) == 0) {
      printf("Successfully connected to the server: %s \n",inet_ntoa(address_server.sin_addr));
    }else{
        perror("Connection failed. \n");
            close(socket_client);
            exit(EXIT_FAILURE);
    }

    freeaddrinfo(results);

    performConnection(socket_client);

    char* charbuffer = (char*)malloc(BUFFER * sizeof(char));
    ssize_t size;
    size = recv(socket_client, charbuffer, BUFFER - 1, 0);
    if (size > 0) charbuffer[size] = '\0';
    printf("Server message: %s\n", charbuffer);


    close(socket_client);
    free(charbuffer);
    printf("Child process end.\n");
    }
    return 0;
}