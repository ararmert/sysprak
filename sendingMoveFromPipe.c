#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "shared_data.h"

#define BUFFER_SIZE 256

//void SendingMoveFromPipe(int pipe_read_fd, int server_socket_fd)
void SendingMoveFromPipe(int pipe_read_fd, int server_socket_fd, FILE* read_File){
    char buffer[BUFFER_SIZE];
    char* player = "PLAY ";

        // Lesen der Pip
       // ssize_t bytes_read = read(pipe_read_fd, buffer, BUFFER_SIZE);
       /* if (bytes_read <= 0) {
            printf("reading from pipe failed! /n");  

        }*/
        FILE* pipeFile= fopen(pipe_read_fd, "r");
        fgets(buffer, BUFFER_SIZE, pipeFile);
        strcat(player , buffer);
         if(fgets(buffer, BUFFER_SIZE, read_File)== NULL){
          printf("reading from pipe failed\n");
        }
        //schreiben der pipe in Server
          ssize_t bytes_written = send(server_socket_fd, player, strlen(player),0);
        //ssize_t bytes_written = write(server_socket_fd, buffer, bytes_read);
        if (bytes_written <= 0) {
        
        printf("sending from pipe failed! /n");
        }
        //buffer = recvLine(server_socket_fd, buffer);
       memset(buffer, 0, strlen(buffer));
       if(fgets(buffer, BUFFER_SIZE, read_File)== NULL){
          printf("recving OKMOVE from server\n");
        }
       // next 2 lines just temporar added for testing
       // char* recvLine(int server_socket_fd, char* buffer);
         // return buffer;
          
        if (!strcmp(buffer, "+ MOVEOK"))
        {
           printf("incorrect Move was sent! /n");
        }
        
     

       //close(server_socket_fd);
}

  


