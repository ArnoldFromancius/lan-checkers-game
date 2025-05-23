#include <stdio.h>
#include <raylib.h>
#include "../include/render.h"


int startMenu(){

}

void drawBoard(int offsetX, int offsetY, int cellSize, int selectedRow, int selectedCol, int Board[BOARD_SIZE_T][BOARD_SIZE_T]){

    // Draw the board
    for (int row = 0; row < BOARD_SIZE_T; row++)
    {
        for (int col = 0; col < BOARD_SIZE_T; col++)
        {
            Color squareColor = ((row + col) % 2 == 0) ? LIGHTGRAY : DARKBROWN_CUSTOM;
            int x = offsetX + col * cellSize;
            int y = offsetY + row * cellSize;
            DrawRectangle(x, y, cellSize, cellSize, squareColor);
        }
    }
}

void drawPieces(int offsetX, int offsetY, int cellSize, int selectedRow, int selectedCol, int Board[BOARD_SIZE_T][BOARD_SIZE_T]){
    for(int row=0; row<BOARD_SIZE_T; row++){
        for(int col=0; col<BOARD_SIZE_T; col++){
            int x = offsetX + col * cellSize;
            int y = offsetY + row * cellSize;
            if(Board[row][col]==1){ //Draw P1 pawns
                DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR, BLACK);
            }else if(Board[row][col]==2){ //Draw P2 pawns
                DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR, RED);
            }
            
            // This draws a yellow outline when a piece is selected
            if (row == selectedRow && col == selectedCol)
            {
                if ( Board[row][col] == 1 )
                {
                    DrawRectangleLinesEx((Rectangle){x + 2, y + 2, cellSize - 4, cellSize - 4}, 4, PINK);
                }else if( Board[row][col] == 2 ){
                    DrawRectangleLinesEx((Rectangle){x + 2, y + 2, cellSize - 4, cellSize - 4}, 4, YELLOW);
                }
            }
        }
    }
}

    