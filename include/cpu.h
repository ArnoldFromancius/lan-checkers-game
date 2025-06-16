#ifndef CPU_H
#define CPU_H

#include "game_logic.h"  // or wherever BOARD_SIZE and constants are

typedef struct {
    int row, col;
} Coord;

typedef struct {
    Coord path[12]; // from → to1 → to2 ...
    int length;
} MoveChain;

void copyBoard(int src[BOARD_SIZE][BOARD_SIZE], int dst[BOARD_SIZE][BOARD_SIZE]);
int simulateMove(int board[BOARD_SIZE][BOARD_SIZE], MoveChain move, int isJump);
void applyMove(int board[BOARD_SIZE][BOARD_SIZE], MoveChain move, int piece);
void findBestCapture(int board[BOARD_SIZE][BOARD_SIZE], int x, int y, int piece, MoveChain* current, MoveChain* best);
void cpuMakeMove(int board[BOARD_SIZE][BOARD_SIZE], int* PlayerTurn);

#endif
