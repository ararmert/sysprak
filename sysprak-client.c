#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#define GAMEKINDNAME Checkers
#define PORTNUMBER 1357
#define HOSTNAME sysprak.priv.lab.nm.ifi.lmu.de


void printHilfe(){

    printf("Please enter your 13-digit Game ID and Spielernummer.");

}

int main(int argc, char* argv[]){

    // Command Parameters as Game Id and Spielnummer
    int game_id[13] = {0};
    int spielnummer = 0;

    int ret;
    while((ret = getopt(argc,argv,"g:p:")) != -1){

        switch(ret){

            case 'g':
                printf("optarg is：%s\n",optarg);
                printf("*optarg is：%c\n",*optarg);

                
                break;
            case 'p':
                spielnummer = atoi(optarg);
                break;
            default:
                printHilfe();
                break;

        }
    }

    printf("Game Id is :%d\n",atoi(game_id));
    printf("Spielnummer is:%d\n",spielnummer);





    return 0;
}