#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <unistd.h>
#include <getopt.h>


int main(int argc, char* argv[]){

    int game_id =0;
    int spielnummer = 0;

    int ret;

    while((ret = getopt(argc,argv,"g:p:")) != -1){

        switch(ret){

            case 'g':
                game_id = atoi(optarg);
                printf("optarg是%d\n",atoi(optarg));
                break;
            case 'p':
                spielnummer = atoi(optarg);
                printf("optarg是%d\n",atoi(optarg));
                break;

        }
    }

    printf("Game Id is :%d\n",game_id);
    printf("Spielnummer is:%d\n",spielnummer);





    return 0;
}