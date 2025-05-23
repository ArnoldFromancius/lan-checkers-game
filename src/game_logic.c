#include <stdio.h>
#include<stdlib.h>
#include "../include/game_logic.h"

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
/*
void boxClicked(int col, int row, int *sPFlag, int *sPRow, int *sPCol, int Board[BOARD_SIZE][BOARD_SIZE]){
    // When a box is clicked
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
    {
        // Check if clicked on a piece
        if(Board[row][col] == P1_PAWN){ //clicked on a P1 pawn
            *sPFlag = P1_PAWN;
            *sPRow = row;
            *sPCol = col;

        }else if(Board[row][col] == P2_PAWN){ //clicked on P2 pawn
            *sPFlag = P2_PAWN;
            *sPRow = row;
            *sPCol = col;

        }//clicked on empty box
        else{ 
            //Move selected piece to the box clicked
            //check if P1 or P2 piece is selected
            if(*sPFlag == P1_PAWN){   //P1 pawn selected
                //movements are upwards 
                if(((*sPRow)-1 == row)&&(((*sPCol)-1)==col) ||
                   ((*sPRow)-1 == row)&&(((*sPCol)+1)==col)
                ){
                    Board[*sPRow][*sPCol] = EMPTY_BOX;
                    Board[row][col] = P1_PAWN;
                }
                
            }else if(*sPFlag == P2_PAWN){ //P2 pawn selected
                //movements are downwards  
                if(((*sPRow)+1 == row)&&(((*sPCol)-1)==col) ||
                   ((*sPRow)+1 == row)&&(((*sPCol)+1)==col)
                ){
                    Board[*sPRow][*sPCol] = EMPTY_BOX;
                    Board[row][col] = P2_PAWN;
                }  
            }
            *sPFlag = 0;
        }
        
    }
    
}
*/

void boxClicked(int col, int row, int *sPFlag, int *sPRow, int *sPCol, int Board[BOARD_SIZE][BOARD_SIZE]) {
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

        if (isValidMove(fromRow, fromCol, toRow, toCol, piece)) {
            Board[fromRow][fromCol] = EMPTY_BOX;
            Board[toRow][toCol] = piece;
        }
        else if (isValidJump(fromRow, fromCol, toRow, toCol, piece, Board)) {
            int midRow = (fromRow + toRow) / 2;
            int midCol = (fromCol + toCol) / 2;
            Board[fromRow][fromCol] = EMPTY_BOX;
            Board[midRow][midCol] = EMPTY_BOX;
            Board[toRow][toCol] = piece;
        }

        *sPFlag = EMPTY_BOX;
    }
}

int getPieceDirection(int piece) {
    if (piece == P1_PAWN) return -1; // P1 moves up
    if (piece == P2_PAWN) return 1;  // P2 moves down
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
    int dRow = toRow - fromRow;
    int dCol = toCol - fromCol;

    if (abs(dRow) == 2 && abs(dCol) == 2) {
        int midRow = (fromRow + toRow) / 2;
        int midCol = (fromCol + toCol) / 2;
        int middlePiece = Board[midRow][midCol];

        // Check opponent piece in the middle
        if (piece == P1_PAWN || piece == P1_KING) {
            if (middlePiece == P2_PAWN || middlePiece == P2_KING) {
                if (piece == P1_KING) return 1;
                return dRow == -2;
            }
        }
        if (piece == P2_PAWN || piece == P2_KING) {
            if (middlePiece == P1_PAWN || middlePiece == P1_KING) {
                if (piece == P2_KING) return 1;
                return dRow == 2;
            }
        }
    }

    return 0;
}

