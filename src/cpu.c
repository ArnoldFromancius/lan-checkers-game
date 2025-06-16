
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

        if (abs(toY - fromY) == 2) {
            int midY = (fromY + toY) / 2;
            int midX = (fromX + toX) / 2;
            board[midY][midX] = EMPTY_BOX;
            captures++;
        }

        if (piece == P1_PAWN && toY == BOARD_SIZE - 1)
            board[toY][toX] = P1_KING;
    }

    return isJump ? captures : 1;
}

void applyMove(int board[BOARD_SIZE][BOARD_SIZE], MoveChain move, int piece) {
    simulateMove(board, move, 1);
}

void findBestCapture(int board[BOARD_SIZE][BOARD_SIZE], int x, int y, int piece, MoveChain* current, MoveChain* best) {
    int found = 0;
    int directions[4][2] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};

    for (int i = 0; i < 4; i++) {
        int toY = y + directions[i][0];
        int toX = x + directions[i][1];

        int midY = y + directions[i][0] / 2;
        int midX = x + directions[i][1] / 2;

        if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE)
            continue;

        int target = board[midY][midX];
        int dest = board[toY][toX];

        if ((piece == P1_PAWN || piece == P1_KING) &&
            (target == P2_PAWN || target == P2_KING) &&
            dest == EMPTY_BOX) {

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

