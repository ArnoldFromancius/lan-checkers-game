#ifndef CPU_H
#define CPU_H
#include "game_logic.h"

typedef struct {
    int fromRow, fromCol;
    int toRow, toCol;
} Move;

Move getRandomCpuMove(int board[BOARD_SIZE][BOARD_SIZE], int pieceType);
int generateAllValidMoves(int board[BOARD_SIZE][BOARD_SIZE], int pieceType, Move movesOut[], int maxMoves);
#endif
