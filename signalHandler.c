#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <shm.h> 
#include <shared_data.h>
#include <board.h>

static struct SharedData sharedData = NULL;
static struct YililiaSHMspielstand = NULL;

void signalHandler(){
    if(sharedData->shouldThink){
        int i = 0;
        int blackCount = 0;
        int whiteCount = 0;
        while i < (sizeof(spielstand)/sizeof(struct Piece)){
            if(spielstand[i].colour == black){
                blackCount++;
            }
            else if(spielstand[i].colour == white){
                whiteCount++;
            }
            i++;
        }

        if(Player->playerNum == 0){
            int numberofOwnStones = whiteCount;
            int numberOfOpponentStones = blackCount;
        }
        else{
            int numberofOwnStones = blackCount;
            int numberOfOpponentStones = whiteCount
        }

        struct Spielsteine own = (struct Spielsteine) malloc(sizeof(struct Piece)* numberofOwnStones);
        struct Spielsteine opponent = (struct Spielsteine) malloc(sizeof(struct Piece)* numberOfOpponentStones);

        int i = 0
        int self = 0
        int opp = 0
        while i < (sizeof(spielstand)/sizeof(struct Piece)){
            if(sharedData->spielernummer == 0 && spielstand[i].colour == white){
                own[self]= spielstand[i];
                self++;
            }
        }
        while i < (sizeof(spielstand)/sizeof(struct Piece)){
            if(sharedData->spielernummer == 0 && spielstand[i].colour == black){
                opponent[opponent]= spielstand[i];
                opp++;
            }
        }
        while i < (sizeof(spielstand)/sizeof(struct Piece)){
            if(sharedData->spielernummer == 1 && spielstand[i].colour == black){
                own[self]= spielstand[i];
                self++;
            }
        }
        while i < (sizeof(spielstand)/sizeof(struct Piece)){
            if(sharedData->spielernummer == 1 && spielstand[i].colour == white){
                opponent[opp]= spielstand[i];
                opp++;
            }
        }
        
            
        }

    //free(rihana)

    }

