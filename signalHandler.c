#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
//#include <shm.h> 
#include "shared_data.h"
#include "board.h"

static struct SharedData *sharedData = NULL;
static struct Piece *pieces = NULL;

void signalHandler(){
    if(sharedData->shouldThink){
        int i = 0;
        int blackCount = 0;
        int whiteCount = 0;
        int numberofOwnStones;
        int numberOfOpponentStones;
        while (i < (sizeof(pieces)/sizeof(struct Piece))){
            if(pieces[i].color == black){
                blackCount++;
            }
            else if(pieces[i].color == white){
                whiteCount++;
            }
            i++;
        }

        if(sharedData->player.playerNum == 0){
            int numberofOwnStones = whiteCount;
            int numberOfOpponentStones = blackCount;
        }
        else{
            int numberofOwnStones = blackCount;
            int numberOfOpponentStones = whiteCount;
        }

        struct Piece *own =  malloc(sizeof(struct Piece) * numberofOwnStones);
        struct Piece *opponent = malloc(sizeof(struct Piece) * numberOfOpponentStones);

        int i = 0;
        int self = 0;
        int opp = 0;
        while (i < (sizeof(pieces)/sizeof(struct Piece))){
            if(sharedData->player.playerNum == 0 && pieces[i].color == white){
                own[self] = pieces[i];
                self++;
            }
        }
        while (i < (sizeof(pieces)/sizeof(struct Piece))){
            if(sharedData->player.playerNum == 0 && pieces[i].color == black){
                opponent[opp] = pieces[i];
                opp++;
            }
        }
        while (i < (sizeof(pieces)/sizeof(struct Piece))){
            if(sharedData->player.playerNum == 1 && pieces[i].color == black){
                own[self] = pieces[i];
                self++;
            }
        }
        while (i < (sizeof(pieces)/sizeof(struct Piece))){
            if(sharedData->player.playerNum == 1 && pieces[i].color == white){
                opponent[opp] = pieces[i];
                opp++;
            }
            
        }
        
            
        }

    //free(rihana)

    }

