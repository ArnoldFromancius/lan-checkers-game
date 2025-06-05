#ifndef NETWORK_H
#define NETWORK_H


#include <stdbool.h>

#define PORT 12345
#define MOVE_MSG 1
#define QUIT_MSG 2
typedef struct {
    int type;      // MOVE_MSG or QUIT_MSG
    int fromX, fromY;
    int toX, toY;
} MovePacket;

const char *get_local_ip();  // returns pointer to static string
bool networkSetup(bool isHost, const char *ip);
bool sendMove(MovePacket move);
bool receiveMove(MovePacket *move);
void closeNetwork();

#endif
