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


void handler(int s) {
    printf("Caught SIGPIPE\n");
}
