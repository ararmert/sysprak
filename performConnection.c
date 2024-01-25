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
#include "shared_data.h" 

#define BUFFER 256

void performConnection(int socket_fd, char gameID[13], char playersend[9])
{

    // Receive the first server message '+MNM'
    char *charbuffer = (char *)malloc(BUFFER * sizeof(char));
    ssize_t size_received = recv(socket_fd, charbuffer, BUFFER - 1, 0);
    if (size_received > 0)
        charbuffer[size_received] = '\0';
    printf("%s\n", charbuffer);

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

    // Receive server response about Client Version
    char *charbufferr = (char *)malloc(BUFFER * sizeof(char));
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
    char gameIDs[18] = "ID ";
    strcat(gameIDs, gameID);
    strcat(gameIDs, "\n");
    printf("game id is %s\n", gameIDs);
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
    // Add a delay
    usleep(500000);

    // Receive server response about Gamekind-Name
    char *charbufferrr = (char *)malloc(BUFFER * sizeof(char));
    ssize_t size_receiveddd = recv(socket_fd, charbufferrr, BUFFER - 1, 0);
    if (size_receiveddd > 0) {
        charbufferrr[size_receiveddd] = '\0';
    printf("%s\n", charbufferrr);
    } else {
        perror("Error receiving server response about the game kind.");
        close(socket_fd);
        exit(EXIT_FAILURE);

    }

    // Quit if Gamekind is not 'Checkers'
    char gkindName[20];
    strncpy(gkindName, &charbufferrr[10], 8);
    gkindName[8] = '\0';

    char strcheck[] = "Checkers";

    int compare = strcmp(gkindName, strcheck);
    if (compare != 0)
    {
        perror("Wrong GameKindName received from the server.\n");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // save Gamename
    char gName[102];
    strncpy(gName, &charbufferrr[21], 100);
    gName[101] = '\0';

    printf("game Name from Webinterface is %s\n",gName);
    
    // Send the PLAYER command to the server
    ssize_t sent_bytes = send(socket_fd, playersend, strlen(playersend), 0);

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

    struct Player player = {.totalPlayerNum = 2};

    // Receive server response after sending PLAYER command.
    
      char quickbuffer[50];
    char* playerbuffer = (char*)malloc(BUFFER * sizeof(char));
    ssize_t player_received = recv(socket_fd, playerbuffer, BUFFER - 1, 0);

    while (player_received > 0 && player_received < 100) playerbuffer[player_received] = '\0';
    {
        snprintf(quickbuffer, sizeof(quickbuffer), "%.42s", playerbuffer); // store the needed information from the server response in quickbuffer

        char* line = strtok(quickbuffer, "\n");

        // Line-by-line parsing for formatting default server responses into readable language.

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
    

    free(charbuffer);
    free(charbufferr);
    free(charbufferrr);
    free(playerbuffer);
    // free(quickbuffer) if it's not used in the main function later on



}  







