#ifndef GAME_LOGIC
#define GAME_LOGIC

#define P1_PAWN 1
#define P2_PAWN 2
#define P1_KING 3
#define P2_KING 4
#define EMPTY_BOX 0
#define BOARD_SIZE 8
#include <stdbool.h>

bool piece_selected false;

void initBoard(int [8][8]);

void boxClicked(int col, int row, int Board[BOARD_SIZE][BOARD_SIZE]);


#endif