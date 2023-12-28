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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <netdb.h>


#define GAMEKINDNAME Checkers
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"


int main(int argc, char* argv[]){

    // init Game-id and Spielernummer
    char game_id[13]={};
    int spielernummer = 0;

    //analyse Command Parameters as Game-Id and Spielnummer
    int parameters;
    while((parameters = getopt(argc,argv,"g:p:")) != -1){
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
        }
    }

    //check game ID and Spielernummer
    printf("game id is:");
    for (int i = 0; i < 13; i++){
        printf("%c",game_id[i]);
    }
    puts("");   
    printf("Spielernummer is:%d\n",spielernummer);
    

    printf("<< Dame Client start! >>\n\n");

    // connect with Gameserver

    //get server ip address_server
    struct addrinfo hints;          
    struct addrinfo *result;        
    int return_server_ip;                        
    memset(&hints, 0, sizeof(struct addrinfo)); 
        hints.ai_flags = AI_PASSIVE;            
        hints.ai_family = AF_INET;              
        hints.ai_socktype = SOCK_STREAM;         
        hints.ai_protocol = 0;                  
    return_server_ip = getaddrinfo(HOSTNAME, NULL,&hints,&result); 
    if (return_server_ip < 0) {
        fprintf(stderr, "%s\n", gai_strerror(return_server_ip));
        exit(1);
    }   
    struct sockaddr_in *addr;       
    addr = (struct sockaddr_in*)result->ai_addr;
    printf("server ip is : %s\n",inet_ntoa(addr->sin_addr));
    
    //prepare socket
    int socket_client = socket(PF_INET,SOCK_STREAM,0);   
    int socket_server;                             

    struct sockaddr_in address_server;                 
    address_server.sin_family = AF_INET;
    address_server.sin_port = htons(PORTNUMBER);

    // connect with server：
    address_server.sin_addr = addr->sin_addr;   
    if(connect(socket_client,(struct sockaddr*) &address_server, sizeof(address_server)) == 0) {
      printf("Verbindung mit %s hergestellt.\n",inet_ntoa(address_server.sin_addr));
    }else{
        printf("connection failed.\n");
    }
    freeaddrinfo(result);

    return 0;
}