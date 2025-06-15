#include <stdio.h>
#include <raylib.h>
#include "../include/render.h"
#include "../include/networking.h"
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
    int screenWidth = GetScreenWidth(); 
    int fontSizeTitle = 60;
    int fontSizeItem = 40;

    const char *title = "Select Play Mode:";
    const char *lanText = "VS LAN";
    const char *cpuText = "VS CPU";
    const char *instructions = "Use [UP↑] [DOWN↓] to select, ENTER to start";

    // title
    int titleWidth = MeasureText(title, fontSizeTitle);
    DrawText(title, (screenWidth - titleWidth) / 2, 350, fontSizeTitle, WHITE);

    // Menu options
    Color lanColor = (selectedMenuOption == 0) ? RED : WHITE;
    Color cpuColor = (selectedMenuOption == 1) ? RED : WHITE;

    int lanWidth = MeasureText(lanText, fontSizeItem);
    int cpuWidth = MeasureText(cpuText, fontSizeItem);

    DrawText(lanText, (screenWidth - lanWidth) / 2, 450, fontSizeItem, lanColor);
    DrawText(cpuText, (screenWidth - cpuWidth) / 2, 500, fontSizeItem, cpuColor);

    // Instructions
    int instrWidth = MeasureText(instructions, fontSizeItem);
    DrawText(instructions, (screenWidth - instrWidth) / 2, 700, fontSizeItem, YELLOW);
}

void drawLanMenu(int selectedMenuOption) {
    int screenWidth = GetScreenWidth(); 
    int fontSizeTitle = 60;
    int fontSizeItem = 40;

    const char *title = "Would you like to:";
    const char *lanText = "Host Game";
    const char *cpuText = "Connect to Server";
    const char *instructions = "Use ENTER to select, M for Menu ";

    // Centered title
    int titleWidth = MeasureText(title, fontSizeTitle);
    DrawText(title, (screenWidth - titleWidth) / 2, 350, fontSizeTitle, WHITE);

    // Menu options
    Color lanColor = (selectedMenuOption == 0) ? RED : WHITE;
    Color cpuColor = (selectedMenuOption == 1) ? RED : WHITE;

    int lanWidth = MeasureText(lanText, fontSizeItem);
    int cpuWidth = MeasureText(cpuText, fontSizeItem);

    DrawText(lanText, (screenWidth - lanWidth) / 2, 450, fontSizeItem, lanColor);
    DrawText(cpuText, (screenWidth - cpuWidth) / 2, 500, fontSizeItem, cpuColor);

    // Instructions
    int instrWidth = MeasureText(instructions, fontSizeItem);
    DrawText(instructions, (screenWidth - instrWidth) / 2, 700, fontSizeItem, YELLOW);
}

void drawLanServerConnect() {
    int screenWidth = GetScreenWidth(); 
    int fontSizeTitle = 60;
    int fontSizeItem = 40;

    const char *title = "HOSTiNG GAME";
    const char *lanText = "Waiting for a client to connect...";
    const char *cpuText = "Your IP: ";
    const char *instructions = "Use [ESC] to Quit, [M] for Menu ";

    // Centered title
    int titleWidth = MeasureText(title, fontSizeTitle);
    DrawText(title, (screenWidth - titleWidth) / 2, 350, fontSizeTitle, WHITE);

    // Menu options

    int lanWidth = MeasureText(lanText, fontSizeItem);
    int cpuWidth = MeasureText(cpuText, fontSizeItem);

    DrawText(lanText, (screenWidth - lanWidth) / 2, 450, fontSizeItem, WHITE);
    DrawText(cpuText, (screenWidth - cpuWidth) / 2, 530, fontSizeItem, WHITE);
    DrawText(get_local_ip(), ((screenWidth - cpuWidth) / 2)-30, 580, fontSizeItem, PINK);
    // Instructions
    int instrWidth = MeasureText(instructions, fontSizeItem);
    DrawText(instructions, (screenWidth - instrWidth) / 2, 700, fontSizeItem, YELLOW);
}

void drawLanClientConnect(char clientIpBuffer[]) {
    int screenWidth = GetScreenWidth(); 
    int fontSizeTitle = 60;
    int fontSizeItem = 40;

    const char *title = "Connect to server";
    const char *lanText = "Enter server IP address...";
    const char *instructions = "Use [ENTER], [ESC] to Quit, [M] for Menu ";

    // title
    int titleWidth = MeasureText(title, fontSizeTitle);
    DrawText(title, (screenWidth - titleWidth) / 2, 350, fontSizeTitle, WHITE);

    // Menu options

    int lanWidth = MeasureText(lanText, fontSizeItem);

    DrawText(lanText, (screenWidth - lanWidth) / 2, 450, fontSizeItem, WHITE);
    DrawText(clientIpBuffer, ((screenWidth - lanWidth) / 2)+200, 520, fontSizeItem, PINK);

    // Instructions
    int instrWidth = MeasureText(instructions, fontSizeItem);
    DrawText(instructions, (screenWidth - instrWidth) / 2, 700, fontSizeItem, YELLOW);
    
}
