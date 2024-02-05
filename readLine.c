#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



char* readLine(char* buffer, FILE* readFile){
    memset(buffer, 0, strlen(buffer));

    char* buf= (char*) malloc(256*sizeof(char));
    memset(buf, 0, 256);

    while(1){
        if(fgets(buf, sizeof(buf), readFile) == (void *)0){
            free(buf);
            perror("Something went wrong when recving");
            return "\0";
        }

        strcat(buffer, buf);
        if(strstr(buf, "\n") != (void *)0){
            free(buf);
            return buffer;
        }
    }
}