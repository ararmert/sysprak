#ifndef SHARED_DATA_H
#define SHARED_DATA_H

// !!!!!!! add structs that need to be used in both source code files 
// (performConnection.c and sysprak-client.c) here and link this header in both code files to include commonly used structs functions etc
struct Player{
    int playerNum;
    int playerName;
    int isReady;
};


struct SharedData{
    struct Player player;
    struct Player opponent;
    char gameName[256];
    int totalPlayers;
    int selfPlayer;
    pid_t thinkerPID;
    pid_t connectorPID;

    bool shouldThink;
};


char* recvLine(int socket_fd, char* buffer);


#endif