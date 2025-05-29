#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "../include/audio.h"
#include "../include/game_logic.h"
#include "../include/log.h"

void initBoard(int Board[BOARD_SIZE][BOARD_SIZE]){
    int row=0,col;
    while(row<8){
        col=0;
        while(col<8){
            // Set default position of pieces on board
            if ((row + col) % 2 != 0)
            {
                if (row < 3) //P1 pieces
                {
                    Board[row][col] = P1_PAWN;
                }
                else if (row >= 5) //P2 pieces
                {
                    Board[row][col] = P2_PAWN;
                }
            }
            //Set remaining boxes to empty
            else{
                Board[row][col]=EMPTY_BOX;
            }
            col++;
        }
        row++;
    }
}

void boxClicked(int row, int col, int *sPFlag, int *sPRow, int *sPCol, int Board[BOARD_SIZE][BOARD_SIZE]) {
    //for debugging purposed
    log_info("\n#BOX_CLICKED -");
    log_info("$highlighted_Piece: flag: %d row: %d col: %d",*sPFlag,*sPRow,*sPCol);
    log_info("$selected_piece: row: %d col: %d",row,col);

    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return;

    int clickedPiece = Board[row][col];

    // Select a piece
    if (clickedPiece == P1_PAWN || clickedPiece == P2_PAWN || 
        clickedPiece == P1_KING || clickedPiece == P2_KING) {
        *sPFlag = clickedPiece;
        *sPRow = row;
        *sPCol = col;
        return;
    }

    // Attempt move
    if (*sPFlag != EMPTY_BOX) {
        int fromRow = *sPRow;
        int fromCol = *sPCol;
        int toRow = row;
        int toCol = col;
        int piece = *sPFlag;
        int tmp;
        if (isValidMove(fromRow, fromCol, toRow, toCol, piece)) {
            log_info("#ValidMove");
            Board[fromRow][fromCol] = EMPTY_BOX;
            Board[toRow][toCol] = piece;
            PlayMoveSound();
            tryPromoteToKing(toRow, toCol, Board); 
        }
        else if (isValidJump(fromRow, fromCol, toRow, toCol, piece, Board)) {
            int midRow = (fromRow + toRow) / 2;
            int midCol = (fromCol + toCol) / 2;
            Board[fromRow][fromCol] = EMPTY_BOX;
            Board[midRow][midCol] = EMPTY_BOX;
            Board[toRow][toCol] = piece;  
            PlayCaptureSound();
            tryPromoteToKing(toRow, toCol, Board); 
        }
          
        // Check for more jumps
        if (hasMoreJumps(toRow, toCol, piece, Board)) {
            *sPRow = toRow;
            *sPCol = toCol;
            *sPFlag = piece;
        } else {
            *sPRow = -1;
            *sPCol = -1;
            *sPFlag = EMPTY_BOX;
        }
    }
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

int isValidMove(int fromRow, int fromCol, int toRow, int toCol, int piece) {
    int dRow = toRow - fromRow;
    int dCol = toCol - fromCol;

    // Regular movement
    if (abs(dRow) == 1 && abs(dCol) == 1) {
        if (piece == P1_KING || piece == P2_KING)
            return 1; // kings can move in both directions
        else
            return dRow == getPieceDirection(piece);
    }

    return 0;
}

int isValidJump(int fromRow, int fromCol, int toRow, int toCol, int piece, int Board[BOARD_SIZE][BOARD_SIZE]) {
    log_info("#INSIDE_ValidJump");
    int dRow = toRow - fromRow;
    int dCol = toCol - fromCol;

    if (abs(dRow) == 2 && abs(dCol) == 2) {
        int midRow = (fromRow + toRow) / 2;
        int midCol = (fromCol + toCol) / 2;
        int middlePiece = Board[midRow][midCol];

        // P1 captures P2
        if ((piece == P1_PAWN || piece == P1_KING) &&
            (middlePiece == P2_PAWN || middlePiece == P2_KING)) {
            return 1;  // allow any jump direction
        }

        // P2 captures P1
        if ((piece == P2_PAWN || piece == P2_KING) &&
            (middlePiece == P1_PAWN || middlePiece == P1_KING)) {
            return 1;  // allow any jump direction
        }
    }

    return 0;
}

int hasMoreJumps(int row, int col, int piece, int Board[BOARD_SIZE][BOARD_SIZE]) {
    int dirs[4][2] = {
        {-2, -2}, {-2, 2}, {2, -2}, {2, 2}
    };

    for (int i = 0; i < 4; i++) {
        int newRow = row + dirs[i][0];
        int newCol = col + dirs[i][1];

        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
            if (isValidJump(row, col, newRow, newCol, piece, Board)) {
                return 1;
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
                            if (isValidMove(row, col, newRow, newCol, piece))
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
    // Reset more state here if needed
}

