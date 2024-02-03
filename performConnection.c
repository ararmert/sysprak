#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared_data.h" // the new header file
#include <ctype.h>



#define BUFFER 256


void performConnection(int socket_fd, char gameID[13], char playersend[], int shm_id, FILE* readFile) {
    // Receive the first server message
    char *firstbuff = (char *)malloc(BUFFER * sizeof(char));
    
    if (fgets(firstbuff, BUFFER, readFile) == NULL) {
        perror("Error receiving server message.");
        free(firstbuff);
        close(socket_fd);
        exit(EXIT_FAILURE);
    } else {
        printf("%s\n", firstbuff);
    }
    
    // Set the socket to non-blocking mode
    int flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("Error getting socket flags");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("Error setting socket to non-blocking mode");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // Send the client version to the server
    char clientVersion[] = "VERSION 3.3\n";
    ssize_t sent_byte = send(socket_fd, clientVersion, strlen(clientVersion), 0);

    if (sent_byte == -1)
    {
        perror("Error sending client version.\n");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("%zd bytes sent(Client Version).\n", sent_byte);
    }

    usleep(500000);

    // Set a timeout for receiving server responses (work in progress)
    // struct timeval timeout;
    // timeout.tv_sec = 5;
    // timeout.tv_usec = 0;

    // fd_set readfds;
    // FD_ZERO(&readfds);
    // FD_SET(socket_fd, &readfds);

    // // Wait for the socket to be ready for reading or until timeout
    // int ready = select(socket_fd + 1, &readfds, NULL, NULL, &timeout);

    // if (ready == -1) {
    //     perror("Error in select");
    //     close(socket_fd);
    //     exit(EXIT_FAILURE);
    // } else if (ready == 0) {
    //     // Timeout occurred
    //     perror("Timeout waiting for server response");
    //     close(socket_fd);
    //     exit(EXIT_FAILURE);
    // }



    // Receive newly added server response(Already happy with your AI?) and the response about Client Version
    char *secondbuff = (char *)malloc(BUFFER * sizeof(char));
    if (fgets(secondbuff, BUFFER, readFile) == NULL) {
        perror("Error receiving server message.");
        free(secondbuff);
        close(socket_fd);
        exit(EXIT_FAILURE);
    } else {
        printf("%s\n", secondbuff);
    }

    usleep(500000);

    char *thirdbuff = (char *)malloc(BUFFER * sizeof(char));
    if (fgets(thirdbuff, BUFFER, readFile) == NULL) {
        perror("Error receiving server response after sending client version.");
        free(thirdbuff);
        close(socket_fd);
        exit(EXIT_FAILURE);
    } else {
        printf("%s\n", thirdbuff);
    }
   
    usleep(500000);

    // Send the Game-ID to the server
     char gameIDs[18] = "ID ";
     
     
     strcat(gameIDs, gameID);
     strcat(gameIDs, "\n");
     printf("Game-ID: %s\n", gameIDs);
     ssize_t sent_gameid = send(socket_fd, gameIDs, strlen(gameIDs), 0);
     if (sent_gameid == -1)
     {
         perror("Error sending Game-ID.\n");
         close(socket_fd);
         exit(EXIT_FAILURE);
     }
     else
     {
         printf("%zd bytes sent(Game-ID).\n", sent_byte);
     }
    
    usleep(500000);


    // Receive server response about GameKind and quit if Gamekind is not 'Checkers'
    char *fourthbuff = (char *)malloc(BUFFER * sizeof(char));
    if (fgets(fourthbuff, BUFFER, readFile) == NULL) {
        perror("Error receiving server response after sending Game-ID.");
        free(fourthbuff);
        close(socket_fd);
        exit(EXIT_FAILURE);
    } else if (strcmp(fourthbuff, "+ PLAYING Checkers")) {
       
        printf("%s\n", fourthbuff);


        // if (strcmp(fourthbuff, "- Game does not exist")) {
        // perror("Error: Invalid Game-ID: This game does not exist.");
        // free(fourthbuff);
        // close(socket_fd);
        // exit(EXIT_FAILURE);
       

    } else {
        //printf("%s\n", fourthbuff);
        

        perror("Wrong game kind received from the server.");
        free(fourthbuff);
        close(socket_fd);
        exit(EXIT_FAILURE);

    }
    
    // save Gamename
    char *gamebuff = (char *)malloc(BUFFER * sizeof(char));
    if (fgets(gamebuff, BUFFER, readFile) == NULL) {
        
        // Quit if game doesn't exist.
        if (strcmp(fourthbuff, "- Game does not exist")) {
        perror("Error: Invalid Game-ID: This game does not exist.");
        free(fourthbuff);
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

        perror("Error receiving game name from the server.");
        free(gamebuff);
        close(socket_fd);
        exit(EXIT_FAILURE);
    } else {
        printf("Game name:\n %s\n", gamebuff);
    }
   
    //usleep(500000);

    // Send the PLAYER command to the server
    if(strstr(playersend, "0")){
        playersend= "PLAYER 0\n";
    }
    else if(strstr(playersend, "1")){
        playersend= "PLAYER 1\n";
    }
    else{
        playersend= "PLAYER \n";
    }
    //printf("%s\n", playersend);
    ssize_t sent_bytes = send(socket_fd, playersend, strlen(playersend),0);

    if (sent_bytes == -1)
    {
        perror("Playerkommando konnte nicht gesendet werden.");
        fprintf(stderr, "Error code: %d\n", errno);
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    else
        printf("%zd bytes sent(Player).\n", sent_bytes);

    // Add a delay
    usleep(500000);

    // Receive server response after sending PLAYER command.
 
    struct Player player = {};
    struct SharedData *shared = (struct SharedData *)shmat(shm_id, NULL, 0);

    char *playerbuffer = (char*)malloc(BUFFER * sizeof(char));

    if (fgets(playerbuffer, BUFFER, readFile) == NULL) {
        perror("No server response received from the server after client sent player command.");
        close(socket_fd);
        free(playerbuffer);
        exit(EXIT_FAILURE);
    }

    
    if(sscanf(playerbuffer, "+ YOU %d Player %d", &player.playerNum, &player.playerName) == 2) {
         printf("Player number: %d (Player name: %d)\n", player.playerNum, player.playerName);
    } else printf("%s\n", playerbuffer);

    usleep(50000);

    char *totalbuffer = (char*)malloc(BUFFER * sizeof(char));

    if (fgets(totalbuffer, BUFFER, readFile) == NULL) {
        perror("No server response received from the server after client sent player command.");
        close(socket_fd);
        free(totalbuffer);
        exit(EXIT_FAILURE);
    }

    if (sscanf(totalbuffer, "+ TOTAL %d", &shared->totalPlayers) == 1) {
        printf("\nTotal players: %d\n", shared->totalPlayers);

    } else printf("%s\n", totalbuffer);

    usleep(50000);

    char *opponentbuffer = (char*)malloc(BUFFER * sizeof(char));

    if (fgets(opponentbuffer, BUFFER, readFile) == NULL) {
        perror("No server response received from the server after client sent player command.");
        close(socket_fd);
        free(opponentbuffer);
        exit(EXIT_FAILURE);
    }

    if (sscanf(opponentbuffer, "+ %d Player %d %d", &player.playerNum, &player.playerName, &player.isReady) == 3) {
         if (player.isReady == 1) {
            printf("\nPlayer %d (Player name: %d) is ready.\n", player.playerNum, player.playerName);
            } else if (player.isReady == 0) {
                printf("\nPlayer %d (Player name: %d) is not ready.\n", player.playerNum, player.playerName);
            }

    } else printf("%s\n", opponentbuffer);


    usleep(50000);

    char *endbuffer = (char*)malloc(BUFFER * sizeof(char));

    if (fgets(endbuffer, BUFFER, readFile) == NULL) {
        perror("No server response received from the server after client sent player command.");
        close(socket_fd);
        free(endbuffer);
        exit(EXIT_FAILURE);
    }

    printf("\n%s\n", endbuffer);
    //recv(socket_fd, firstbuff, sizeof(firstbuff), 0);
    //printf("%s\n", firstbuff);
   
    /*char quickbuffer[50];
    struct Player player = {};
    struct SharedData *shared = (struct SharedData *)shmat(shm_id, NULL, 0);

    while (strlen(playerbuffer) > 0 && strlen(playerbuffer) < 100) playerbuffer[0] = '\0';

    {
        //snprintf(quickbuffer, sizeof(quickbuffer), "%.42s", playerbuffer); // store the needed information from the server response in quickbuffer

        char* line = strtok(quickbuffer, "\n");

        // Line-by-line parsing for formatting default server responses into readable language.

    while (line != NULL) {
        if (sscanf(line, "+ YOU %d Player %d", &player.playerNum, &player.playerName) == 2) {
            // Parse the first line
            printf("Player %d (Name: %d)\n", player.playerNum, player.playerName);
        } else if (sscanf(line, "+ TOTAL %d", &shared->totalPlayers) == 1) {
            // Parse the second line
            printf("Total players: %d\n", shared->totalPlayers);
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

    char *endbuff = (char *)malloc(BUFFER * sizeof(char));
    endbuff = recvLine(socket_fd, endbuff);
    if (strcmp(endbuff, "-1")) {
        endbuff[0] = '\0';
    printf("%s\n", endbuff);

    } else {
        perror("Error receiving server response.");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    */


        // printf("%s\n", quickbuffer); to test

        // printf("%d\n", player_received); to print the size of buffer being used
    

    free(firstbuff);
    free(secondbuff);
    free(thirdbuff);
    free(fourthbuff);
    free(gamebuff);
    free(playerbuffer);
    free(endbuffer);
    free(opponentbuffer);
    free(totalbuffer);
    



}  




