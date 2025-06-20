#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include "../include/audio.h"
#include "../include/game_logic.h"
#include "../include/networking.h"
#include "../include/log.h"

void initCpu() {
    srand(time(NULL));
}

void initBoard(int Board[BOARD_SIZE][BOARD_SIZE]){
    int row = 0, col;
    while(row < BOARD_SIZE){
        col = 0;
        while(col < BOARD_SIZE){
            if ((row + col) % 2 != 0) {
                // Dark squares — playable
                if (row < 3) {          // P1 pieces start rows
                    Board[row][col] = P1_PAWN;
                }
                else if (row >= 5) {    // P2 pieces start rows
                    Board[row][col] = P2_PAWN;
                }
                else {
                    Board[row][col] = EMPTY_BOX; // Empty playable square
                }
            } else {
                // Light squares — illegal
                Board[row][col] = ILLEGAL_SQUARE;
            }
            col++;
        }
        row++;
    }
}
/*
void boxClicked(int row, int col, int *sPFlag, int *sPRow, int *sPCol, int Board[BOARD_SIZE][BOARD_SIZE], int *playerTurn) {
    log_info("\n#BOX_CLICKED -");
    log_info("$highlighted_Piece: flag: %d row: %d col: %d", *sPFlag, *sPRow, *sPCol);
    log_info("$selected_piece: row: %d col: %d", row, col);

    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return;

    int clickedPiece = Board[row][col];

    // Check if appropriate player's piece is selected or already in progress
    if (*playerTurn == 1) {
        if ((clickedPiece != P1_PAWN && clickedPiece != P1_KING) && (*sPFlag != P1_PAWN && *sPFlag != P1_KING)) {
            return;
        }
    } else if (*playerTurn == 2) {
        if ((clickedPiece != P2_PAWN && clickedPiece != P2_KING) && (*sPFlag != P2_PAWN && *sPFlag != P2_KING)) {
            return;
        }
    }

    // Select a piece
    if ((clickedPiece == P1_PAWN || clickedPiece == P2_PAWN ||
         clickedPiece == P1_KING || clickedPiece == P2_KING)) {

        *sPFlag = clickedPiece;
        *sPRow = row;
        *sPCol = col;
        return;
    }

    // Attempt move or jump
    if (*sPFlag != EMPTY_BOX) {
        int fromRow = *sPRow;
        int fromCol = *sPCol;
        int toRow = row;
        int toCol = col;
        int piece = *sPFlag;

        if (isValidMove(fromRow, fromCol, toRow, toCol, piece, Board)) {
            log_info("#ValidMove");
            Board[fromRow][fromCol] = EMPTY_BOX;
            Board[toRow][toCol] = piece;
            PlayMoveSound();
            tryPromoteToKing(toRow, toCol, Board);
            switchPlayer(playerTurn);
        }
        else if (isValidJump(fromRow, fromCol, toRow, toCol, piece, Board)) {
            // Clear origin
            Board[fromRow][fromCol] = EMPTY_BOX;

            // Find and remove the jumped opponent (works for flying kings)
            int dRow = (toRow > fromRow) ? 1 : -1;
            int dCol = (toCol > fromCol) ? 1 : -1;
            int r = fromRow + dRow;
            int c = fromCol + dCol;

            while (r != toRow && c != toCol) {
                int midPiece = Board[r][c];
                if ((piece == P1_PAWN || piece == P1_KING) && (midPiece == P2_PAWN || midPiece == P2_KING)) {
                    Board[r][c] = EMPTY_BOX;
                    break;
                }
                if ((piece == P2_PAWN || piece == P2_KING) && (midPiece == P1_PAWN || midPiece == P1_KING)) {
                    Board[r][c] = EMPTY_BOX;
                    break;
                }
                r += dRow;
                c += dCol;
            }

            // Place piece at new location
            Board[toRow][toCol] = piece;
            PlayCaptureSound();
            tryPromoteToKing(toRow, toCol, Board);

            // Check for further jumps
            if (hasMoreJumps(toRow, toCol, piece, Board)) {
                *sPRow = toRow;
                *sPCol = toCol;
                *sPFlag = piece;
            } else {
                *sPRow = -1;
                *sPCol = -1;
                *sPFlag = EMPTY_BOX;
                switchPlayer(playerTurn);
            }
        }
    }
}
*/
void boxClicked(int row, int col, int *sPFlag, int *sPRow, int *sPCol,
                int Board[BOARD_SIZE][BOARD_SIZE], int *playerTurn, MovePacket *outMove)
{
    log_info("\n#BOX_CLICKED -");
    log_info("$highlighted_Piece: flag: %d row: %d col: %d", *sPFlag, *sPRow, *sPCol);
    log_info("$selected_piece: row: %d col: %d", row, col);

    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return;

    int clickedPiece = Board[row][col];

    if (*playerTurn == 1) {
        if ((clickedPiece != P1_PAWN && clickedPiece != P1_KING) &&
            (*sPFlag != P1_PAWN && *sPFlag != P1_KING)) {
            return;
        }
    } else if (*playerTurn == 2) {
        if ((clickedPiece != P2_PAWN && clickedPiece != P2_KING) &&
            (*sPFlag != P2_PAWN && *sPFlag != P2_KING)) {
            return;
        }
    }

    if ((clickedPiece == P1_PAWN || clickedPiece == P2_PAWN ||
         clickedPiece == P1_KING || clickedPiece == P2_KING)) {
        *sPFlag = clickedPiece;
        *sPRow = row;
        *sPCol = col;
        return;
    }

    if (*sPFlag != EMPTY_BOX) {
        int fromRow = *sPRow;
        int fromCol = *sPCol;
        int toRow = row;
        int toCol = col;
        int piece = *sPFlag;

        if (isValidMove(fromRow, fromCol, toRow, toCol, piece, Board)) {
            log_info("#ValidMove");
            Board[fromRow][fromCol] = EMPTY_BOX;
            Board[toRow][toCol] = piece;
            PlayMoveSound();
            tryPromoteToKing(toRow, toCol, Board);

            if (outMove) {
                outMove->fromRow = fromRow;
                outMove->fromCol = fromCol;
                outMove->toRow = toRow;
                outMove->toCol = toCol;
                outMove->isCapture = 0;
                outMove->flag = 1;
                outMove->moreJumps = 0;
            }

            switchPlayer(playerTurn);
        }
        else if (isValidJump(fromRow, fromCol, toRow, toCol, piece, Board)) {
            Board[fromRow][fromCol] = EMPTY_BOX;

            int dRow = (toRow > fromRow) ? 1 : -1;
            int dCol = (toCol > fromCol) ? 1 : -1;
            int r = fromRow + dRow;
            int c = fromCol + dCol;

            while (r != toRow && c != toCol) {
                int midPiece = Board[r][c];
                if ((piece == P1_PAWN || piece == P1_KING) &&
                    (midPiece == P2_PAWN || midPiece == P2_KING)) {
                    Board[r][c] = EMPTY_BOX;
                    break;
                }
                if ((piece == P2_PAWN || piece == P2_KING) &&
                    (midPiece == P1_PAWN || midPiece == P1_KING)) {
                    Board[r][c] = EMPTY_BOX;
                    break;
                }
                r += dRow;
                c += dCol;
            }

            Board[toRow][toCol] = piece;
            PlayCaptureSound();
            tryPromoteToKing(toRow, toCol, Board);

            if (outMove) {
                outMove->fromRow = fromRow;
                outMove->fromCol = fromCol;
                outMove->toRow = toRow;
                outMove->toCol = toCol;
                outMove->isCapture = 1;
                outMove->flag = 1;
                outMove->moreJumps = hasMoreJumps(toRow, toCol, piece, Board) ? 1 : 0;
            }

            if (hasMoreJumps(toRow, toCol, piece, Board)) {
                *sPRow = toRow;
                *sPCol = toCol;
                *sPFlag = piece;
            } else {
                *sPRow = -1;
                *sPCol = -1;
                *sPFlag = EMPTY_BOX;
                switchPlayer(playerTurn);
            }
        }
    }
}

void applyMovePacket(int Board[BOARD_SIZE][BOARD_SIZE], MovePacket *move) {
    int fromRow = move->fromRow;
    int fromCol = move->fromCol;
    int toRow = move->toRow;
    int toCol = move->toCol;
    int piece = Board[fromRow][fromCol];

    // Clear original position
    Board[fromRow][fromCol] = EMPTY_BOX;

    // If it was a capture, remove opponent
    if (move->isCapture) {
        int dRow = (toRow > fromRow) ? 1 : -1;
        int dCol = (toCol > fromCol) ? 1 : -1;
        int r = fromRow + dRow;
        int c = fromCol + dCol;

        while (r != toRow && c != toCol) {
            int midPiece = Board[r][c];

            if ((piece == P1_PAWN || piece == P1_KING) && (midPiece == P2_PAWN || midPiece == P2_KING)) {
                Board[r][c] = EMPTY_BOX;
                break;
            }
            if ((piece == P2_PAWN || piece == P2_KING) && (midPiece == P1_PAWN || midPiece == P1_KING)) {
                Board[r][c] = EMPTY_BOX;
                break;
            }

            r += dRow;
            c += dCol;
        }
        PlayCaptureSound();
    } else {
        PlayMoveSound();
    }

    // Place piece at destination
    Board[toRow][toCol] = piece;

    // Try king promotion
    tryPromoteToKing(toRow, toCol, Board);
}

int checkWinCondition(int Board[BOARD_SIZE][BOARD_SIZE]) {
    int p1HasPieces = countPieces(1, Board);
    int p2HasPieces = countPieces(2, Board);
    int p1HasMoves = hasValidMoves(1, Board);
    int p2HasMoves = hasValidMoves(2, Board);

    if (p2HasPieces == 0 || p2HasMoves == 0) {
        return 1; // Player 1 wins
    } else if (p1HasPieces == 0 || p1HasMoves == 0) {
        return 2; // Player 2 wins
    }

    return 0; // No winner yet
}

int getPieceDirection(int piece) {
    if (piece == P1_PAWN) return 1; // P1 moves down
    if (piece == P2_PAWN) return -1;  // P2 moves up
    return 0; // kings handled separately
}

int isValidMove(int fromRow, int fromCol, int toRow, int toCol, int piece, int board[BOARD_SIZE][BOARD_SIZE]) {
    if (toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE)
        return 0; // off board
    
    if (board[toRow][toCol] == ILLEGAL_SQUARE || board[toRow][toCol] != EMPTY_BOX)
        return 0; // can't move to illegal or occupied square

    int dRow = toRow - fromRow;
    int dCol = toCol - fromCol;

    if (abs(dRow) != abs(dCol))
        return 0; // must move diagonally

    int direction = getPieceDirection(piece);

    // Normal pawns: only one step forward
    if ((piece == P1_PAWN || piece == P2_PAWN)) {
        if (abs(dRow) != 1 || abs(dCol) != 1)
            return 0;
        if (dRow != direction)
            return 0;
        return 1;
    }

    // Kings: can fly diagonally if path is clear
    if (piece == P1_KING || piece == P2_KING) {
        int stepY = (dRow > 0) ? 1 : -1;
        int stepX = (dCol > 0) ? 1 : -1;

        int y = fromRow + stepY;
        int x = fromCol + stepX;
        while (y != toRow && x != toCol) {
            if (board[y][x] != EMPTY_BOX)
                return 0; // path blocked
            y += stepY;
            x += stepX;
        }
        return 1;
    }

    return 0;
}

int isValidJump(int fromRow, int fromCol, int toRow, int toCol, int piece, int Board[BOARD_SIZE][BOARD_SIZE]) {
    if (toRow < 0 || toRow >= BOARD_SIZE || toCol < 0 || toCol >= BOARD_SIZE)
        return 0;

    if (Board[toRow][toCol] != EMPTY_BOX || Board[toRow][toCol] == ILLEGAL_SQUARE)
        return 0;

    int dRow = toRow - fromRow;
    int dCol = toCol - fromCol;

    if (abs(dRow) != abs(dCol))
        return 0; // must move diagonally

    int stepY = (dRow > 0) ? 1 : -1;
    int stepX = (dCol > 0) ? 1 : -1;

    int y = fromRow + stepY;
    int x = fromCol + stepX;
    int opponentFound = 0;

    while (y != toRow && x != toCol) {
        int cell = Board[y][x];

        if (cell == ILLEGAL_SQUARE) return 0;

        // Check for opponents or blocking
        if (piece == P1_PAWN || piece == P1_KING) {
            if (cell == P2_PAWN || cell == P2_KING) {
                opponentFound++;
                if (opponentFound > 1) return 0;
            } else if (cell != EMPTY_BOX) {
                return 0; // blocked by own piece
            }
        } else if (piece == P2_PAWN || piece == P2_KING) {
            if (cell == P1_PAWN || cell == P1_KING) {
                opponentFound++;
                if (opponentFound > 1) return 0;
            } else if (cell != EMPTY_BOX) {
                return 0; // blocked by own piece
            }
        }

        y += stepY;
        x += stepX;
    }

    // Pawns: only 2-step jump with 1 enemy
    if ((piece == P1_PAWN || piece == P2_PAWN) && abs(dRow) == 2 && opponentFound == 1)
        return 1;

    // Kings: can jump multiple squares but only over one opponent
    if ((piece == P1_KING || piece == P2_KING) && opponentFound == 1)
        return 1;

    return 0;
}

int hasMoreJumps(int row, int col, int piece, int Board[BOARD_SIZE][BOARD_SIZE]) {
    int directions[4][2] = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (int i = 0; i < 4; i++) {
        int dy = directions[i][0];
        int dx = directions[i][1];

        int y = row + dy;
        int x = col + dx;

        // For pawns, check only one jump in that direction (2 tiles away)
        if (piece == P1_PAWN || piece == P2_PAWN) {
            int jumpRow = row + dy * 2;
            int jumpCol = col + dx * 2;

            if (jumpRow >= 0 && jumpRow < BOARD_SIZE && jumpCol >= 0 && jumpCol < BOARD_SIZE) {
                if (isValidJump(row, col, jumpRow, jumpCol, piece, Board))
                    return 1;
            }
        } else {
            // For kings: scan in all diagonal directions for any valid jump
            while (y >= 0 && y < BOARD_SIZE && x >= 0 && x < BOARD_SIZE) {
                if (abs(y - row) >= 2 && isValidJump(row, col, y, x, piece, Board))
                    return 1;

                y += dy;
                x += dx;
            }
        }
    }

    return 0;
}

int hasValidMoves(int player, int Board[BOARD_SIZE][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            int piece = Board[row][col];

            if ((player == 1 && (piece == P1_PAWN || piece == P1_KING)) ||
                (player == 2 && (piece == P2_PAWN || piece == P2_KING))) {

                for (int dr = -2; dr <= 2; dr++) {
                    for (int dc = -2; dc <= 2; dc++) {
                        if (dr == 0 || dc == 0 || abs(dr) != abs(dc)) continue;
                        int newRow = row + dr;
                        int newCol = col + dc;

                        if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE)
                            continue;

                        if (Board[newRow][newCol] != EMPTY_BOX)
                            continue;

                        if (abs(dr) == 1) {
                            if (isValidMove(row, col, newRow, newCol, piece, Board))
                                return 1;
                        } else if (abs(dr) == 2) {
                            if (isValidJump(row, col, newRow, newCol, piece, Board))
                                return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int countPieces(int player, int Board[BOARD_SIZE][BOARD_SIZE]) {
    int count = 0;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            int piece = Board[row][col];
            if ((player == 1 && (piece == P1_PAWN || piece == P1_KING)) ||
                (player == 2 && (piece == P2_PAWN || piece == P2_KING))) {
                count++;
            }
        }
    }
    return count;
}

void tryPromoteToKing(int row, int col, int Board[BOARD_SIZE][BOARD_SIZE]) {
    int piece = Board[row][col];
    if (piece == P1_PAWN && row == 7) {
        Board[row][col] = P1_KING;
        log_info("#KINGING: P1 pawn at (%d, %d) promoted to KING", row, col);
        PlayKingSound();
    } else if (piece == P2_PAWN && row == 0) {
        Board[row][col] = P2_KING;
        log_info("#KINGING: P2 pawn at (%d, %d) promoted to KING", row, col);
        PlayKingSound();
    }
}

void resetGame(int Board[BOARD_SIZE][BOARD_SIZE], int *sPFlag, int *sPRow, int *sPCol) {
    initBoard(Board);
    *sPFlag = 0;
    *sPRow = -1;
    *sPCol = -1;
}

int countPawns(int board[BOARD_SIZE][BOARD_SIZE], int playerPawn) {
    int count = 0;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (board[y][x] == playerPawn) {
                count++;
            }
        }
    }
    return count;
}

int countKings(int board[BOARD_SIZE][BOARD_SIZE], int playerKing) {
    int count = 0;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (board[y][x] == playerKing) {
                count++;
            }
        }
    }
    return count;
}

void switchPlayer(int *PlayerTurn){
    if(*PlayerTurn == 1) *PlayerTurn=2;
    else if(*PlayerTurn == 2) *PlayerTurn=1;
    log_info("current Player: %d", *PlayerTurn);
}

