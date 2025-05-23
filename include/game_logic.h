#ifndef GAME_LOGIC
#define GAME_LOGIC

#define P1_PAWN 1
#define P2_PAWN 2
#define P1_KING 3
#define P2_KING 4
#define EMPTY_BOX 0
#define BOARD_SIZE 8


void initBoard(int [BOARD_SIZE][BOARD_SIZE]);

void boxClicked(int col, int row, int *sPFlag, int *sPRow, int *sPCol, int Board[BOARD_SIZE][BOARD_SIZE]);

int getPieceDirection(int piece);

int isValidMove(int fromRow, int fromCol, int toRow, int toCol, int piece);

int isValidJump(int fromRow, int fromCol, int toRow, int toCol, int piece, int Board[BOARD_SIZE][BOARD_SIZE]);

#endif