#include <stdbool.h>


struct Position {
    int row;
    int col; 
    bool isKing; // zum Überprüfen ob der stein ein damestein ist!
    char* Color;
};


#define BOARD_SIZE 8

int** spielfeld;
int* LEERES_FELD;

split(struct Position*, struct Position** opponent, struct Position** own);


bool isFieldFree(int col, int row, struct Position* opponent, struct Position* spielerPosition){
    int size = sizeof(opponent)/ sizeof(struct Position);
for (int i = 0; i < size; i++){

   if ( col == opponent[i].col && row == opponent[i].row)
   {
    return false;
   }
}
    size= sizeof(spielerPosition)/ sizeof(struct Position);
for (int i = 0; i < size; i++)
{
    if (col == spielerPosition[i].col && row == spielerPosition[i].row)
    {
       return false;
    }
    
}
}

    
// Methode3.1 prüft ob einer stein einen anderen schlagen kann
bool canStoneCapture(struct Position currentPlayer, struct Position* opponent, struct Position* spielerPosition, struct Position* Color) {
    // zielfeld aufm spielbrett?
    int size = sizeof(opponent)/ sizeof(struct Position);

    // für dunkle Spieler
    for(int i=0; i< size; i++){

    //if (opponent[i].row < 0 || opponent[i].row >= BOARD_SIZE || opponent[i].col < 0 || opponent[i].col >= BOARD_SIZE) {
    //    return false;
    //}
        
    if(currentPlayer.Color == "Black" && currentPlayer.row-1 == opponent[i].row){
        if(currentPlayer.col -1 == opponent[i].col){
            if(isFieldFree(currentPlayer.col-2, currentPlayer.row-2, opponent , spielerPosition )){
                return true;  

            }
        }
        if(currentPlayer.col +1 == opponent[i].col){
            if(isFieldFree(currentPlayer.col+2, currentPlayer.row-2, opponent , spielerPosition )){
                return true;

            }  
        }
    } 

    if(currentPlayer.Color == "White" && currentPlayer.row+1 == opponent[i].row){
        if(currentPlayer.col -1 == opponent[i].col){
            if(isFieldFree(currentPlayer.col-2, currentPlayer.row+2, opponent , spielerPosition )){

            i--;
            }
        }
        if(currentPlayer.col +1 == opponent[i].col){
            if(isFieldFree(currentPlayer.col+2, currentPlayer.row+2, opponent , spielerPosition )){

            i--;
            } 
        }
    }
    }

// für helle Spieler
    /*for (int j = 0; j < size; j++){

        if (opponent[j].row < 0 || opponent[j].row >= BOARD_SIZE || opponent[j].col < 0 || opponent[j].col >= BOARD_SIZE) {
        return false;
    }
        
    if(currentPlayer.Color == "White" && currentPlayer.row-1 == opponent[j].row){
        if(currentPlayer.col -1 == opponent[j].col){
            if(isFieldFree(currentPlayer.col-2, currentPlayer.row-2, opponent , spielerPosition )){


           j--;     

            }
        }
        if(currentPlayer.col +1 == opponent[j].col){
            if(isFieldFree(currentPlayer.col+2, currentPlayer.row-2, opponent , spielerPosition )){

            j--;
            }  
        }
    } 

    if(currentPlayer.Color == "White" && currentPlayer.row+1 == opponent[j].row){
        if(currentPlayer.col -1 == opponent[j].col){
            if(isFieldFree(currentPlayer.col-2, currentPlayer.row+2, opponent , spielerPosition )){

            j--;
            }
        }
        if(currentPlayer.col +1 == opponent[j].col){
            if(isFieldFree(currentPlayer.col+2, currentPlayer.row+2, opponent , spielerPosition )){

            j--;
            } 
        }
    }
    }*/
                     
            
       
    
    return false;
    }

   
// Methode 3.2 && 3.3
//einer der Steine einen Gegner schlagen kann?
struct Position canAnyStoneCapture(struct Position* spielerPositionen, struct Position* gegnerPosition) {
    int eigeneSteine = sizeof(spielerPositionen)/ sizeof(struct Position);
    for (int i = 0; i < eigeneSteine; ++i) {
        if (spielerPositionen[i].isKing && canKingCapture(spielerPositionen[i], gegnerPosition)){
            return spielerPositionen[i];
            
        }
        else if(canStoneCapture(spielerPositionen[i], gegnerPosition)) {
            return spielerPositionen[i];
        }
    }
    struct Position tmp= {.row =0 , .col =0, .isKing=0};
    return tmp;
}

int obenRechtsHelper(struct Position currentPlayer, struct Position* opponent, struct Position* spielerPosition){
    int size = sizeof(opponent)/ sizeof(struct Position);
    for(int i=0; i< size; i++){
        if(currentPlayer.col+ i == opponent[i].col && currentPlayer.row+ i == opponent[i].row){
            if(isFieldFree(currentPlayer.col+i+1, currentPlayer.row+i+1, opponent , spielerPosition ))
                return 1;
            return 0;
        }
        // checkt ob auf nächstem Feld ein Freund liegt
        if(!isFieldFree(currentPlayer.col+ i, currentPlayer.row+i, opponent , spielerPosition)){
            return 0;
        }
    }
    return -1;
}

// Methode 3.3 && 3.4
// Def:ob ein stein Dame ist bzw. ein damenstein schlagen kann! -> wenn die Zeilen- und Spaltenunterschiede gleich sind
bool canKingCapture(struct Position currentPlayer, struct Position* opponent, struct Position* spielerPosition) {
    int size = sizeof(opponent)/ sizeof(struct Position);

    int i= 0;
    while(true){
        if(currentPlayer.col+ i >= BOARD_SIZE || currentPlayer.row+ i >= BOARD_SIZE){
            break;
        }  
        int tmp= obenRechtsHelper(currentPlayer, opponent, spielerPosition);
        if(tmp == 1)
            return true;
        else if(tmp == 0);
            break;
        i++;
    }

    // für alle 4 Richtungen schreiben

    return false;


    /*int rowDiff = abs(currentPlayer.row - opponent.row);
    int colDiff = abs(currentPlayer.col - opponent.col);
    return rowDiff == colDiff;*/
}


bool (*whatForStone(bool Dame, char playerColor))(struct Position, struct Position, char) {
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
char* move(struct Position currentPlayer, struct Position* spielerPositionen, struct Position* opponent){
    char result [256]= '/0';
    if(currentPlayer.col == 0 && currentPlayer.row == 0){
        int eigeneSteine = sizeof(spielerPositionen)/ sizeof(struct Position);

        for (int i = 0; i < eigeneSteine; ++i) {
            if(!spielerPositionen[i].isKing){
                if(spielerPositionen[i].Color == "Black"){
                    if(isFieldFree(spielerPositionen[i].col +1, spielerPositionen[i].row -1, opponent , spielerPositionen )){
                        struct Position tmp= {.row = currentPlayer.row+1, .col =currentPlayer.col-1, .isKing=currentPlayer.isKing, .Color= currentPlayer.Color};
                        return strcat(positionToString(currentPlayer), positionToString(tmp)); // mit strcat : an ersten String
                    }
                    if(isFieldFree(spielerPositionen[i].col -1, spielerPositionen[i].row -1, opponent , spielerPositionen )){
                        return true;
                    }
                }

                //selbe für white
            }else{
                if(isFieldFree(currentPlayer.col+1, currentPlayer.row+1, opponent , spielerPositionen )){
                    return true;
                }

                // selbe 4 für Richtungen
            }
        }
    }
    else{
        result == positionToString(currentPlayer);
        if(!currentPlayer.isKing){
            int size = sizeof(opponent)/ sizeof(struct Position);
            for(int i=0; i< size;++i){
                if(currentPlayer.Color == "Black" && currentPlayer.row-1 == opponent[i].row){
                    if(currentPlayer.col -1 == opponent[i].col){
                        if(isFieldFree(currentPlayer.col-2, currentPlayer.row-2, opponent , spielerPositionen )){
                            return true;  

                        }
                    }
                    if(currentPlayer.col +1 == opponent[i].col){
                        if(isFieldFree(currentPlayer.col+2, currentPlayer.row-2, opponent , spielerPositionen )){
                            return true;
                        }  
                    }
                } 
            }

            // selbe für Weiß

        }else{

        }
    }
}

char* positionToString(struct Position currentPlayer){
    int y= currentPlayer.col;
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
    result[1]= currentPlayer.row;

    return result;
}

char* recusviBlack(struct Position currentPlayer, struct Position* spielerPositionen, struct Position* opponent){
                int size = sizeof(opponent)/ sizeof(struct Position);
            for(int i=0; i< size;++i){
                if(currentPlayer.Color == "Black" && currentPlayer.row-1 == opponent[i].row){
                    if(currentPlayer.col -1 == opponent[i].col){
                        if(isFieldFree(currentPlayer.col-2, currentPlayer.row-2, opponent , spielerPositionen )){
                            struct Position tmp= {.row = currentPlayer.row-2, .col =currentPlayer.col-2, .isKing=currentPlayer.isKing, .Color= currentPlayer.Color};
                            return strcat(strcat(postionToString(currentPlayer), ":"), recusviBlack(tmp, spielerPositionen, opponent));
                        }
                    }
                    if(currentPlayer.col +1 == opponent[i].col){
                        if(isFieldFree(currentPlayer.col+2, currentPlayer.row-2, opponent , spielerPositionen )){
                            return true; // replace see open
                        }  
                    }
                } 
            }
            return "\n"//testen
}

     
        



       /* if ((currentPlayer.Color == "Black" && currentPlayer.row - 1 == opponent[i].row ) || ( currentPlayer.Color == "White" && currentPlayer.row + 1 == opponent.row)) {
            
            if (currentPlayer.col - 1 == opponent.col || currentPlayer.col + 1 == opponent.col) {
                return true;
            }
        }*/


/*..................................*/

/*#include <stdbool.h>

// Position des Spielsteins
struct Position {
    int row;
    int col;
};

// Spielfeldgröße
#define BOARD_SIZE 8

// Spielbrett
int** spielfeld;
int* LEERES_FELD;

// Methode zur Überprüfung, ob ein Stein einen anderen schlagen kann
bool canStoneCapture(struct Position currentPlayer, struct Position opponent, char playerColor) {
    // Hier wird geprüft, ob das Zielfeld sich auf dem Spielbrett befindet
    if (opponent.row < 0 || opponent.row >= BOARD_SIZE || opponent.col < 0 || opponent.col >= BOARD_SIZE) {
        return false;
    }

    // Überprüfe, ob das Zielfeld leer ist
    if (spielfeld[opponent.row][opponent.col] == *LEERES_FELD) {
        // Überprüfe, ob der Stein diagonal neben dem Gegner liegt, je nach Spielerfarbe
        if ((playerColor == 'X' && currentPlayer.row - 1 == opponent.row) || (playerColor == 'O' && currentPlayer.row + 1 == opponent.row)) {
            // Überprüfe, ob das Zielfeld diagonal hinter dem Gegner liegt
            if (currentPlayer.col - 1 == opponent.col || currentPlayer.col + 1 == opponent.col) {
                return true;
            }
        }
    }

    return false;
}

// Methode, die für einen gegebenen Spieler überprüft, ob einer seiner Steine einen Gegner schlagen kann
bool canAnyStoneCapture(struct Position spielerPositionen[], int spielerAnzahl, struct Position gegnerPosition, char playerColor) {
    for (int i = 0; i < spielerAnzahl; ++i) {
        if (canStoneCapture(spielerPositionen[i], gegnerPosition, playerColor)) {
            return true;
        }
    }
    return false;
}*/

