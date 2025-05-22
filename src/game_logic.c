#include <stdio.h>
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

void boxClicked(int col, int row, int Board[BOARD_SIZE][BOARD_SIZE]){
    // When a box is clicked
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
    {
        // Check if clicked on a piece
        if(Board[row][col] == 1){ //clicked on a P1 pawn

            piece_selected = true;
        }else if(Board[row][col] == 2){ //clicked on P2 pawn

            piece_selected = true;
        }//clicked on empty box
        else{ 
            //Move selected piece to the box clicked
            if(piece_selected){
                
                piece_selected = false;
            }
        }
        
    }
    
}