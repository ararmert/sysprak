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
#include "thinker.h"

static struct SharedData *sharedData = NULL;
static struct Piece *pieces = NULL;


int charToInt(char y){
int result;
switch (y){
case 'A':
result= 1;
break;
case 'B':
result= 2;
break;
case 'C':
result = 3;
break;
case 'D':
result = 4;
break;
case 'E':
result = 5;
break;
case 'F':
result = 6;
break;
case 'G':
result = 7;
break;
case 'H':
result = 8;
break;

}
return result;
}
//
void signalHandler(){
    if(sharedData->shouldThink == true){
        sharedData->shouldThink = false;
        int i = 0;
        int blackCount = 0;
        int whiteCount = 0;
        int numberofOwnStones = 0;
        int numberOfOpponentStones = 0;
        while (i < 24){ // i < steinanzahl von yilila
            
            if(pieces[i].exist == false) {
                i++;
                continue;
            }
            if(pieces[i].color == black){
                blackCount++;
            }
            else if(pieces[i].color == white){
                whiteCount++;
            }
            i++;
        }

        if(sharedData->player.playerNum == 0){
            numberofOwnStones = whiteCount;
            numberOfOpponentStones = blackCount;
        }
        else{
            numberofOwnStones = blackCount;
            numberOfOpponentStones = whiteCount;
        }

        struct position *own =  malloc(sizeof(struct Piece) * numberofOwnStones);
        struct position *opponent = malloc(sizeof(struct Piece) * numberOfOpponentStones);

        i = 0;
        int self = 0;
        int opp = 0;
        while (i < 24){
            if(pieces[i].exist == false) {
                i++;
                continue;
            }
            if(sharedData->player.playerNum == 0 && pieces[i].color == white){
                struct position tmp = {.x= pieces[i].y , .y = charToInt(pieces[i].x), .status= pieces[i].status, .color= pieces[i].color};
                own[self] = tmp;
                self++;
            }
        }
        while (i < 24){
            if(pieces[i].exist == false) {
                i++;
                continue;
            }
            if(sharedData->player.playerNum == 0 && pieces[i].color == black){
                struct position tmp = {.x= pieces[i].y , .y = charToInt(pieces[i].x), .status= pieces[i].status, .color= pieces[i].color};
                opponent[opp] = tmp;
                opp++;
            }
        }
        while (i < 24){
            if(pieces[i].exist == false) {
                i++;
                continue;
            }
            if(sharedData->player.playerNum == 1 && pieces[i].color == black){
                struct position tmp = {.x= pieces[i].y , .y = charToInt(pieces[i].x), .status= pieces[i].status, .color= pieces[i].color};
                own[self] = tmp;
                self++;
            }
        }
        while (i < 24){
            if(pieces[i].exist == false) {
                i++;
                continue;
            }
            if(sharedData->player.playerNum == 1 && pieces[i].color == white){
                struct position tmp = {.x= pieces[i].y , .y = charToInt(pieces[i].x), .status= pieces[i].status, .color= pieces[i].color};
                opponent[opp] = tmp;
                opp++;
            }
            
        }
         
        //call rihannas functions with the required parameters
        
        move(canAnyStoneCapture(own, opponent, sizeof(opponent), sizeof(own)),own, opponent, sizeof(opponent), sizeof(own));

        free(own);
        free(opponent);

        printf("signalHandler.c ran successfully.\n");
       
            
        }

       

  

    }

