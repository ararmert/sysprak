#include <stdbool.h>
#include "board.h"
#include "thinker.h"


/*struct position {
    int x;
    int y; 
    enum color color;
    enum Status status;

    bool status; // zum Überprüfen ob der stein ein damestein ist!
    //int* color;

};*/
/*enum Color{
    black, 
    white
};
enum Status{
    SOLDIER,
    QUEEN
};*/

#define BOARD_SIZE 8

int** spielfeld;
int* LEERES_FELD;

void split(struct position*, struct position** opponent, struct position** own);


bool isFieldFree(int y, int x, struct position* opponent, struct position* spielerPosition, int sizeopponent, int sizespielerPosition){
   // int size = sizeof(opponent)/ sizeof(struct position);
for (int i = 0; i < sizeopponent; i++){

   if ( y == opponent[i].y && x == opponent[i].x)
   {
    return false;
   }
}
    //size= sizeof(spielerPosition)/ sizeof(struct position);
for (int i = 0; i < sizespielerPosition; i++)
{
    if (y == spielerPosition[i].y && x == spielerPosition[i].x)
    {
       return false;
    }
    
    }
    return true;
}

    
// Methode3.1 prüft ob einer stein einen anderen schlagen kann
bool canStoneCapture(struct position currentPlayer, struct position* opponent, struct position* spielerPosition, int sizeopponent, int sizespielerPosition ) {
    // zielfeld aufm spielbrett?
    //int size = sizeof(opponent)/ sizeof(struct position);

    // für dunkle Spieler
    for(int i=0; i< sizeopponent; i++){

    //if (opponent[i].x < 0 || opponent[i].x >= BOARD_SIZE || opponent[i].y < 0 || opponent[i].y >= BOARD_SIZE) {
    //    return false;
    //}
        
    if(currentPlayer.color == black && currentPlayer.x-1 == opponent[i].x){
        if(currentPlayer.y -1 == opponent[i].y){
            if(isFieldFree(currentPlayer.y-2, currentPlayer.x-2, opponent , spielerPosition,  sizeopponent, sizespielerPosition )){
                return true;  

            }
        }
        if(currentPlayer.y +1 == opponent[i].y){
            if(isFieldFree(currentPlayer.y+2, currentPlayer.x-2, opponent , spielerPosition , sizeopponent,  sizespielerPosition)){
                return true;

            }  
        }
    } 

    if(currentPlayer.color == white && currentPlayer.x+1 == opponent[i].x){
        if(currentPlayer.y -1 == opponent[i].y){
            if(isFieldFree(currentPlayer.y-2, currentPlayer.x+2, opponent , spielerPosition,  sizeopponent,  sizespielerPosition )){

            i--;
              return true;
            }
          
        }
        if(currentPlayer.y +1 == opponent[i].y){
            if(isFieldFree(currentPlayer.y+2, currentPlayer.x+2, opponent , spielerPosition,  sizeopponent,  sizespielerPosition )){

            i--;
              return true;
            } 
            
        }
    }
    }

// für helle Spieler
    /*for (int j = 0; j < size; j++){

        if (opponent[j].x < 0 || opponent[j].x >= BOARD_SIZE || opponent[j].y < 0 || opponent[j].y >= BOARD_SIZE) {
        return false;
    }
        
    if(currentPlayer.color == white && currentPlayer.x-1 == opponent[j].x){
        if(currentPlayer.y -1 == opponent[j].y){
            if(isFieldFree(currentPlayer.y-2, currentPlayer.x-2, opponent , spielerPosition )){


           j--;     

            }
        }
        if(currentPlayer.y +1 == opponent[j].y){
            if(isFieldFree(currentPlayer.y+2, currentPlayer.x-2, opponent , spielerPosition )){

            j--;
            }  
        }
    } 

    if(currentPlayer.color == white && currentPlayer.x+1 == opponent[j].x){
        if(currentPlayer.y -1 == opponent[j].y){
            if(isFieldFree(currentPlayer.y-2, currentPlayer.x+2, opponent , spielerPosition )){

            j--;
            }
        }
        if(currentPlayer.y +1 == opponent[j].y){
            if(isFieldFree(currentPlayer.y+2, currentPlayer.x+2, opponent , spielerPosition )){

            j--;
            } 
        }
    }
    }*/
                     
    
    return false;
  }

   
// Methode 3.2 && 3.3
//einer der Steine einen Gegner schlagen kann?


int obenRechtsHelper(struct position currentPlayer, struct position* opponent, struct position* spielerPosition, int sizeopponent, int sizespielerPosition){
   // int size = sizeof(opponent)/ sizeof(struct position);
    for(int i=0; i< sizeopponent; i++){
        if(currentPlayer.y-i >0 || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        } 
        if(currentPlayer.y+ i == opponent[i].y && currentPlayer.x+ i == opponent[i].x){
            if(isFieldFree(currentPlayer.y+i+1, currentPlayer.x+i+1, opponent , spielerPosition, sizeopponent, sizespielerPosition ))
                return 1;
            return 0;
        }
        // checkt ob auf nächstem Feld ein Freund liegt
        if(!isFieldFree(currentPlayer.y+ i, currentPlayer.x+i, opponent , spielerPosition, sizeopponent, sizespielerPosition)){
            return 0;
        }
    }
    return -1;
}
int obenLinksHelper(struct position currentPlayer, struct position* opponent, struct position* spielerPosition, int sizeopponent, int sizespielerPosition){
   // int size = sizeof(opponent)/ sizeof(struct position);
    for(int i=0; i< sizeopponent; i++){
        if(currentPlayer.y-i >0 || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        } 
        if(currentPlayer.y- i == opponent[i].y && currentPlayer.x+ i == opponent[i].x){
            if(isFieldFree(currentPlayer.y-i-1, currentPlayer.x+i+1, opponent , spielerPosition, sizeopponent, sizespielerPosition ))
                return 1;
            return 0;
        }
        // checkt ob auf nächstem Feld ein Freund liegt
        if(!isFieldFree(currentPlayer.y- i, currentPlayer.x+i, opponent , spielerPosition, sizeopponent, sizespielerPosition)){
            return 0;
        }
    }
    return -1;
}

int untenRechtsHelper(struct position currentPlayer, struct position* opponent, struct position* spielerPosition, int sizeopponent, int sizespielerPosition){
    //int size = sizeof(opponent)/ sizeof(struct position);
    for(int i=0; i< sizeopponent; i++){
        if(currentPlayer.y-i <0 || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        } 
        if(currentPlayer.y+ i == opponent[i].y && currentPlayer.x- i == opponent[i].x){
            if(isFieldFree(currentPlayer.y+i+1, currentPlayer.x-i-1, opponent , spielerPosition, sizeopponent, sizespielerPosition ))
                return 1;
            return 0;
        }
        // checkt ob auf nächstem Feld ein Freund liegt
        if(!isFieldFree(currentPlayer.y+ i, currentPlayer.x-i, opponent , spielerPosition, sizeopponent, sizespielerPosition)){
            return 0;
        }
    }
    return -1;
}

int untenLinkssHelper(struct position currentPlayer, struct position* opponent, struct position* spielerPosition, int sizeopponent,int sizespielerPosition){
    //int size = sizeof(opponent)/ sizeof(struct position);
     
    for(int i=0; i< sizeopponent; i++){
        if(currentPlayer.y-i <0 || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        }  
        if(currentPlayer.y- i == opponent[i].y && currentPlayer.x- i == opponent[i].x){
            if(isFieldFree(currentPlayer.y-i-1, currentPlayer.x-i-1, opponent , spielerPosition, sizeopponent, sizespielerPosition ))
                return 1;
            return 0;
        }
        // checkt ob auf nächstem Feld ein Freund liegt
        if(!isFieldFree(currentPlayer.y- i, currentPlayer.x-i, opponent , spielerPosition, sizeopponent, sizespielerPosition)){
            return 0;
        }
    }
    return -1;
}

// Methode 3.3 && 3.4
// Def:ob ein stein Dame ist bzw. ein damenstein schlagen kann! -> wenn die Zeilen- und Spaltenunterschiede gleich sind
bool canKingCapture(struct position currentPlayer, struct position* opponent, struct position* spielerPosition, int sizeopponent, int sizespielerPosition, struct position tmpPosition) {
    int size = sizeof(opponent)/ sizeof(struct position);
    int i= 0;
    
    while(true){
        if(currentPlayer.y+ i >= BOARD_SIZE || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        } 
        struct position tmpPosition= {.x= currentPlayer.x+i, .y= currentPlayer.y+i, .status= currentPlayer.status, .color= currentPlayer.color}; 
        int tmp= obenRechtsHelper(currentPlayer, opponent, spielerPosition, sizeopponent, sizespielerPosition);
        if(tmp == 1)
            return true;
        else if(tmp == 0);
            break;
        i++;
    }
    i = 0;
    while(true){
        if(currentPlayer.y- i >= BOARD_SIZE || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        }  
         struct position tmpPosition= {.x= currentPlayer.x+i, .y= currentPlayer.y-i, .status= currentPlayer.status, .color= currentPlayer.color}; 
        int tmp= obenlinksHelper(currentPlayer, opponent, spielerPosition, sizeopponent, sizespielerPosition);
        if(tmp == 1)
            return true;
        else if(tmp == 0);
            break;
        i++;
    }
    i = 0;

    while(true){
        if(currentPlayer.y+ i >= BOARD_SIZE || currentPlayer.x- i >= BOARD_SIZE){
            break;
        } 
         struct position tmpPosition= {.x= currentPlayer.x-i, .y= currentPlayer.y+i, .status= currentPlayer.status, .color= currentPlayer.color};  
        int tmp= untenRechtsHelper(currentPlayer, opponent, spielerPosition, sizeopponent, sizespielerPosition);
        if(tmp == 1)
            return true;
        else if(tmp == 0);
            break;
        i++;
    }
    i = 0;

    while(true){
        if(currentPlayer.y- i >= BOARD_SIZE || currentPlayer.x- i >= BOARD_SIZE){
            break;
        } 
         struct position tmpPosition= {.x= currentPlayer.x-i, .y= currentPlayer.y-i, .status= currentPlayer.status, .color= currentPlayer.color};  
        int tmp= untenLinkssHelper(currentPlayer, opponent, spielerPosition, sizeopponent, sizespielerPosition);
        if(tmp == 1)
            return true;
        else if(tmp == 0);
            break;
        i++;
    }
    i = 0;

    // für alle 4 Richtungen schreiben -> Done!

    return false;


  }

  struct position canAnyStoneCapture(struct position* spielerPosition, struct position* opponnent, int sizeopponent, int sizespielerPosition) {
    //int eigeneSteine = sizeof(spielerPosition)/ sizeof(struct position);
    for (int i = 0; i < sizespielerPosition; ++i) {
        if (spielerPosition[i].status == QUEEN && canKingCapture(spielerPosition[i], opponnent, spielerPosition, sizeopponent, sizespielerPosition, tmpPosition)){
            return spielerPosition[i];
            
        }
        else if(canStoneCapture(spielerPosition[i], opponnent, spielerPosition ,sizeopponent, sizespielerPosition )) {
            return spielerPosition[i];
        }
    }
    struct position tmp= {.x =0 , .y =0, .status=0};
    return tmp;
}

    /*int xDiff = abs(currentPlayer.x - opponent.x);
    int colDiff = abs(currentPlayer.y - opponent.y);
    return xDiff == colDiff;*/

bool (*whatForStone(bool Dame, int playercolor))(struct position, struct position, int) {
    if (canKingCapture) {
        return &canAnyStoneCapture; // die frage ist soll ich hier die die Adresse der Funktion zurückgeben lassen oder die Funktion selbst?
    } else {
        return &canStoneCapture; 
    }
}
/*if(canKingCapture)
{
    return canAnyStoneCapture; //die frage ist soll ich hier die die Adresse der Funktion zurückgeben lassen oder die Funktion selbst?
}*/

/*bool whatForStone(istKeineDame){
    if (istKeineDame == 1)
    {
       return canStoneCapture; 
    }else
    {
        return canAnyStoneCapture;
    }
    
    
}*/

// return zu strings stat true
char* move(struct position currentPlayer, struct position* spielerPosition, struct position* opponent, int sizeopponent, int sizespielerPosition){
    int result [256]= '/0';
    if(currentPlayer.y == 0 && currentPlayer.x == 0){
        //int eigeneSteine = sizeof(spielerPosition)/ sizeof(struct position);

        for (int i = 0; i < sizespielerPosition; ++i) {
            if(!spielerPosition[i].status == QUEEN){
                if(spielerPosition[i].color == black){
                    if(isFieldFree(spielerPosition[i].y +1, spielerPosition[i].x -1, opponent , spielerPosition, sizeopponent, sizespielerPosition )){
                        struct position tmp= {.x = currentPlayer.x-1, .y =currentPlayer.y+1, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp), recusviBlack);// mit strcat : an ersten String
                    }
                    if(isFieldFree(spielerPosition[i].y -1, spielerPosition[i].x -1, opponent , spielerPosition, sizeopponent,  sizespielerPosition )){
                        struct position tmp= {.x = currentPlayer.x-1, .y =currentPlayer.y-1, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp), recusviBlack);
                    }
                }

                //selbe für white


                if(spielerPosition[i].color == white){
                    if(isFieldFree(spielerPosition[i].y +1, spielerPosition[i].x +1, opponent , spielerPosition , sizeopponent,  sizespielerPosition)){
                        struct position tmp= {.x = currentPlayer.x+1, .y =currentPlayer.y+1, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp), recusviBlack); // mit strcat : an ersten String
                    }
                    if(isFieldFree(spielerPosition[i].y -1, spielerPosition[i].x +1, opponent , spielerPosition, sizeopponent,  sizespielerPosition )){
                        struct position tmp= {.x = currentPlayer.x+1, .y =currentPlayer.y-1, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp), recusviBlack);
                    }
                }
            }else{
                if(isFieldFree(currentPlayer.y+1, currentPlayer.x+1, opponent , spielerPosition, sizeopponent,  sizespielerPosition )){
                    struct position tmp= {.x = currentPlayer.x+1, .y =currentPlayer.y+1, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp), recusviBlack);
                }

                if(isFieldFree(currentPlayer.y-1, currentPlayer.x+1, opponent , spielerPosition, sizeopponent,  sizespielerPosition )){
                    struct position tmp= {.x = currentPlayer.x+1, .y =currentPlayer.y-1, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp)),recusviBlack;
                }

                if(isFieldFree(currentPlayer.y+1, currentPlayer.x-1, opponent , spielerPosition , sizeopponent,  sizespielerPosition)){
                    struct position tmp= {.x = currentPlayer.x-1, .y =currentPlayer.y+1, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp),recusviBlack);
                }

                if(isFieldFree(currentPlayer.y-1, currentPlayer.x-1, opponent , spielerPosition , sizeopponent,  sizespielerPosition)){
                    struct position tmp= {.x = currentPlayer.x-1, .y =currentPlayer.y-1, .status=currentPlayer.status, .color= currentPlayer.color};
                         return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp), recusviBlack);
                }

                // selbe 4 für Richtungen
            }
        }
    }
    else{
        //result = positionToString(currentPlayer);
        if(!currentPlayer.status){
            //int size = sizeof(opponent)/ sizeof(struct position);
            for(int i=0; i< sizeopponent;++i){
                if(currentPlayer.color == black && currentPlayer.x-1 == opponent[i].x){
                    if(currentPlayer.y -1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y-2, currentPlayer.x-2, opponent , spielerPosition, sizeopponent, sizespielerPosition )){
                            struct position tmp= {.x = currentPlayer.x-2, .y =currentPlayer.y-2, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp));

                        }
                    }
                    if(currentPlayer.y +1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y+2, currentPlayer.x-2, opponent , spielerPosition, sizeopponent, sizespielerPosition )){
                             struct position tmp= {.x = currentPlayer.x-2, .y =currentPlayer.y+2, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp));
                        }  
                    }
                }
            }

            // selbe für Weiß

            for(int i=0; i< sizeopponent;++i){
                if(currentPlayer.color == white && currentPlayer.x+1 == opponent[i].x){
                    if(currentPlayer.y -1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y-2, currentPlayer.x+2, opponent , spielerPosition,sizeopponent, sizespielerPosition )){
                             struct position tmp= {.x = currentPlayer.x+2, .y =currentPlayer.y-2, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp));

                        }
                    }
                    if(currentPlayer.y +1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y+2, currentPlayer.x+2, opponent , spielerPosition, sizeopponent, sizespielerPosition )){
                             struct position tmp= {.x = currentPlayer.x+2, .y =currentPlayer.y+2, .status=currentPlayer.status, .color= currentPlayer.color};
                        return strcat(strcat(postionToString(currentPlayer),":"), positionToString(tmp));
                        }  
                    }
                } 
            }

        }else{
            int i= 0;
    while(true){
        if(currentPlayer.y+ i >= BOARD_SIZE || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        }
        struct position tmpPosition= {.x= currentPlayer.x+i, .y= currentPlayer.y+i, .status= currentPlayer.status, .color= currentPlayer.color};
        int tmp= obenRechtsHelper(tmpPosition, opponent, spielerPosition, sizeopponent, sizespielerPosition);
        if(tmp == 1)
            return strcat(strcat(strcat(postionToString(currentPlayer)),":"), recursivQueen());
        else if(tmp == 0)
            break;
        i++;
    }

    i=0;

     while(true){
        if(currentPlayer.y+ i >= BOARD_SIZE || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        }
        struct position tmpPosition= {.x= currentPlayer.x+i, .y= currentPlayer.y-i, .status= currentPlayer.status, .color= currentPlayer.color};
        int tmp= obenLinksHelper(tmpPosition, opponent, spielerPosition, sizeopponent, sizespielerPosition);
        if(tmp == 1)
            return strcat(strcat(strcat(postionToString(currentPlayer)),":"), recursivQueen());
        else if(tmp == 0)
            break;
        i++;
    }

    i=0;

     while(true){
        if(currentPlayer.y+ i >= BOARD_SIZE || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        }
        struct position tmpPosition= {.x= currentPlayer.x-i, .y= currentPlayer.y+i, .status= currentPlayer.status, .color= currentPlayer.color};
        int tmp= untenRechtsHelper(tmpPosition, opponent, spielerPosition, sizeopponent, sizespielerPosition);
        if(tmp == 1)
            return strcat(strcat(strcat(postionToString(currentPlayer)),":"), recursivQueen());
        else if(tmp == 0)
            break;
        i++;
    }

    i=0;

     while(true){
        if(currentPlayer.y+ i >= BOARD_SIZE || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        }
        struct position tmpPosition= {.x= currentPlayer.x-i, .y= currentPlayer.y-i, .status= currentPlayer.status, .color= currentPlayer.color};
        int tmp= untenLinkssHelper(tmpPosition, opponent, spielerPosition, sizeopponent, sizespielerPosition);
        if(tmp == 1)
            return strcat(strcat(strcat(postionToString(currentPlayer)),":"), recursivQueen());
        else if(tmp == 0)
            break;
        i++;
    }

    i=0;   


        }
    }
}

char* positionToString(struct position currentPlayer){
    int y= currentPlayer.y;
    char result [2]= '/0';
    switch (y){
        case 1:
            result[0]= 'A';
            break;
        case 2:
            result[0]= 'B';
            break;
        case 3:
            result[0] = 'C';
            break;
        case 4:
            result[0] = 'D';
            break;
        case 5:
            result[0] = 'E';
            break;
        case 6:
            result[0] = 'F';
            break;
        case 7:
            result[0] = 'G';
            break;
        case 8:
            result[0] = 'H';
            break;
       /* case 3:
            result[0] = '';
            break;*/                     
    }
    result[1]= currentPlayer.x;

    return result;
}

int* recusviBlack(struct position currentPlayer, struct position* spielerPosition, struct position* opponent, int sizeopponent, int sizespielerPosition){
                //int size = sizeof(opponent)/ sizeof(struct position);
            for(int i=0; i< sizeopponent;++i){
                if(currentPlayer.color == black && currentPlayer.x-1 == opponent[i].x){
                    if(currentPlayer.y -1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y-2, currentPlayer.x-2, opponent , spielerPosition,  sizeopponent,  sizespielerPosition )){
                            struct position tmp= {.x = currentPlayer.x-2, .y =currentPlayer.y-2, .status=currentPlayer.status, .color= currentPlayer.color};
                            return strcat(strcat(postionToString(currentPlayer), ":"), recusviBlack(tmp, spielerPosition, opponent,  sizeopponent,  sizespielerPosition));
                        }
                    }
                    if(currentPlayer.y +1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y+2, currentPlayer.x-2, opponent , spielerPosition,  sizeopponent, sizespielerPosition )){
                            struct position tmp= {.x = currentPlayer.x-2, .y =currentPlayer.y+2, .status=currentPlayer.status, .color= currentPlayer.color};
                            return strcat(strcat(postionToString(currentPlayer), ":"), recusviBlack(tmp, spielerPosition, opponent,  sizeopponent,  sizespielerPosition));
                        }  
                    }
                }

                if(currentPlayer.color == white && currentPlayer.x+1 == opponent[i].x){
                    if(currentPlayer.y -1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y-2, currentPlayer.x+2, opponent , spielerPosition,  sizeopponent,  sizespielerPosition )){
                            struct position tmp= {.x = currentPlayer.x-2, .y =currentPlayer.y+2, .status=currentPlayer.status, .color= currentPlayer.color};
                            return strcat(strcat(postionToString(currentPlayer), ":"), recusviBlack(tmp, spielerPosition, opponent,  sizeopponent, sizespielerPosition));
                        }
                    }
                    if(currentPlayer.y +1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y+2, currentPlayer.x+2, opponent , spielerPosition,  sizeopponent,  sizespielerPosition)){
                            struct position tmp= {.x = currentPlayer.x-2, .y =currentPlayer.y-2, .status=currentPlayer.status, .color= currentPlayer.color};
                            return strcat(strcat(postionToString(currentPlayer), ":"), recusviBlack(tmp, spielerPosition, opponent,  sizeopponent, sizespielerPosition));
                        }  
                    }
                }  
            }
            return "\n" ; //testen
}

char* recusviQueen(struct position currentPlayer, struct position* spielerPositionen, struct position* opponent, int sizeOpponent, int sizespielerPosition){
                //int size = sizeof(opponent)/ sizeof(struct position);
        int j=0;
        while(true){
            for(int i=0; i< sizeOpponent;++i){
                if(currentPlayer.y+ j >= BOARD_SIZE || currentPlayer.x+ j >= BOARD_SIZE){
                    break;
                }
                int tmp= obenRechtsHelper(currentPlayer, opponent, spielerPositionen, sizeOpponent, sizespielerPosition);
                if(tmp == 1){
                    struct position tmpPosition= {.x= currentPlayer.x+j, .y=currentPlayer.y+j, .status= currentPlayer.status, .color= currentPlayer.color};
                    return strcat(strcat(positionToString, ":"), recusviQueen(tmpPosition, spielerPositionen, opponent, sizeOpponent, sizespielerPosition));
                }
                else if(tmp == 0)
                    break;
            }
            j++;
        }
         j=0; 
         
          while(true){
            for(int i=0; i< sizeOpponent;++i){
                if(currentPlayer.y+ j >= BOARD_SIZE || currentPlayer.x+ j >= BOARD_SIZE){
                    break;
                }
                int tmp= obenLinksHelper(currentPlayer, opponent, spielerPositionen, sizeOpponent, sizespielerPosition);
                if(tmp == 1){
                    struct position tmpPosition= {.x= currentPlayer.x+j, .y=currentPlayer.y-j, .status= currentPlayer.status, .color= currentPlayer.color};
                    return strcat(strcat(positionToString, ":"), recusviQueen(tmpPosition, spielerPositionen, opponent, sizeOpponent, sizespielerPosition));
                }
                else if(tmp == 0)
                    break;
            }
            j++;
        }
         j=0; 
         
          while(true){
            for(int i=0; i< sizeOpponent;++i){
                if(currentPlayer.y+ j >= BOARD_SIZE || currentPlayer.x+ j >= BOARD_SIZE){
                    break;
                }
                int tmp= untenRechtsHelper(currentPlayer, opponent, spielerPositionen, sizeOpponent, sizespielerPosition);
                if(tmp == 1){
                    struct position tmpPosition= {.x= currentPlayer.x-j, .y=currentPlayer.y+j, .status= currentPlayer.status, .color= currentPlayer.color};
                    return strcat(strcat(positionToString, ":"), recusviQueen(tmpPosition, spielerPositionen, opponent, sizeOpponent, sizespielerPosition));
                }
                else if(tmp == 0)
                    break;
            }
            j++;
        }
         j=0; 
         
          while(true){
            for(int i=0; i< sizeOpponent;++i){
                if(currentPlayer.y+ j >= BOARD_SIZE || currentPlayer.x+ j >= BOARD_SIZE){
                    break;
                }
                int tmp= untenLinkssHelper(currentPlayer, opponent, spielerPositionen, sizeOpponent, sizespielerPosition);
                if(tmp == 1){
                    struct position tmpPosition= {.x= currentPlayer.x-j, .y=currentPlayer.y-j, .status= currentPlayer.status, .color= currentPlayer.color};
                    return strcat(strcat(positionToString, ":"), recusviQueen(tmpPosition, spielerPositionen, opponent, sizeOpponent, sizespielerPosition));
                }
                else if(tmp == 0)
                    break;
            }
            j++;
        }
         j=0; 
         
           return "\n" ; //testen
} 

     
        



       /* if ((currentPlayer.color == black && currentPlayer.x - 1 == opponent[i].x ) || ( currentPlayer.color == white && currentPlayer.x + 1 == opponent.x)) {
            
            if (currentPlayer.y - 1 == opponent.y || currentPlayer.y + 1 == opponent.y) {
                return true;
            }
        }*/


/*..................................*/

/*#include <stdbool.h>

// position des Spielsteins
struct position {
    int x;
    int y;
};

// Spielfeldgröße
#define BOARD_SIZE 8

// Spielbrett
int** spielfeld;
int* LEERES_FELD;

// Methode zur Überprüfung, ob ein Stein einen anderen schlagen kann
bool canStoneCapture(struct position currentPlayer, struct position opponent, int playercolor) {
    // Hier wird geprüft, ob das Zielfeld sich auf dem Spielbrett befindet
    if (opponent.x < 0 || opponent.x >= BOARD_SIZE || opponent.y < 0 || opponent.y >= BOARD_SIZE) {
        return false;
    }

    // Überprüfe, ob das Zielfeld leer ist
    if (spielfeld[opponent.x][opponent.y] == *LEERES_FELD) {
        // Überprüfe, ob der Stein diagonal neben dem Gegner liegt, je nach Spielerfarbe
        if ((playercolor == 'X' && currentPlayer.x - 1 == opponent.x) || (playercolor == 'O' && currentPlayer.x + 1 == opponent.x)) {
            // Überprüfe, ob das Zielfeld diagonal hcharer dem Gegner liegt
            if (currentPlayer.y - 1 == opponent.y || currentPlayer.y + 1 == opponent.y) {
                return true;
            }
        }
    }

    return false;
}

// Methode, die für einen gegebenen Spieler überprüft, ob einer seiner Steine einen Gegner schlagen kann
bool canAnyStoneCapture(struct position spielerPosition[], int spielerAnzahl, struct position gegnerPosition, int playercolor) {
    for (int i = 0; i < spielerAnzahl; ++i) {
        if (canStoneCapture(spielerPosition[i], gegnerPosition, playercolor)) {
            return true;
        }
    }
    return false;
}*/

