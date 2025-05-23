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

void boxClicked(int col, int row, int *sPFlag, int *sPRow, int *sPCol, int Board[BOARD_SIZE][BOARD_SIZE]){
    // When a box is clicked
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
    {
        // Check if clicked on a piece
        if(*sPFlag== P1_PAWN){ //clicked on a P1 pawn
            *sPFlag = P1_PAWN;
            *sPRow = row;
            *sPCol = col;

        }else if(*sPFlag == P2_PAWN){ //clicked on P2 pawn
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
                    printf("%d %d",*sPRow,*sPCol);
                }
                
            }else if(*sPFlag == P2_PAWN){ //P2 pawn selected
                //movements are downwards  
                if(((*sPRow)+1 == row)&&(((*sPCol)-1)==col) ||
                   ((*sPRow)+1 == row)&&(((*sPCol)+1)==col)
                ){
                    Board[*sPRow][*sPCol] = EMPTY_BOX;
                    Board[row][col] = P2_PAWN;
                    printf("%d %d",*sPRow,*sPCol);
                }  
            }
            *sPFlag = 0;
        }
        
    }
    
}