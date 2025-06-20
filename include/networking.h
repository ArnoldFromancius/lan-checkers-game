#ifndef NETWORK_H
#define NETWORK_H


#include <stdbool.h>

#define PORT 12345
#define MOVE_MSG 1
#define QUIT_MSG 2

typedef struct {
    int flag;         // 1 = actual move made, not just a basic click
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
    int isCapture;    // 0 = move, 1 = capture
    int moreJumps;    // 1 = more jumps expected
} MovePacket;

const char *get_local_ip();  // returns pointer to static string
bool networkSetup(bool isHost, const char *ip);
bool isClientConnected();
int recvMove(int sock, MovePacket *move);
int sendMove(int sock, MovePacket *move);
int getSocketFD();
void closeNetwork();

#endif
