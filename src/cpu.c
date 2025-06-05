#include "../include/cpu.h"
#include "../include/game_logic.h"
#include <stdlib.h>
#include <time.h>

#define MAX_MOVES 100

int generateAllValidMoves(int board[BOARD_SIZE][BOARD_SIZE], int pieceType, Move movesOut[], int maxMoves) {
    int count = 0;

    // Check for jumps first
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == pieceType || board[row][col] == pieceType + 1) {
                for (int dr = -2; dr <= 2; dr++) {
                    for (int dc = -2; dc <= 2; dc++) {
                        if (abs(dr) == abs(dc) && dr != 0) {
                            int newRow = row + dr;
                            int newCol = col + dc;
                            if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                                if (isValidJump(row, col, newRow, newCol, pieceType, board)) {
                                    if (count < maxMoves) {
                                        movesOut[count++] = (Move){row, col, newRow, newCol};
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (count > 0) return count; // Found jumps

    // Else, check normal moves
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == pieceType || board[row][col] == pieceType + 1) {
                for (int dr = -1; dr <= 1; dr += 2) {
                    for (int dc = -1; dc <= 1; dc += 2) {
                        int newRow = row + dr;
                        int newCol = col + dc;
                        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
                            if (isValidMove(row, col, newRow, newCol, pieceType, board)) {
                                if (count < maxMoves) {
                                    movesOut[count++] = (Move){row, col, newRow, newCol};
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return count;
}

Move getRandomCpuMove(int board[BOARD_SIZE][BOARD_SIZE], int pieceType) {
    Move moveList[MAX_MOVES];
    int count = generateAllValidMoves(board, pieceType, moveList, MAX_MOVES);

    if (count == 0) return (Move){-1, -1, -1, -1};

    srand(time(NULL)); 
    return moveList[rand() % count];
}
