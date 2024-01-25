#ifndef SHARED_DATA_H
#define SHARED_DATA_H

// !!!!!!! add structs that need to be used in both source code files 
// (performConnection.c and sysprak-client.c) here and link this header in both code files to include commonly used structs functions etc
struct Player{
    int playerNum;
    int playerName;
    bool isReady;
};

#endif