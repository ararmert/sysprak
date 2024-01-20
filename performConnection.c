#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<errno.h>
#include<signal.h>
#include<netdb.h>
#include<fcntl.h>

               
#define BUFFER 256


void performConnection(int socket_fd,char gameID[13]) {

    // Receive the first server message
    char* charbuffer = (char*)malloc(BUFFER * sizeof(char));
    ssize_t size_received = recv(socket_fd, charbuffer, BUFFER - 1, 0);
    if (size_received > 0) charbuffer[size_received] = '\0';
    printf("%s\n", charbuffer);


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
    char clientVersion[] = "VERSION 3.3\n"; 
    ssize_t sent_byte = send(socket_fd, clientVersion, strlen(clientVersion), 0);

    if(sent_byte == -1) {
        perror("Error sending client version.\n");
        close(socket_fd);
        exit(EXIT_FAILURE);

        // Receive server response

    } else { 
        printf("%zd bytes sent(Client Version).\n", sent_byte);
        

    }  

    // Add a delay
    usleep(500000);

    // Receive server response
    char* charbufferr = (char*)malloc(BUFFER * sizeof(char));
    ssize_t size_receivedd = recv(socket_fd, charbufferr, BUFFER - 1, 0);
    if (size_receivedd > 0) charbufferr[size_receivedd] = '\0';
    printf("%s\n", charbufferr);

    // Send the Game-ID to the server
    char gameIDs[] = "ID 0sk8wc9exo1g1\n";
    ssize_t sent_gameid = send(socket_fd, gameIDs, strlen(gameIDs), 0);


    if(sent_gameid == -1) {
        perror("Error sending Game-ID.\n");
        close(socket_fd);
        exit(EXIT_FAILURE);
    } else { 
        printf("%zd bytes sent(Game-ID).\n", sent_byte);
    }

     // Add a delay
    usleep(500000);

    // Receive server response
    char* charbufferrr = (char*)malloc(BUFFER * sizeof(char));
    ssize_t size_receiveddd = recv(socket_fd, charbufferrr, BUFFER - 1, 0);
    if (size_receiveddd > 0) charbufferrr[size_receiveddd] = '\0';
    printf("%s\n", charbufferrr);

    
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


     // Receive server response
    char* playerbuffer = (char*)malloc(BUFFER * sizeof(char));
    ssize_t player_received = recv(socket_fd, playerbuffer, BUFFER - 1, 0);
    if (player_received > 0) playerbuffer[player_received] = '\0';
    printf("%s\n", playerbuffer);


    // Close the socket when done
    close(socket_fd);

}

