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


#define GAMEKINDNAME "Checkers"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define BUFFER 256


void handler(int s) {
    printf("Caught SIGPIPE\n");
}

// Prologphase der Kommunikation
void performConnection(int socket_fd, struct sockaddr* address) {

    if(connect(socket_fd, address, sizeof(*address)) != 0) {

        printf("Verbindung mit dem Server erfolgreich!\n");
       

        char response_buffer[256];
        ssize_t received_bytes = recv(socket_fd, response_buffer, sizeof(response_buffer)-1, 0);

        if (received_bytes == -1) {
            perror("Keine Antwort wurde vom Server ermittelt.\n");
            fprintf(stderr, "Error code: %d\n", errno);
            exit(EXIT_FAILURE);
        } else if (received_bytes == 0) {

            fprintf(stderr, "Server closed the connection. \n.");
            exit(EXIT_FAILURE);
        } else {

        response_buffer[received_bytes] = '\0';  

        printf("Server message: %s\n", response_buffer);
        }




         /* char playerKommando[] = "Player\n";
        ssize_t sent_bytes = send(socket_fd, playerKommando, strlen(playerKommando), 0);

        if (sent_bytes == -1) {
            if (errno == EPIPE) {

                fprintf(stderr, "Server closed the connection.\n");
            } else {
            perror("Playerkommando konnte nicht gesendet werden.\n");
            }
            fprintf(stderr, "Error code: %d\n", errno);
            exit(EXIT_FAILURE);
        } */




} else {
        perror("Error connecting to the server");
        fprintf(stderr, "Error code: %d\n", errno);
        exit(EXIT_FAILURE);

    } 

    close(socket_fd);

}

// Main function
int main(int argc, char**argv) {

    signal(SIGPIPE, handler);

    char *buffer = (char *)malloc(sizeof(char) * BUFFER);
    ssize_t size;
    // Create a TCP socket
    
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORTNUMBER);
    inet_pton(AF_INET, HOSTNAME, &(server_address.sin_addr));

    performConnection(socket_fd, (struct sockaddr*)&server_address);

    
    free(buffer);

    return 0;
}
