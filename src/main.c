#include <stdio.h>
#include <raylib.h>
#include "../include/game_logic.h"
#include "../include/render.h"

int Board[BOARD_SIZE][BOARD_SIZE];

int main(){

    SetConfigFlags(FLAG_FULLSCREEN_MODE); // Always fullscreen
    InitWindow(0, 0, "Checkers");
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int cellSize = (screenHeight < screenWidth) ? screenHeight/BOARD_SIZE : screenWidth/BOARD_SIZE;
    int offsetX = (screenWidth - (cellSize * BOARD_SIZE)) / 2;
    int offsetY = (screenHeight - (cellSize * BOARD_SIZE)) / 2;
    
    SetTargetFPS(60);
    initBoard(Board);
    //Draw initial state of the board
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
    drawBoard(offsetX, offsetY, cellSize, -1, -1, Board);
    drawPieces(offsetX, offsetY, cellSize, -1, -1, Board);
    EndDrawing();

    //Main game loop
    int row=-1,col=-1;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        //When a box is clicked
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
             Vector2 mouse = GetMousePosition();
             col = (mouse.x - offsetX) / cellSize;
             row = (mouse.y - offsetY) / cellSize;
             //Logic for steps to take if a box is clicked
             boxClicked(row, col, Board);   
        }
        //Display changes 
        ClearBackground(BACKGROUND_COLOR);
        drawBoard(offsetX, offsetY, cellSize, row, col, Board);
        drawPieces(offsetX, offsetY, cellSize, row, col, Board);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}