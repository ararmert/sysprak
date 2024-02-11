//#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
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

  


