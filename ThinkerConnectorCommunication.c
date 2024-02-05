#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "shared_data.h"

#define BUFFER_SIZE 256

void SendingMoveFromPipe(int pipe_read_fd, int server_socket_fd){
    char buffer[BUFFER_SIZE];
    char* player = "PLAY ";

        // Lesen der Pip
        ssize_t bytes_read = fgets(pipe_read_fd, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            printf("reading from pipe failed! /n");  

        }
        strcat(player , buffer);
        //schreiben der pipe in Server
          ssize_t bytes_written = send(server_socket_fd, player, strlen(player),0);
        //ssize_t bytes_written = write(server_socket_fd, buffer, bytes_read);
        if (bytes_written <= 0) {
        
        printf("sending from pipe failed! /n");
        }
        buffer = recvLine(server_socket_fd, buffer);
        if (!strcmp(buffer, "+ MOVEOK"))
        {
           printf("incorrect Move was sent! /n");
        }
        
     

       close(server_socket_fd);
}

  


/* 
int main() {
    // Annahme: Hier wird die Socket-Verbindung zum Server erstellt (server_socket_fd)
    // Beachte, dass dies nur ein Beispiel ist, und du die Verbindung entsprechend deinem Code erstellen musst.

    int pipe_fds[2];

    // Erstelle die unbenannte Pipe
    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Code für den Thinker-Prozess
        close(pipe_fds[0]); // Schließe Leseseite der Pipe im Thinker-Prozess
        think(pipe_fds[1]);
        exit(EXIT_SUCCESS);

    } else {
        // Code für den Connector-Prozess
        close(pipe_fds[1]); // Schließe Schreibseite der Pipe im Connector-Prozess

        // Hier solltest du den Code für die Verbindung zum Gameserver haben (z.B., Socket erstellen und verbinden)
        int server_socket_fd = /* ... */;

        // Starte die sending-Methode im Connector-Prozess
       // sending(pipe_fds[0], server_socket_fd);

        // Warte auf den Thinker-Prozess, um Zombie-Prozesse zu vermeiden
       // wait(NULL);
  //  }

  //  return 0;
//}*/