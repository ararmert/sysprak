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


char* recvLine(int socket_fd, char* buffer) {

    buffer[0]= '\0';

    
   char charbuff = 0;
      
    ssize_t tmp = -1; 

     while (true) {
        tmp = recv(socket_fd, &charbuff, 1, 0);

        if (tmp == -1) {
         perror("Error");
        } else if (tmp > 0) {
            strcat(buffer, &charbuff);
        }
        if (charbuff == '\n') {
            return buffer;
        } 
    }
}