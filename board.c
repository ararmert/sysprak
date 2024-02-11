#include "board.h"
#include <stdio.h>

// 定义棋盘上棋子的处理函数
void CatchPieces(struct Piece *pieces, char spielstand[8][8]) {
    int index = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            switch (spielstand[y][x]) {
                case 'b':
                    pieces[index].color = black;
                    pieces[index].status = SOLDIER;
                    pieces[index].x = 'A' + x;
                    pieces[index].y = y + 1;
                    pieces[index].exist = true;
                    index++;
                    break;

                case 'B':
                    pieces[index].color = black;
                    pieces[index].status = QUEEN;
                    pieces[index].x = 'A' + x;
                    pieces[index].y = y + 1;
                    pieces[index].exist = true;
                    index++;
                    break;

                case 'w':
                    pieces[index].color = white;
                    pieces[index].status = SOLDIER;
                    pieces[index].x = 'A' + x;
                    pieces[index].y = y + 1;
                    pieces[index].exist = true;
                    index++;
                    break;

                case 'W':
                    pieces[index].color = white;
                    pieces[index].status = QUEEN;
                    pieces[index].x = 'A' + x;
                    pieces[index].y = y + 1;
                    pieces[index].exist = true;
                    index++;
                    break;
            }
        }
    }
}

// 定义打印棋盘的函数
void PrintSavedSD(char spielstand[8][8]) {
printf("   A B C D E F G H\n");
    printf("  +---------------+\n");
        for (int y = 0; y < 8; y++) {
                printf("%d| ", y + 1);
        for (int x = 0; x < 8; x++) {
            printf("%c ", spielstand[y][x]);
        }
        printf("|%d\n", y + 1);
    }
    printf("  +---------------+\n");
    printf("   A B C D E F G H\n");
}

// 定义打印棋子信息的函数
void printPieces(struct Piece *pieces, int count) {
    printf("Information for all the Pieces: \n");
    for (int i = 0; i < count; i++) {
        printf("Piece %d: Color = %s, Status = %s, Position = %c%d, Existence = %s\n",
               i,
               pieces[i].color == black ? "Black" : "White",
               pieces[i].status == SOLDIER ? "Soldier" : "Queen",
               pieces[i].x,
               pieces[i].y,
               pieces[i].exist ? "True" : "False");

    }
}
