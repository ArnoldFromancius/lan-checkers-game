#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include "../include/audio.h"
#include "../include/game_logic.h"
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

void boxClicked(int row, int col, int *sPFlag, int *sPRow, int *sPCol, int Board[BOARD_SIZE][BOARD_SIZE], int *playerTurn) {
    //for debugging purposed
    log_info("\n#BOX_CLICKED -");
    log_info("$highlighted_Piece: flag: %d row: %d col: %d",*sPFlag,*sPRow,*sPCol);
    log_info("$selected_piece: row: %d col: %d",row,col);

    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return;

    int clickedPiece = Board[row][col];
    //check if appropriate player's turn playing
    if (*playerTurn == 1) {
        if ((clickedPiece != P1_PAWN && clickedPiece != P1_KING) && (*sPFlag != P1_PAWN && *sPFlag != P1_KING)){
            return;
        }   
    } else if (*playerTurn == 2) {
        if ((clickedPiece != P2_PAWN && clickedPiece != P2_KING) && (*sPFlag != P2_PAWN && *sPFlag != P2_KING)){
            return;
        }
    }
   
    //Select a piece
    if ((clickedPiece == P1_PAWN || clickedPiece == P2_PAWN || 
        clickedPiece == P1_KING || clickedPiece == P2_KING)) {

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
        if (isValidMove(fromRow, fromCol, toRow, toCol, piece, Board)) {
            log_info("#ValidMove");
            Board[fromRow][fromCol] = EMPTY_BOX;
            Board[toRow][toCol] = piece;
            PlayMoveSound();
            tryPromoteToKing(toRow, toCol, Board);
            switchPlayer(playerTurn); 
        }
        else if (isValidJump(fromRow, fromCol, toRow, toCol, piece, Board)) {
            int midRow = (fromRow + toRow) / 2;
            int midCol = (fromCol + toCol) / 2;
            Board[fromRow][fromCol] = EMPTY_BOX;
            Board[midRow][midCol] = EMPTY_BOX;
            Board[toRow][toCol] = piece; 
            PlayCaptureSound();
            tryPromoteToKing(toRow, toCol, Board);
            // Check for more jumps
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
    
    if (board[toRow][toCol] == ILLEGAL_SQUARE)
        return 0; // cannot move to illegal square

    if (board[toRow][toCol] != EMPTY_BOX)
        return 0; // destination not empty

    int dRow = toRow - fromRow;
    int dCol = toCol - fromCol;

    if (abs(dRow) != 1 || abs(dCol) != 1)
        return 0; // must move one step diagonally

    int direction = getPieceDirection(piece);

    // If it's a pawn (not king), it can only move forward
    if (board[fromRow][fromCol] == piece && dRow != direction)
        return 0;

    return 1; // legal move
}

int isValidJump(int fromRow, int fromCol, int toRow, int toCol, int piece, int Board[BOARD_SIZE][BOARD_SIZE]) {
    int dRow = toRow - fromRow;
    int dCol = toCol - fromCol;

    if (abs(dRow) == 2 && abs(dCol) == 2) {
        int midRow = (fromRow + toRow) / 2;
        int midCol = (fromCol + toCol) / 2;
        int middlePiece = Board[midRow][midCol];

        if (Board[toRow][toCol] != EMPTY_BOX)
            return 0; // must land on empty square
        
        if (Board[toRow][toCol] == ILLEGAL_SQUARE)
            return 0; // cannot land on illegal square

        // P1 captures P2
        if ((piece == P1_PAWN || piece == P1_KING) &&
            (middlePiece == P2_PAWN || middlePiece == P2_KING)) {
            return 1;
        }

        // P2 captures P1
        if ((piece == P2_PAWN || piece == P2_KING) &&
            (middlePiece == P1_PAWN || middlePiece == P1_KING)) {
            return 1;
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

void switchPlayer(int *PlayerTurn){
    if(*PlayerTurn == 1) *PlayerTurn=2;
    else if(*PlayerTurn == 2) *PlayerTurn=1;
    log_info("current Player: %d", *PlayerTurn);
}

int canCapture(int x, int y, int piece, int Board[BOARD_SIZE][BOARD_SIZE]) {
    int directions[2] = { -2, 2 }; // Jump by 2
    for (int dy = 0; dy < 2; dy++) {
        for (int dx = 0; dx < 2; dx++) {
            int newX = x + directions[dx];
            int newY = y + directions[dy];

            if (isValidJump(y, x, newY, newX, piece, Board)) {
                return 1;
            }
        }
    }
    return 0;
}


void cpuMakeMove(int Board[BOARD_SIZE][BOARD_SIZE], int *PlayerTurn) {
    int captureMade = 0;

    // --- Check for captures first ---
    for (int sy = 0; sy < BOARD_SIZE; sy++) {
        for (int sx = 0; sx < BOARD_SIZE; sx++) {
            int piece = Board[sy][sx];
            if (piece == P1_PAWN || piece == P1_KING) {
                for (int dy = -2; dy <= 2; dy += 4) {
                    for (int dx = -2; dx <= 2; dx += 4) {
                        int dxDest = sx + dx;
                        int dyDest = sy + dy;

                        // 🛠 Fixed argument order
                        if (isValidJump(sy, sx, dyDest, dxDest, piece, Board)) {
                            // Execute capture
                            Board[dyDest][dxDest] = piece;
                            Board[sy][sx] = EMPTY_BOX;

                            // 🛠 Corrected mid capture coordinates
                            int midY = sy + (dyDest - sy) / 2;
                            int midX = sx + (dxDest - sx) / 2;
                            Board[midY][midX] = EMPTY_BOX;

                            // Kinging
                            if (dyDest == BOARD_SIZE - 1 && piece == P1_PAWN) {
                                Board[dyDest][dxDest] = P1_KING;
                            }

                            captureMade = 1;

                            // Continue multi-jump
                            int movedPiece = Board[dyDest][dxDest];
                            if (canCapture(dyDest, dxDest, movedPiece, Board)) {
                                cpuMakeMove(Board, PlayerTurn);
                            }else {
                                *PlayerTurn = 2; 
                            }
                        }
                    }
                }
            }
        }
    }

    // --- No captures available: try to make a smart move ---
    if (!captureMade) {
        int moveList[100][4];  // sx, sy, dx, dy
        int moveCount = 0;

        for (int sy = 0; sy < BOARD_SIZE; sy++) {
            for (int sx = 0; sx < BOARD_SIZE; sx++) {
                int piece = Board[sy][sx];
                if (piece == P1_PAWN || piece == P1_KING) {
                    int directions[2] = { -1, 1 };
                    for (int i = 0; i < 2; i++) {
                        int dx = directions[i];
                        int dy = (piece == P1_KING) ? directions[i] : 1;

                        int dxDest = sx + dx;
                        int dyDest = sy + dy;

                        // 🛠 Fixed argument order
                        if (isValidMove(sy, sx, dyDest, dxDest, piece, Board)) {
                            // Risk check (simplified)
                            int safe = 1;
                            for (int ey = -1; ey <= 1; ey += 2) {
                                for (int ex = -1; ex <= 1; ex += 2) {
                                    int oppX = dxDest + ex;
                                    int oppY = dyDest + ey;
                                    int jumpX = dxDest - ex;
                                    int jumpY = dyDest - ey;
                                    if (oppX >= 0 && oppX < BOARD_SIZE &&
                                        oppY >= 0 && oppY < BOARD_SIZE &&
                                        jumpX >= 0 && jumpX < BOARD_SIZE &&
                                        jumpY >= 0 && jumpY < BOARD_SIZE &&
                                        (Board[oppY][oppX] == P2_PAWN || Board[oppY][oppX] == P2_KING) &&
                                        Board[jumpY][jumpX] == EMPTY_BOX) {
                                        safe = 0;
                                    }
                                }
                            }

                            if (safe) {
                                moveList[moveCount][0] = sx;
                                moveList[moveCount][1] = sy;
                                moveList[moveCount][2] = dxDest;
                                moveList[moveCount][3] = dyDest;
                                moveCount++;
                            }
                        }
                    }
                }
            }
        }

        if (moveCount > 0) {
            // Pick move with deepest advance
            int bestIndex = 0;
            int maxRow = moveList[0][3];
            for (int i = 1; i < moveCount; i++) {
                if (moveList[i][3] > maxRow) {
                    maxRow = moveList[i][3];
                    bestIndex = i;
                }
            }

            int sx = moveList[bestIndex][0];
            int sy = moveList[bestIndex][1];
            int dx = moveList[bestIndex][2];
            int dy = moveList[bestIndex][3];

            Board[dy][dx] = Board[sy][sx];
            Board[sy][sx] = EMPTY_BOX;

            if (dy == BOARD_SIZE - 1 && Board[dy][dx] == P1_PAWN) {
                Board[dy][dx] = P1_KING;
            }

            *PlayerTurn = 2;
        }
    }

    log_board_state(Board);
}



/*
void handleGameOver(int winner, Texture2D background, int screenWidth, int screenHeight, int Board[BOARD_SIZE][BOARD_SIZE], Selection* selectedPiece) {
    const char *message1 = (winner == 1) ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!";
    const char *message2 =  "Press ENTER to restart, ESC to quit...";
    int message1FontSize = 50;
    int message2FontSize = 35;
    int message1Width = MeasureText(message1, message1FontSize);
    int message2Width = MeasureText(message2, message2FontSize);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);
        DrawText(message1, screenWidth/2 - message1Width/2, 400, message1FontSize, BLUE);
        DrawText(message2, screenWidth/2 - message2Width/2, 480, message2FontSize, WHITE);
        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)) {
            resetGame(Board, &selectedPiece->flag, &selectedPiece->row, &selectedPiece->col);
            initBoard(Board);
            break;
        } else if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
            exit(0);
        }
    }
}
    */
