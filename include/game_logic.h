#ifndef GAME_LOGIC
#define GAME_LOGIC

#define P1_PAWN 1
#define P2_PAWN 2
#define P1_KING 3
#define P2_KING 4
#define EMPTY_BOX 0
#define ILLEGAL_SQUARE -9
#define BOARD_SIZE 8
#define MAX_IP_LEN 32 //ip address

typedef enum {
    MENU_STATE,
    GAME_STATE,
    NETWORK_SETUP_STATE,
    NETWORK_CONNECT_STATE,
} GameState;

//for cpu gameplay(moves)
#define MAX_JUMP_CHAIN 12
typedef struct {
    int startX, startY;
    int pathX[MAX_JUMP_CHAIN];
    int pathY[MAX_JUMP_CHAIN];
    int length;  // number of jumps in chain
    int finalX, finalY;
} Move;

void initBoard(int [BOARD_SIZE][BOARD_SIZE]);

void initCpu();

void cpuMakeMove();

void boxClicked(int row, int col, int *sPFlag, int *sPRow, int *sPCol, int Board[BOARD_SIZE][BOARD_SIZE], int *playerTurn);

int checkWinCondition(int Board[BOARD_SIZE][BOARD_SIZE]);

int getPieceDirection(int piece);

int isValidMove(int fromRow, int fromCol, int toRow, int toCol, int piece, int Board[BOARD_SIZE][BOARD_SIZE]);

int isValidJump(int fromRow, int fromCol, int toRow, int toCol, int piece, int Board[BOARD_SIZE][BOARD_SIZE]);

int hasMoreJumps(int row, int col, int piece, int Board[BOARD_SIZE][BOARD_SIZE]);

int hasValidMoves(int player, int Board[BOARD_SIZE][BOARD_SIZE]);

int canCapture(int x, int y, int piece, int Board[BOARD_SIZE][BOARD_SIZE]);

void simulateCaptureChain(int Board[BOARD_SIZE][BOARD_SIZE], int x, int y, int piece, Move *currentMove, Move *bestMove);

int countPieces(int player, int Board[BOARD_SIZE][BOARD_SIZE]);

void tryPromoteToKing(int row, int col, int Board[BOARD_SIZE][BOARD_SIZE]);

void resetGame(int Board[BOARD_SIZE][BOARD_SIZE], int *sPFlag, int *sPRow, int *sPCol);

void switchPlayer(int *PlayerTurn);


#endif