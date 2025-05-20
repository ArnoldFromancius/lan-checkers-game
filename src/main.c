#include<stdio.h>
#include<raylib.h>

#define BOARD_SIZE 8
#define DARKBROWN (Color){139, 69, 19, 255}
#define SILVER (Color){80, 30, 200, 155}

int main(void) {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);  // Always fullscreen
    InitWindow(0, 0, "Checkers - Responsive");

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int cellSize;
    if (screenHeight < screenWidth){
	    cellSize = screenHeight / BOARD_SIZE;
    }
    else{
	    cellSize = screenWidth /BOARD_SIZE;
    }

    int offsetX = (screenWidth - (cellSize * BOARD_SIZE)) / 2;
    int offsetY = (screenHeight - (cellSize * BOARD_SIZE)) / 2;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SILVER);

        // Draw the board
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                Color squareColor = ((row + col) % 2 == 0) ? LIGHTGRAY : DARKBROWN;
            	DrawRectangle(offsetX + col * cellSize, offsetY + row * cellSize, cellSize, cellSize, squareColor);

	    }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

