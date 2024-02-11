#include "board.h"
struct position {
    int x;
    int y; 
    enum Color color;
    enum Status status;
};
//void think(int pipe_write_fd);
char* move(struct position currentPlayer, struct position* spielerPosition, struct position* opponent, int sizeopponent, int sizespielerPosition);
struct position canAnyStoneCapture(struct position* spielerPosition, struct position* opponnent, int sizeopponent, int sizespielerPosition);

/*enum Color{
    black, 
    white
};
enum Status{
    SOLDIER,
    QUEEN
};*/
