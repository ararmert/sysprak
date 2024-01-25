#include<sys/types.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<errno.h>
#include<signal.h>
#include<netdb.h>
#include<fcntl.h>
#include "shared.h" 

#define GAMEKINDNAME "Checkers"
#define PORTNUMBER "1357"
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"                   
#define BUFFER 256


struct SharedData {
     struct Player player;
    char gameName[256];
    
     int totalPlayers;
    pid_t thinkerPID;
    pid_t connectorPID;
 };



void handler(int s) {
     printf("Caught SIGPIPE\n");
}

void thinker() {
    printf("Thinker process has started.\n");


    int status;
    waitpid(-1, &status, 0);
}

// Prologphase der Kommunikation
void performConnection(int socket_fd) {

    // Receive the first server message
    char* charbuffer = (char*)malloc(BUFFER * sizeof(char));
    ssize_t size_received = recv(socket_fd, charbuffer, BUFFER - 1, 0);
    if (size_received > 0) { 
        charbuffer[size_received] = '\0';
        printf("%s\n", charbuffer);
    } else {
        perror("Error receiving the first server message.");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }


     // Set the socket to non-blocking mode
    int flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags == -1) {
        perror("Error getting socket flags");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("Error setting socket to non-blocking mode");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // Send the client version to the server
    char clientVersion[] = "VERSION 3.1\n"; 
    ssize_t sent_byte = send(socket_fd, clientVersion, strlen(clientVersion), 0);

    if(sent_byte == -1) {
        perror("Error sending client version.\n");
        close(socket_fd);
        exit(EXIT_FAILURE);
    
    } else { 
        printf("%zd bytes sent(Client Version).\n", sent_byte);
        

    }  

    // Add a delay
    usleep(500000);
    


    

     // Wait for the socket to be ready for reading or until timeout
    // if (waitForSocket(socket_fd, 5) == 0) {
    //     perror("Timeout waiting for server response");
    //     close(socket_fd);
    //     exit(EXIT_FAILURE);
    // }



    // Receive server response
    char* charbufferr = (char*)malloc(BUFFER * sizeof(char));
    ssize_t size_receivedd = recv(socket_fd, charbufferr, BUFFER - 1, 0);
    if (size_receivedd > 0) {
        charbufferr[size_receivedd] = '\0';
        printf("%s\n", charbufferr);
    
    } else {
        perror("Error receiving server response after sending client version.");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }


    // Send the Game-ID to the server
    char gameID[] = "ID 0sk8wc9exo1g1\n";
    ssize_t sent_gameid = send(socket_fd, gameID, strlen(gameID), 0);


    if(sent_gameid == -1) {
        perror("Error sending Game-ID.\n");
        close(socket_fd);
        exit(EXIT_FAILURE);
    } else { 
        printf("%zd bytes sent(Game-ID).\n", sent_byte);
    }

    // Add a delay (bunu perror icinde yapmaya calis)
    usleep(500000);

   

    // Receive server response
    char* charbufferrr = (char*)malloc(BUFFER * sizeof(char));
    ssize_t size_receiveddd = recv(socket_fd, charbufferrr, BUFFER - 1, 0);
    if (size_receiveddd > 0) {
    charbufferrr[size_receiveddd] = '\0';
    printf("%s\n", charbufferrr);
    
    char gName[20];
    strncpy(gName, &charbufferrr[10], 8);
    gName[8] = '\0';
    

    // Exit if game kind is not Checkers
    char strcheck[] = "Checkers";
    
    int compare = strcmp(gName, strcheck);
    if (compare != 0) {
        perror("Wrong GameKindName received from the server.\n");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    


    } else {
        perror("Error receiving server response after sending Game-ID");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

   
    

    

    
    // Send the PLAYER command to the server
    char playerKommando[] = "PLAYER \n";
    ssize_t sent_bytes = send(socket_fd, playerKommando, strlen(playerKommando), 0);

    if (sent_bytes == -1) {
        perror("Playerkommando konnte nicht gesendet werden.");
        fprintf(stderr, "Error code: %d\n", errno);
        close(socket_fd);
        exit(EXIT_FAILURE);
    } else printf("%zd bytes sent(Player).\n", sent_bytes);


    // Add a delay
    usleep(500000);



    struct Player player = {.totalPlayerNum = 2};
    


    // Receive server response after sending PLAYER command.
    char quickbuffer[50];
    char* playerbuffer = (char*)malloc(BUFFER * sizeof(char));
    ssize_t player_received = recv(socket_fd, playerbuffer, BUFFER - 1, 0);

    while (player_received > 0 && player_received < 100) playerbuffer[player_received] = '\0';
    {
        snprintf(quickbuffer, sizeof(quickbuffer), "%.42s", playerbuffer); // store the needed information from the server response in quickbuffer

        char* line = strtok(quickbuffer, "\n");

    while (line != NULL) {
        if (sscanf(line, "+ YOU %d Player %d", &player.playerNum, &player.playerName) == 2) {
            // Parse the first line
            printf("Player %d (Name: %d)\n", player.playerNum, player.playerName);
        } else if (sscanf(line, "+ TOTAL %d", &player.totalPlayerNum) == 1) {
            // Parse the second line
            printf("Total players: %d\n", player.totalPlayerNum);
        } else if (sscanf(line, "+ %d Player %d %d", &player.playerNum, &player.playerName, &player.isReady) == 3) {
            // Parse the third line
            if (player.isReady == 1) {
            printf("Is Player %d (Name: %d) ready? Yes!", player.playerNum, player.playerName);
            } else if (player.isReady == 0) {
                printf("Is Player %d (Name: %d) ready? No!", player.playerNum, player.playerName);
            }
        } else {
            // Else:
            printf("Unrecognized server response: %s\n", line);
        }

        // Move to the next line
        line = strtok(NULL, "\n");
    }
        
}


        // printf("%s\n", quickbuffer); to test

        // printf("%d\n", player_received); to print the size of buffer being used
    

    free(playerbuffer);
    







       
 

    // Close the socket when done
    close(socket_fd);

}

void connector(int socket_fd) {
    printf("Connector process has started.\n");

    performConnection(socket_fd);

    exit(EXIT_SUCCESS);
}



// Main function
int main(int argc, char** argv) {
    signal(SIGPIPE, handler);


    int socket_fd;
    char* charbuffer = (char*)malloc(BUFFER * sizeof(char));
    ssize_t size;

    // Create a TCP socket
    //int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    //if (socket_fd == -1) {
      //  perror("Failed to create socket");
       // exit(EXIT_FAILURE);
    //} else if (socket_fd == 3) {

      //  printf("Socket created successfully.\n");
    //}




    // Placeholder code up until the creation of the Elternprozess
    // TODO Replace with own implementation
    





    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    struct addrinfo* results = NULL;
    struct addrinfo* record = NULL;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(HOSTNAME, PORTNUMBER, &hints, &results) != 0) {
        perror("Failed to translate client socket.");
        return -1;
    }

    for (record = results; record != NULL; record = record->ai_next) {
        socket_fd = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
         if (socket_fd == -1) {
        perror("Error creating socket");
        continue;  
    }

        if (connect(socket_fd, record->ai_addr, record->ai_addrlen) != -1) {

            printf("Connected to the server: \n");
            break;

        } else {
            perror("Connection failed. \n");
            close(socket_fd);
            exit(EXIT_FAILURE);
        }
}

   freeaddrinfo(results);

    // Elternprozess
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error forking process.");
        exit(EXIT_FAILURE);

    } else if (pid == 0) {
        // Kindprozess (Connector)

        // Shared Memory Bereich
        int shm_id = shmget(IPC_PRIVATE, sizeof(struct SharedData), IPC_CREAT | 0666);
        if (shm_id < 0) {
            perror("Error creating shared memory segment.");
            exit(EXIT_FAILURE);
        } else if (shm_id > 0) {
        // Shared Memory Attach

        struct SharedData* sharedData = (struct SharedData*)shmat(shm_id, NULL, 0);

        
        if ((intptr_t)sharedData == -1) {
            perror("Error attaching shared memory");
            exit(EXIT_FAILURE);
        }

        //Initialize shared data
        strcpy(sharedData->gameName, GAMEKINDNAME);
        sharedData->player.playerNum = 1;
        sharedData->totalPlayers = 2;
        sharedData->thinkerPID = getpid();
        sharedData->connectorPID = 0;

        if(shmdt(sharedData) == -1) {
            perror("Error detaching shared memory");
            exit(EXIT_FAILURE);
        } exit(EXIT_SUCCESS);

        if(shmctl(shm_id, IPC_RMID, NULL) == -1) {
            perror("Error removing shared memory");
            exit(EXIT_FAILURE);
        } exit(EXIT_SUCCESS);

        



    }

}
        connector(socket_fd);
        exit(EXIT_SUCCESS);
    
        // Elternprozess
        thinker();
        // Warte auf die Terminierung des Kindprozesses
        int status;
        waitpid(pid, &status, 0);
    
    
    
    size = recv(socket_fd, charbuffer, BUFFER - 1, 0);
    if (size > 0) charbuffer[size] = '\0';
    printf("Server message: %s", charbuffer);

    

    close(socket_fd);
    free(charbuffer);
    

    return 0;


}



