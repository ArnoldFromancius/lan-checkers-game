
#include <stdlib.h>
#include <string.h>
#include "../include/cpu.h"

void copyBoard(int src[BOARD_SIZE][BOARD_SIZE], int dst[BOARD_SIZE][BOARD_SIZE]) {
    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
            dst[y][x] = src[y][x];
}

int simulateMove(int board[BOARD_SIZE][BOARD_SIZE], MoveChain move, int isJump) {
    int captures = 0;

    for (int i = 0; i < move.length - 1; i++) {
        int fromY = move.path[i].row;
        int fromX = move.path[i].col;
        int toY = move.path[i + 1].row;
        int toX = move.path[i + 1].col;

        int piece = board[fromY][fromX];
        board[toY][toX] = piece;
        board[fromY][fromX] = EMPTY_BOX;

        if (isJump && abs(toY - fromY) > 1 && abs(toY - fromY) == abs(toX - fromX)) {
            int dY = (toY - fromY) > 0 ? 1 : -1;
            int dX = (toX - fromX) > 0 ? 1 : -1;

            int y = fromY + dY;
            int x = fromX + dX;

            while (y != toY && x != toX) {
                int target = board[y][x];
                if ((piece == P1_PAWN || piece == P1_KING) && (target == P2_PAWN || target == P2_KING)) {
                    board[y][x] = EMPTY_BOX;
                    captures++;
                    break;
                } else if ((piece == P2_PAWN || piece == P2_KING) && (target == P1_PAWN || target == P1_KING)) {
                    board[y][x] = EMPTY_BOX;
                    captures++;
                    break;
                }
                y += dY;
                x += dX;
            }
        }

        // Promote if needed
        if (piece == P1_PAWN && toY == BOARD_SIZE - 1)
            board[toY][toX] = P1_KING;
        else if (piece == P2_PAWN && toY == 0)
            board[toY][toX] = P2_KING;
    }

    return isJump ? captures : 1;
}

void applyMove(int board[BOARD_SIZE][BOARD_SIZE], MoveChain move, int piece) {
    simulateMove(board, move, 1);
}

void findBestCapture(int board[BOARD_SIZE][BOARD_SIZE], int x, int y, int piece, MoveChain* current, MoveChain* best) {
    int found = 0;

    int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (int i = 0; i < 4; i++) {
        int dy = directions[i][0];
        int dx = directions[i][1];

        int j = 1; // step multiplier for kings
        while (1) {
            int midY = y + dy * j;
            int midX = x + dx * j;
            int toY = y + dy * (j + 1);
            int toX = x + dx * (j + 1);

            if (toY < 0 || toY >= BOARD_SIZE || toX < 0 || toX >= BOARD_SIZE)
                break;

            int target = board[midY][midX];
            int dest = board[toY][toX];

            int isEnemy =
                (piece == P1_PAWN || piece == P1_KING) ? (target == P2_PAWN || target == P2_KING)
                : (piece == P2_PAWN || piece == P2_KING) ? (target == P1_PAWN || target == P1_KING)
                : 0;

            if (isEnemy && dest == EMPTY_BOX) {
                // Ensure path to enemy is clear for kings
                int blocked = 0;
                if (piece == P1_KING || piece == P2_KING) {
                    for (int k = 1; k < j; k++) {
                        int checkY = y + dy * k;
                        int checkX = x + dx * k;
                        if (board[checkY][checkX] != EMPTY_BOX) {
                            blocked = 1;
                            break;
                        }
                    }
                }

                if (!blocked) {
                    int tempBoard[BOARD_SIZE][BOARD_SIZE];
                    copyBoard(board, tempBoard);

                    MoveChain next = *current;
                    next.path[next.length++] = (Coord){toY, toX};

                    simulateMove(tempBoard, next, 1);
                    findBestCapture(tempBoard, toX, toY, tempBoard[toY][toX], &next, best);

                    if (next.length > best->length) {
                        *best = next;
                        found = 1;
                    }
                }
            }

            // Pawns can only jump 2 steps
            if (piece == P1_PAWN || piece == P2_PAWN)
                break;

            j++; // kings keep scanning diagonally
        }
    }

    if (!found && current->length > best->length) {
        *best = *current;
    }
}

void cpuMakeMove(int board[BOARD_SIZE][BOARD_SIZE], int* PlayerTurn) {
    MoveChain bestCapture = { .length = 0 };
    MoveChain bestMove = { .length = 0 };

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            int piece = board[y][x];
            if (piece != P1_PAWN && piece != P1_KING) continue;

            MoveChain current = { .length = 1 };
            current.path[0] = (Coord){y, x};

            MoveChain result = { .length = 0 };
            findBestCapture(board, x, y, piece, &current, &result);

            if (result.length > bestCapture.length) {
                bestCapture = result;
            }

            // Check for normal moves only if no captures found yet
            if (bestCapture.length <= 1) {
                int dy[2] = {1, (piece == P1_KING) ? -1 : 0};
                int dx[2] = {-1, 1};

                for (int i = 0; i < 2; i++) {
                    if (dy[i] == 0) continue;
                    for (int j = 0; j < 2; j++) {
                        int ny = y + dy[i];
                        int nx = x + dx[j];

                        if (ny >= 0 && ny < BOARD_SIZE && nx >= 0 && nx < BOARD_SIZE &&
                            board[ny][nx] == EMPTY_BOX) {

                            MoveChain move = { .length = 2 };
                            move.path[0] = (Coord){y, x};
                            move.path[1] = (Coord){ny, nx};

                            // Only pick first valid fallback move
                            if (bestMove.length == 0)
                                bestMove = move;
                        }
                    }
                }
            }
        }
    }

    if (bestCapture.length > 1) {
        applyMove(board, bestCapture, board[bestCapture.path[0].row][bestCapture.path[0].col]);
    } else if (bestMove.length == 2) {
        applyMove(board, bestMove, board[bestMove.path[0].row][bestMove.path[0].col]);
    }

    *PlayerTurn = 2;
}

