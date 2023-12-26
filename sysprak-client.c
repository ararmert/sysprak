#include <stdio.h>      // for printf()
#include <stdlib.h>
#include <float.h>
#include <unistd.h>
#include <getopt.h>     // for getopt()
#include <string.h>
#include <sys/types.h>  // for socket()
#include <sys/socket.h> // for sockaddr
#include <netinet/in.h> // for sockaddr_in
#include <arpa/inet.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define GAMEKINDNAME Checkers
#define PORTNUMBER 1357
#define HOSTNAME sysprak.priv.lab.nm.ifi.lmu.de


int main(int argc, char* argv[]){

    // Command Parameters as Game Id and Spielnummer
    char game_id[13]={};
    int spielernummer = 0;

    int ret;
    while((ret = getopt(argc,argv,"g:p:")) != -1){
        int i = 0;
        int count = 0;
        switch(ret){

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
    printf("game-id is:");
    for (int i = 0; i < 13; i++){
        printf("%c",game_id[i]);
    }
    puts("");   
    printf("Spielernummer is:%d\n",spielernummer);
    

    printf("<< Dame Client start! >>\n\n");

    // get hostname
    char hostbuffer[256];
    int hostname;

    hostname = gethostname(hostbuffer,sizeof(hostbuffer));
    printf("hostname is ： %s\n",hostbuffer);

    //prepare socket
    /*int socket_server = socket(PF_INET,SOCK_STREAM,0);
    int socket_client;

    struct sockaddr_in address_client;
        address_client.sin_family = AF_INET;
        address_client.sin_port = htons(1357);
        address_client.sin_addr = "0.0.0.0";
    
    if(connect(socket_server,(struct sockaddr*)&address_client,sizeof(address_client)) == 0){
        printf("Connection with %s succeed.\n",inet_ntoa(address_client.sin_addr));
    }*/
    return 0;
}