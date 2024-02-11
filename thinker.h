#include "board.h"
struct position {
    int x;
    int y; 
    enum Color color;
    enum Status status;
};
//void think(int pipe_write_fd);
char* move(struct position currentPlayer, struct position* spielerPositionen, struct position* opponent);
struct position canAnyStoneCapture(struct position* spielerPositionen, struct position* gegnerPosition);

/*enum Color{
    black, 
    white
};
enum Status{
    SOLDIER,
    QUEEN
};*/
