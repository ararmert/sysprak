#ifndef BOARD_H
#define BOARD_H

// 定义棋子状态和颜色的枚举
enum Status{
    SOLDIER,
    QUEEN
};

enum Color{
    black, 
    white
};

// 定义棋子结构
struct Piece{
    enum Color color;
    enum Status status;
    char x;
    int y;
};

// 声明处理棋盘和打印棋盘的函数
void savePiecesInSHM(struct Piece *pieces, char spielstand[8][8]);
void PrintSavedSD(char spielstand[8][8]);
void printPieces(struct Piece *pieces, int count);

#endif // BOARD_H
