#include<stdio.h>
#include<raylib.h>

#define BOARD_SIZE 8
#define DARKBROWN (Color){139, 69, 19, 255}
#define SILVER (Color){80, 30, 200, 155}
#define PIECE_RADIUS_FACTOR 0.4f

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
            	int x=offsetX + col * cellSize;
		int y=offsetY + row * cellSize;
		DrawRectangle(x, y, cellSize, cellSize, squareColor);
		
		// Piece logic (only draw on dark squares)
                if ((row + col) % 2 != 0) {
                    if (row < 3) {
                        DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR, RED);
                    } else if (row >= 5) {
                        DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR, BLACK);
                    }
                }
	    }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

