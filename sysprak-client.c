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
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "handler.h"
#include "performConnection.h"
#include "config.h"

#define BUFFER 256
#define GAMEKINDNAME "Checkers"
#define PORTNUMBER "1357"
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

struct config{
    char Hostname[256];
    int PortNumer;
    char GameKindName[256];
};

struct Player{
    int playerNum;
    int playerName;
    bool isReady;
};

struct SharedData{
    struct Player player;
    char gameName[256];
    int totalPlayers;
    pid_t thinkerPID;
    pid_t connectorPID;
};


int main(int argc, char* argv[]){
    signal(SIGPIPE, handler);
    

    // init Game-id and Spielernummer
    char gameID[14]={};
    int playerKommando = 0;
   // char parameterName[256];
    struct config config_server = {"un know",0,"un know"};
    char fileName[256]="client.conf\0";

    //analyse Command Parameters as Game-Id, Spielnummer and configuration
    int parameters;
    while((parameters = getopt(argc,argv,"g:p:c::")) != -1){
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
                        gameID[i] = *(optarg+i);
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
                    playerKommando = atoi(optarg);
                   }                  
                }  
                break;    
            case 'c':   //analysis configuration 
                strcpy(fileName,optarg);
                break;       
        }
    }

    //check game ID,Spielernummer and configuration
    strcpy(gameID,"0sk8wc9exo1g1");
    
    printf("game id is:");
    for (int i = 0; i < 13; i++){
        printf("%c",gameID[i]);
    }
    puts("");   
    printf("Spielernummer is:%d\n", playerKommando);
    

    printf("before, Hostname is: %s\n",config_server.Hostname);
    printf("before, Posrtnumber is: %d\n",config_server.PortNumer); 
    printf("before, GameKindName is: %s\n\n",config_server.GameKindName);

    config(fileName,&config_server);


    printf("after, Hostname is: %s\n",config_server.Hostname);
    printf("after, Posrtnumber is: %d\n",config_server.PortNumer); 
    printf("after, GameKindNameis :%s\n",config_server.GameKindName); 
    
    
    printf("<< Dame Client start! >>\n\n");

    // Shared Memory Bereich
    int shm_id = shmget(IPC_PRIVATE, sizeof(struct SharedData), IPC_CREAT | 0666); 
    if (shm_id == -1){
        perror("Error creating shared memory segment.");
        exit(EXIT_FAILURE);
    }else {
        // Shared Memory Attach
        struct SharedData *sharedData = (struct SharedData *)shmat(shm_id, NULL, 0);
        if ((intptr_t)sharedData == -1){
            perror("Error attaching shared memory");
            exit(EXIT_FAILURE);
        }

        // Initialize shared data
        strcpy(sharedData->gameName, GAMEKINDNAME);
        sharedData->player.playerNum = 1;
        sharedData->totalPlayers = 2;
        sharedData->thinkerPID = getpid();
        sharedData->connectorPID = 0;
    }
          
    pid_t pid  = fork();


    if (pid< 0) {
        perror ("Fehler bei fork().");
        exit(EXIT_FAILURE);
    } 
    // parentprocess
    if (pid > 0) {
        printf("Parent process beginn.\n");
        //wait for childprocess
        pid = waitpid(pid, NULL, 0);
        printf("parent process end.\n");
        if (pid < 0) {
        perror ("Fehler beim Warten auf Kindprozess.");
        exit(EXIT_FAILURE);
        }
    }

    /* Child process-------> Connector */
    else{
        printf("Child process beginn.\n"); 
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
        int socket_fd = socket(AF_INET,SOCK_STREAM,0);  
        if  (socket_fd == -1){
            perror("Error by creating socket");
        }   
    

        // connect with server：
        if(connect(socket_fd,(struct sockaddr*) &address_server, sizeof(address_server)) == 0) {
        printf("Successfully connected to the server: %s \n",inet_ntoa(address_server.sin_addr));
        }else{
            perror("Connection failed. \n");
            close(socket_fd);
            exit(EXIT_FAILURE);
        }

        freeaddrinfo(results);

        performConnection(socket_fd);

        char* charbuffer = (char*)malloc(BUFFER * sizeof(char));
        ssize_t size;
        size = recv(socket_fd, charbuffer, BUFFER - 1, 0);
        if (size > 0) charbuffer[size] = '\0';
        printf("Server message: %s\n", charbuffer);

        close(socket_fd);
        free(charbuffer);

        /* if (shmdt(sharedData) == -1){
            perror("Error detaching shared memory");
            exit(EXIT_FAILURE);
        } */
        
        if (shmctl(shm_id, IPC_RMID, NULL) == -1){
            perror("Error removing shared memory");
            exit(EXIT_FAILURE);
        }

    printf("Child process end.\n");
    }
    
    

    return 0;
}