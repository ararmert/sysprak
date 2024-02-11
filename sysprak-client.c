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
#include "performConnection.h"
#include "config.h"
#include "shared_data.h"
#include "move_wait_over.h"
#include "board.h"
// #include "sendingMoveFromPipe.h"
#include "signalHandler.h"


#define BUFFER 256


struct config{
    char Hostname[256];
    int PortNumer;
    char GameKindName[256];
};


int main(int argc, char* argv[]){

    
    // init Game-id and Spielernummer
    char *gameID = (char*) malloc (sizeof(char)* 13); //to avoid avoid stack confusion
    
    // char gameID[13]="";
    int spielernummer = -1;
   // char parameterName[256];
    struct config config_server = {"un know",0,"un know"};
    char fileName[256] = "client.conf\0";

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
                    printf("Game ID should be 13 digits long.\n");
                    return -1; }
            
                break;
            case 'p':
                //Determines whether spielernummer is 0 or 1.
                while(*(optarg+i) != '\0'){
                    i++;
                    count++;
                }                 
                if(count != 1){
                    printf("Spielernummer should be 1 digit long.\n");
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
    printf("GAME-ID: ");
    for (int i = 0; i < 13; i++){
        printf("%c",gameID[i]);
    
    }
    
  
    puts("");   
    printf("Spielernummer is: %d\n", spielernummer);
    

    char playersend[15] = "PLAYER ";
     if(spielernummer == 0 ){
         strcat(playersend, "0\n");
     }else if(spielernummer == 1){
         strcat(playersend, "1\n");
     }else{
         strcat(playersend, " \n");
     }
     printf("playersend is %s\n",playersend);
     printf("GAME-ID: %s\n", gameID);



    config(fileName,&config_server);  

    char* p_hostname = config_server.Hostname; 
    char portnumber_string[5];
    sprintf(portnumber_string,"%d",config_server.PortNumer);
    char* p_portnumber = portnumber_string;
    char* p_gamekindname = config_server.GameKindName;
    
    /* printf("Hostname: %s\n",config_server.Hostname);
    printf("Portnumber: %d\n",config_server.PortNumer); 
    printf("GameKindName: %s\n",config_server.GameKindName); 
     */

    printf("<< Dame Client start! >>\n\n");

    struct SharedData *sharedData;

    // Shared Memory Bereich about game information
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

    //SHM about piece in main get & at  
    int SHMSpielstand = shmget(IPC_PRIVATE, 24 * sizeof(struct Piece), IPC_CREAT | 0666);
    if (SHMSpielstand == -1){
        perror("Error shmget for spielstand");
        exit(EXIT_FAILURE);
    }else{
        printf("SHMSpielstand succesfully create\n");
    }
    //SHM about pice attach in main
    struct Piece *pieces = (struct Piece *)shmat(SHMSpielstand, NULL, 0);
    if ((intptr_t)pieces == -1) {
        perror("Error shmat SHMSpielstand");
        exit(EXIT_FAILURE);
    }
    



    int pipe_fds[2];
    // pipe-Erstellung
    if(pipe(pipe_fds) == -1) {
        perror("Pipe konnte nicht erstellt werden.");
        exit(EXIT_FAILURE);
    }

    pid_t pid  = fork();

    if (pid < 0) {
        perror ("Fehler bei fork().");
        exit(EXIT_FAILURE);
    } 
    // parent process
    if (pid > 0) {
        
        printf("Parent process Thinker beginn.\n");
        //wait for childprocess
        close(pipe_fds[0]);


        // 设置信号处理函数
        signal(SIGUSR1, signalHandler);
        
        //TO-DO

        pid = waitpid(pid, NULL, 0);

        if (pid < 0) {
        perror ("Fehler beim Warten auf Kindprozess");
        exit(EXIT_FAILURE);
        }


        //SHM about information dt & ctl in parent process
        if (shmdt(sharedData) == -1){
             perror("Error detaching shared memory");
             exit(EXIT_FAILURE);
        } 
        if (shmctl(shm_id, IPC_RMID, NULL) == -1){
            perror("Error removing shared memory");
            exit(EXIT_FAILURE);
        }else{
            printf("Information SHM in parent delete \n");
        }

        //SHM dt & ctr in parent-process
        if (shmdt(pieces) == -1) {
            perror("Error detaching SHMSpielstand in parent-process\n");
            exit(EXIT_FAILURE);
        }else{          
            printf("SHMSPielstand in parent-process detached\n");
        }
        if (shmctl(SHMSpielstand, IPC_RMID, NULL) == -1){
            perror("Error removing SHMSpielstand in parent-process");
            exit(EXIT_FAILURE);
        }else{
            printf("SHMSpielstand in parent-process deleted\n");
        }


    printf("Parent process end.\n");
    }

    /* Child process-------> Connector */
    else{
        printf("Child process beginn.\n"); 
        // connect with Gameserver
        //get server ip address_server
        close(pipe_fds[1]);
        struct SharedData *sharedData = (struct SharedData *)shmat(shm_id, NULL, 0);
        sharedData->connectorPID = getpid();
        shmdt(sharedData);
    
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
        printf("Server ip: %s\n",inet_ntoa(addr->sin_addr));
    
        struct sockaddr_in address_server;                 
        address_server.sin_family = AF_INET;
        address_server.sin_port = htons(1357);
        address_server.sin_addr = addr->sin_addr; 
        //prepare socket
        int socket_fd = socket(PF_INET,SOCK_STREAM,0);  
        if (socket_fd == -1){
            perror("Error creating socket.");
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

        

        FILE* readFile = fdopen(socket_fd, "r");
        
        performConnection(socket_fd,gameID,playersend,shm_id,readFile);

        // //Flag shouldThink !!!!Temporarilly in Notes, cuz if not it can not work
        // // 设置 shouldThink 标志
        //sharedData->shouldThink = true;

        // // 发送信号给 Thinker 进程
        //kill(sharedData->thinkerPID, SIGUSR1);
        // 发送信号给 Thinker 进程
    // printf("About to send SIGUSR1 signal to parent.\n");
    // kill(getppid(), SIGUSR1);
    // printf("SIGUSR1 signal sent to parent.\n");


        //SHM at in Childprocess
        struct Piece *pieces = (struct Piece *)shmat(SHMSpielstand, NULL, 0);
        if ((intptr_t)pieces == -1) {
        perror("Error shmat SHMSpielstand");
        exit(EXIT_FAILURE);
    }

        move_wait_over(socket_fd,readFile,pieces,&pipe_fds[0]);
        


        close(socket_fd);
       
        


    
    if (shmdt(sharedData) == -1){
             perror("Error detaching shared memory");
             exit(EXIT_FAILURE);
        } 

    if (shmdt(pieces) == -1) {
        perror("Error detaching SHMSpielstand in child-process\n");
        exit(EXIT_FAILURE);
    }else{
        printf("SHMSPilestand in child-process detached\n");
    }

    

    printf("Child process end.\n");
    }
     
    free(gameID);
    return 0;
}