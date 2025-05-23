#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>

FILE* openFile(char* name){
    char fileName[256];
    FILE* file = NULL;

    if(name == NULL){
        strcpy(fileName,"client.conf\0");
    }else{
        strncpy(fileName, name, 255*sizeof(char));
    }

    if((file = fopen(fileName,"r")) == NULL){
        perror("Error by open the file:");
    }
return file;
}

struct config{
    char Hostname[256];
    int PortNumer;
    char GameKindName[256];
};


int config(char* fileName,struct config* con){
    FILE* file = openFile(fileName);
    if(file == NULL) {
        perror("Open file failed. Error");
        return -1;
    }
    
    char* p = (char*)malloc(sizeof(char)*256); 
    char temp1[256];
    char temp2[256];
    
    while(!feof(file)){
        memset(p,0,256);
        fgets(p,256,file);
        sscanf(p,"%s = %s",temp1,temp2);
      
        if(!strcmp(temp1,"Hostname")){
            strcpy(con->Hostname,temp2);
        }else if(!strcmp(temp1,"GameKindName")){
            strcpy(con->GameKindName,temp2);
        }else if(!strcmp(temp1,"PortNumer")){
            con->PortNumer = atoi(temp2);
        }else{
            printf("wrong parameter\n");
        }
       
    }

    free(p);
    fclose(file);   
    return 0;
}

