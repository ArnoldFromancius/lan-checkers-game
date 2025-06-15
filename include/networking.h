#ifndef NETWORK_H
#define NETWORK_H


#include <stdbool.h>

#define PORT 12345
#define MOVE_MSG 1
#define QUIT_MSG 2

const char *get_local_ip();  // returns pointer to static string
bool networkSetup(bool isHost, const char *ip);
bool isClientConnected();
int recvBoard(int sock, int Board[8][8]);
int sendBoard(int sock, int Board[8][8]);
int getSocketFD();
void closeNetwork();

#endif
