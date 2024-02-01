#include <stdio.h>      
#include <stdlib.h>
#include <float.h>
#include <string.h> 
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
#include "shared_data.h"

#define BUFFER 256


struct config{
    char Hostname[256];
    int PortNumer;
    char GameKindName[256];
};


int main(int argc, char* argv[]){

    signal(SIGPIPE, handler);
    
    // init Game-id and Spielernummer
    char *gameID = (char*) malloc (sizeof(char)* 13); //to avoid avoid stack confusion
    
    // char gameID[13]="";
    int spielernummer = -1;
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
                if(strlen(optarg)==13){
                    printf("%s\n optarg: ",optarg);
                    strcpy(gameID, optarg);
                } else {   
                    printf("Game ID should be 13 digits lang.\n");
                    return -1; }

                   
                

                //Determines whether game id is 13 digits long.
                // while(*(optarg+i) != '\0'){
                //     i++;
                //     count++;
                // }                 
                // if(count != 13){
                //     printf("Game ID should be 13 digits lang.\n");
                //     return -1;
                // }else{
                //     i = 0;
                //     while(*(optarg+i) != '\0'){
                //         gameID[i] = *(optarg+i);
                //         i++;
                //         printf("Game ID: %s\n", gameID);
                //     }  
                                        
                break;
            case 'p':
                //Determines whether spielernummer is 0 or 1.
                while(*(optarg+i) != '\0'){
                    i++;
                    count++;
                }                 
                if(count != 1){
                    printf("Spielernummer should 1 digit long.\n");
                    return -1;
                }else{
                   if((atoi(optarg) != 1) && (atoi(optarg) != 0)){
                        printf("Spielernummer should be 0 or 1.\n");
                        return -1;
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
        printf("%c",gameID[i]);
    
    }
    
  
    puts("");   
    printf("Spielernummer is:%d\n", spielernummer);
    printf("before palyer send");

    printf("GAMEID TEST BEFORE: %s\n", gameID);

    char playersend[15] = "PLAYER ";
     if(spielernummer == 0 ){
         strcat(playersend, "0\n");
     }else if(spielernummer == 1){
         strcat(playersend, "1\n");
     }else{
         strcat(playersend, " \n");
     }
     printf("playersend is %s\n",playersend);
     printf("GAMEID TEST AFTER: %s\n", gameID);

     
    
    //check conf file setting
    // printf("before, Hostname is: %s\n",config_server.Hostname);
    // printf("before, Posrtnumber is: %d\n",config_server.PortNumer); 
    // printf("before, GameKindName is: %s\n\n",config_server.GameKindName);

    config(fileName,&config_server);  

    char* p_hostname = config_server.Hostname; 
    char portnumber_string[5];
    sprintf(portnumber_string,"%d",config_server.PortNumer);
    char* p_portnumber = portnumber_string;
    char* p_gamekindname = config_server.GameKindName;
    
    printf("after, Hostname is: %s\n",config_server.Hostname);
    printf("after, Posrtnumber is: %d\n",config_server.PortNumer); 
    printf("after, GameKindNameis :%s\n",config_server.GameKindName); 
    
    

    printf("<< Dame Client start! >>\n\n");

    struct SharedData *sharedData;

    // Shared Memory Bereich
    int shm_id = shmget(IPC_PRIVATE, sizeof(struct SharedData), IPC_CREAT | 0666); 
    if (shm_id == -1){
        perror("Error creating shared memory segment.");
        exit(EXIT_FAILURE);
    }else {
        // Shared Memory Attach
        sharedData = (struct SharedData *)shmat(shm_id, NULL, 0);
        if ((intptr_t)sharedData == -1){
            perror("Error attaching shared memory");
            exit(EXIT_FAILURE);
        }

        // Initialize shared data
        strcpy(sharedData->gameName, p_gamekindname);
        sharedData->player.playerNum = 1;
        sharedData->totalPlayers = 2;
        sharedData->thinkerPID = getpid();
        sharedData->connectorPID = 0;
        

        

        
    }

    
    
    int pipe_fds[2];

    // pipe-Erstellung
    if(pipe(pipe_fds) == -1) {
        perror("Pipe konnte nicht erstellt werden.");
        exit(EXIT_FAILURE);
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
        close(pipe_fds[0]);
        pid = waitpid(pid, NULL, 0);
        printf("parent process end.\n");
        if (pid < 0) {
        perror ("Fehler beim Warten auf Kindprozess.");
        exit(EXIT_FAILURE);
        }

        if (shmdt(sharedData) == -1){
             perror("Error detaching shared memory");
             exit(EXIT_FAILURE);
    } 
        
    if (shmctl(shm_id, IPC_RMID, NULL) == -1){
        perror("Error removing shared memory");
        exit(EXIT_FAILURE);
    }else{
        printf("SHM delete \n");
    }
    }

    /* Child process-------> Connector */
    else{
        printf("Child process beginn.\n"); 
        // connect with Gameserver
        //get server ip address_server
        close(pipe_fds[1]);
        struct SharedData *sharedData = (struct SharedData *)shmat(shm_id, NULL, 0);
        sharedData->connectorPID = getpid();

        struct addrinfo hints;          
        struct addrinfo* results;        
                           
        memset(&hints, 0, sizeof(struct addrinfo)); 
        hints.ai_flags = AI_PASSIVE;            
        hints.ai_family = AF_INET;              
        hints.ai_socktype = SOCK_STREAM;         
        hints.ai_protocol = 0;         
        int return_server_ip;          
        return_server_ip = getaddrinfo(p_hostname,p_portnumber,&hints,&results); 
        if (return_server_ip != 0) {
            fprintf(stderr, "%s\n", gai_strerror(return_server_ip));
            return -1;
        }   
    
        struct sockaddr_in *addr;       
        addr = (struct sockaddr_in*)results->ai_addr;
        printf("server ip is : %s\n",inet_ntoa(addr->sin_addr));
    
        struct sockaddr_in address_server;                 
        address_server.sin_family = AF_INET;
        address_server.sin_port = htons(1357);
        address_server.sin_addr = addr->sin_addr; 
        //prepare socket
        int socket_fd = socket(PF_INET,SOCK_STREAM,0);  
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

        printf("%s\n", gameID);
        printf("%s\n", playersend);

        FILE* readFile = fdopen(socket_fd, "r");
        

        

        performConnection(socket_fd,gameID,playersend,shm_id,readFile);

        /* char* charbuffer = (char*)malloc(BUFFER * sizeof(char));
        ssize_t size;
        size = recv(socket_fd, charbuffer, BUFFER - 1, 0);
        if (size > 0) charbuffer[size] = '\0';
        printf("Server message: %s\n", charbuffer); */

        close(socket_fd);
        //free(charbuffer);

         if (shmdt(sharedData) == -1){
            perror("Error detaching shared memory");
            exit(EXIT_FAILURE);
    } 
        
    if (shmctl(shm_id, IPC_RMID, NULL) == -1){
        perror("Error removing shared memory");
        exit(EXIT_FAILURE);
    }else{
        printf("SHM deleted. \n");
    } 
        
    printf("Child process end.\n");
    }
     

    return 0;
}