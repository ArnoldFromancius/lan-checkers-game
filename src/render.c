#include <stdio.h>
#include <raylib.h>
#include "../include/render.h"
#include "../include/game_logic.h"

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
            if(Board[row][col]==P1_PAWN){ //Draw P1 pawns
                DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR, BLACK);
            }else if(Board[row][col]==P2_PAWN){ //Draw P2 pawns
                DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR, RED);
            }else if(Board[row][col]==P1_KING){
                DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR, BLACK);
                // Gold outline around the king
                DrawCircleLines(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR + 2, GOLD);
                //text "K" for king
                DrawText("K", x + cellSize / 2 - cellSize / 6, y + cellSize / 2 - cellSize / 4, cellSize / 2.5, GOLD);
            }else if(Board[row][col]==P2_KING){
                DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR, RED);
                // Gold outline around the king
                DrawCircleLines(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR + 2, GOLD);
                //text "K" for king
                DrawText("K", x + cellSize / 2 - cellSize / 6, y + cellSize / 2 - cellSize / 4, cellSize / 2.5, GOLD);
            }
            
            // This draws a yellow outline when a piece is selected
            if (row == selectedRow && col == selectedCol)
            {
                if ( Board[row][col] == P1_PAWN || Board[row][col] == P1_KING )
                {
                    DrawRectangleLinesEx((Rectangle){x + 2, y + 2, cellSize - 4, cellSize - 4}, 4, PINK);
                }else if( Board[row][col] == P2_PAWN || Board[row][col] == P2_KING ){
                    DrawRectangleLinesEx((Rectangle){x + 2, y + 2, cellSize - 4, cellSize - 4}, 4, YELLOW);
                }
            }
        }
    }
}

void drawMenu(int selectedMenuOption) {
    int screenWidth = GetScreenWidth(); // Replace with your actual screen width
    int fontSizeTitle = 40;
    int fontSizeItem = 20;

    const char *title = "Classic Checkers";
    const char *lanText = "VS LAN";
    const char *cpuText = "VS CPU";
    const char *instructions = "Use [UP↑] [DOWN↓] to select, ENTER to start";

    // Centered title
    int titleWidth = MeasureText(title, fontSizeTitle);
    DrawText(title, (screenWidth - titleWidth) / 2, 100, fontSizeTitle, DARKGRAY);

    // Menu options
    Color lanColor = (selectedMenuOption == 0) ? RED : BLACK;
    Color cpuColor = (selectedMenuOption == 1) ? RED : BLACK;

    int lanWidth = MeasureText(lanText, fontSizeItem);
    int cpuWidth = MeasureText(cpuText, fontSizeItem);

    DrawText(lanText, (screenWidth - lanWidth) / 2, 200, fontSizeItem, lanColor);
    DrawText(cpuText, (screenWidth - cpuWidth) / 2, 250, fontSizeItem, cpuColor);

    // Instructions
    int instrWidth = MeasureText(instructions, fontSizeItem);
    DrawText(instructions, (screenWidth - instrWidth) / 2, 350, fontSizeItem, GRAY);
}
