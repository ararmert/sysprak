#include <stdbool.h>


struct Piece {
    int x;
    int y; 
    enum Color color;
    enum Status status;

    bool isKing; // zum Überprüfen ob der stein ein damestein ist!
    //int* Color;

};
enum Color{
    black, 
    white
};
enum Status{
    SOLDIER,
    QUEEN
};


#define BOARD_SIZE 8

int** spielfeld;
int* LEERES_FELD;

void split(struct Piece*, struct Piece** opponent, struct Piece** own);


bool isFieldFree(int y, int x, struct Piece* opponent, struct Piece* spielerPosition){
    int size = sizeof(opponent)/ sizeof(struct Piece);
for (int i = 0; i < size; i++){

   if ( y == opponent[i].y && x == opponent[i].x)
   {
    return false;
   }
}
    size= sizeof(spielerPosition)/ sizeof(struct Piece);
for (int i = 0; i < size; i++)
{
    if (y == spielerPosition[i].y && x == spielerPosition[i].x)
    {
       return false;
    }
    
}
}

    
// Methode3.1 prüft ob einer stein einen anderen schlagen kann
bool canStoneCapture(struct Piece currentPlayer, struct Piece* opponent, struct Piece* spielerPosition, struct Piece* Color) {
    // zielfeld aufm spielbrett?
    int size = sizeof(opponent)/ sizeof(struct Piece);

    // für dunkle Spieler
    for(int i=0; i< size; i++){

        
    if(currentPlayer.color == "Black" && currentPlayer.x-1 == opponent[i].x){
        if(currentPlayer.y -1 == opponent[i].y){
            if(isFieldFree(currentPlayer.y-2, currentPlayer.x-2, opponent , spielerPosition )){
                return true;  

            }
        }
        if(currentPlayer.y +1 == opponent[i].y){
            if(isFieldFree(currentPlayer.y+2, currentPlayer.x-2, opponent , spielerPosition )){
                return true;

            }  
        }
    } 

    if(currentPlayer.color == "White" && currentPlayer.x+1 == opponent[i].x){
        if(currentPlayer.y -1 == opponent[i].y){
            if(isFieldFree(currentPlayer.y-2, currentPlayer.x+2, opponent , spielerPosition )){

            i--;
            }
        }
        if(currentPlayer.y +1 == opponent[i].y){
            if(isFieldFree(currentPlayer.y+2, currentPlayer.x+2, opponent , spielerPosition )){

            i--;
            } 
        }
    }
    }


    return false;
  }

   
// Methode 3.2 && 3.3
//einer der Steine einen Gegner schlagen kann?
struct Piece canAnyStoneCapture(struct Piece* spielerPositionen, struct Piece* gegnerPosition) {
    int eigeneSteine = sizeof(spielerPositionen)/ sizeof(struct Piece);
    for (int i = 0; i < eigeneSteine; ++i) {
        if (spielerPositionen[i].isKing && canKingCapture(spielerPositionen[i], gegnerPosition)){
            return spielerPositionen[i];
            
        }
        else if(canStoneCapture(spielerPositionen[i], gegnerPosition)) {
            return spielerPositionen[i];
        }
    }
    struct Piece tmp= {.x =0 , .y =0, .isKing=0};
    return tmp;
}

int obenRechtsHelper(struct Piece currentPlayer, struct Piece* opponent, struct Piece* spielerPosition){
    int size = sizeof(opponent)/ sizeof(struct Piece);
    for(int i=0; i< size; i++){
        if(currentPlayer.y+ i == opponent[i].y && currentPlayer.x+ i == opponent[i].x){
            if(isFieldFree(currentPlayer.y+i+1, currentPlayer.x+i+1, opponent , spielerPosition ))
                return 1;
            return 0;
        }
        // checkt ob auf nächstem Feld ein Freund liegt
        if(!isFieldFree(currentPlayer.y+ i, currentPlayer.x+i, opponent , spielerPosition)){
            return 0;
        }
    }
    return -1;
}
int obenLinksHelper(struct Piece currentPlayer, struct Piece* opponent, struct Piece* spielerPosition){
    int size = sizeof(opponent)/ sizeof(struct Piece);
    for(int i=0; i< size; i++){
        if(currentPlayer.y- i == opponent[i].y && currentPlayer.x+ i == opponent[i].x){
            if(isFieldFree(currentPlayer.y-i-1, currentPlayer.x+i+1, opponent , spielerPosition ))
                return 1;
            return 0;
        }
        // checkt ob auf nächstem Feld ein Freund liegt
        if(!isFieldFree(currentPlayer.y- i, currentPlayer.x+i, opponent , spielerPosition)){
            return 0;
        }
    }
    return -1;
}

int untenRechtsHelper(struct Piece currentPlayer, struct Piece* opponent, struct Piece* spielerPosition){
    int size = sizeof(opponent)/ sizeof(struct Piece);
    for(int i=0; i< size; i++){
        if(currentPlayer.y+ i == opponent[i].y && currentPlayer.x- i == opponent[i].x){
            if(isFieldFree(currentPlayer.y+i+1, currentPlayer.x-i-1, opponent , spielerPosition ))
                return 1;
            return 0;
        }
        // checkt ob auf nächstem Feld ein Freund liegt
        if(!isFieldFree(currentPlayer.y+ i, currentPlayer.x-i, opponent , spielerPosition)){
            return 0;
        }
    }
    return -1;
}

int untenLinkssHelper(struct Piece currentPlayer, struct Piece* opponent, struct Piece* spielerPosition){
    int size = sizeof(opponent)/ sizeof(struct Piece);
    for(int i=0; i< size; i++){
        if(currentPlayer.y- i == opponent[i].y && currentPlayer.x- i == opponent[i].x){
            if(isFieldFree(currentPlayer.y-i-1, currentPlayer.x-i-1, opponent , spielerPosition ))
                return 1;
            return 0;
        }
        // checkt ob auf nächstem Feld ein Freund liegt
        if(!isFieldFree(currentPlayer.y- i, currentPlayer.x-i, opponent , spielerPosition)){
            return 0;
        }
    }
    return -1;
}

// Methode 3.3 && 3.4
// Def:ob ein stein Dame ist bzw. ein damenstein schlagen kann! -> wenn die Zeilen- und Spaltenunterschiede gleich sind
bool canKingCapture(struct Piece currentPlayer, struct Piece* opponent, struct Piece* spielerPosition) {
    int size = sizeof(opponent)/ sizeof(struct Piece);

    int i= 0;
    while(true){
        if(currentPlayer.y+ i >= BOARD_SIZE || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        }  
        int tmp= obenRechtsHelper(currentPlayer, opponent, spielerPosition);
        if(tmp == 1)
            return true;
        else if(tmp == 0);
            break;
        i++;
    }

    while(true){
        if(currentPlayer.y- i >= BOARD_SIZE || currentPlayer.x+ i >= BOARD_SIZE){
            break;
        }  
        int tmp= obenlinksHelper(currentPlayer, opponent, spielerPosition);
        if(tmp == 1)
            return true;
        else if(tmp == 0);
            break;
        i++;
    }

    while(true){
        if(currentPlayer.y+ i >= BOARD_SIZE || currentPlayer.x- i >= BOARD_SIZE){
            break;
        }  
        int tmp= untenRechtsHelper(currentPlayer, opponent, spielerPosition);
        if(tmp == 1)
            return true;
        else if(tmp == 0);
            break;
        i++;
    }

    while(true){
        if(currentPlayer.y- i >= BOARD_SIZE || currentPlayer.x- i >= BOARD_SIZE){
            break;
        }  
        int tmp= untenLinkssHelper(currentPlayer, opponent, spielerPosition);
        if(tmp == 1)
            return true;
        else if(tmp == 0);
            break;
        i++;
    }

    // für alle 4 Richtungen schreiben -> Done!

    return false;


  }



bool (*whatForStone(bool Dame, int playerColor))(struct Piece, struct Piece, int) {
    if (canKingCapture) {
        return &canAnyStoneCapture; // die frage ist soll ich hier die die Adresse der Funktion zurückgeben lassen oder die Funktion selbst?
    } else {
        return &canStoneCapture; 
    }
}

// return zu strings stat true
int* move(struct Piece currentPlayer, struct Piece* spielerPositionen, struct Piece* opponent){
    int result [256]= '/0';
    if(currentPlayer.y == 0 && currentPlayer.x == 0){
        int eigeneSteine = sizeof(spielerPositionen)/ sizeof(struct Piece);

        for (int i = 0; i < eigeneSteine; ++i) {
            if(!spielerPositionen[i].isKing){
                if(spielerPositionen[i].color == "Black"){
                    if(isFieldFree(spielerPositionen[i].y +1, spielerPositionen[i].x -1, opponent , spielerPositionen )){
                        struct Piece tmp= {.x = currentPlayer.x+1, .y =currentPlayer.y-1, .isKing=currentPlayer.isKing, .color= currentPlayer.Color};
                        return strcat(positionToString(currentPlayer), positionToString(tmp)); // mit strcat : an ersten String
                    }
                    if(isFieldFree(spielerPositionen[i].y -1, spielerPositionen[i].x -1, opponent , spielerPositionen )){
                        return true;
                    }
                }

                //selbe für white


                if(spielerPositionen[i].color == "White"){
                    if(isFieldFree(spielerPositionen[i].y +1, spielerPositionen[i].x +1, opponent , spielerPositionen )){
                        struct Piece tmp= {.x = currentPlayer.x-1, .y =currentPlayer.y-1, .isKing=currentPlayer.isKing, .color= currentPlayer.Color};
                        return strcat(positionToString(currentPlayer), positionToString(tmp)); // mit strcat : an ersten String
                    }
                    if(isFieldFree(spielerPositionen[i].y -1, spielerPositionen[i].x +1, opponent , spielerPositionen )){
                        return true;
                    }
                }
            }else{
                if(isFieldFree(currentPlayer.y+1, currentPlayer.x+1, opponent , spielerPositionen )){
                    return true;
                }

                if(isFieldFree(currentPlayer.y-1, currentPlayer.x+1, opponent , spielerPositionen )){
                    return true;
                }

                if(isFieldFree(currentPlayer.y+1, currentPlayer.x-1, opponent , spielerPositionen )){
                    return true;
                }

                if(isFieldFree(currentPlayer.y-1, currentPlayer.x-1, opponent , spielerPositionen )){
                    return true;
                }

                // selbe 4 für Richtungen
            }
        }
    }
    else{
        result == positionToString(currentPlayer);
        if(!currentPlayer.isKing){
            int size = sizeof(opponent)/ sizeof(struct Piece);
            for(int i=0; i< size;++i){
                if(currentPlayer.Color == "Black" && currentPlayer.x-1 == opponent[i].x){
                    if(currentPlayer.y -1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y-2, currentPlayer.x-2, opponent , spielerPositionen )){
                            return true;  

                        }
                    }
                    if(currentPlayer.y +1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y+2, currentPlayer.x-2, opponent , spielerPositionen )){
                            return true;
                        }  
                    }
                } 
            }

            // selbe für Weiß

            for(int i=0; i< size;++i){
                if(currentPlayer.Color == "White" && currentPlayer.x+1 == opponent[i].x){
                    if(currentPlayer.y -1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y-2, currentPlayer.x+2, opponent , spielerPositionen )){
                            return true;  

                        }
                    }
                    if(currentPlayer.y +1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y+2, currentPlayer.x+2, opponent , spielerPositionen )){
                            return true;
                        }  
                    }
                } 
            }

        }else{

        }
    }
}

int* positionToString(struct Piece currentPlayer){
    int y= currentPlayer.y;
    int result [2]= '/0';
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
                    
    }
    result[1]= currentPlayer.x;

    return result;
}

int* recusviBlack(struct Piece currentPlayer, struct Piece* spielerPositionen, struct Piece* opponent){
                int size = sizeof(opponent)/ sizeof(struct Piece);
            for(int i=0; i< size;++i){
                if(currentPlayer.Color == "Black" && currentPlayer.x-1 == opponent[i].x){
                    if(currentPlayer.y -1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y-2, currentPlayer.x-2, opponent , spielerPositionen )){
                            struct Piece tmp= {.x = currentPlayer.x-2, .y =currentPlayer.y-2, .isKing=currentPlayer.isKing, .Color= currentPlayer.Color};
                            return strcat(strcat(postionToString(currentPlayer), ":"), recusviBlack(tmp, spielerPositionen, opponent));
                        }
                    }
                    if(currentPlayer.y +1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y+2, currentPlayer.x-2, opponent , spielerPositionen )){
                            return true; // replace see open
                        }  
                    }
                }

                if(currentPlayer.Color == "White" && currentPlayer.x+1 == opponent[i].x){
                    if(currentPlayer.y -1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y-2, currentPlayer.x+2, opponent , spielerPositionen )){
                            struct Piece tmp= {.x = currentPlayer.x-2, .y =currentPlayer.y+2, .isKing=currentPlayer.isKing, .Color= currentPlayer.Color};
                            return strcat(strcat(postionToString(currentPlayer), ":"), recusviBlack(tmp, spielerPositionen, opponent));
                        }
                    }
                    if(currentPlayer.y +1 == opponent[i].y){
                        if(isFieldFree(currentPlayer.y+2, currentPlayer.x+2, opponent , spielerPositionen )){
                            return true; // replace see open
                        }  
                    }
                }  
            }
            return "\n" ; //testen
}
