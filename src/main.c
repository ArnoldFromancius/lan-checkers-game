#include <stdio.h>
#include <raylib.h>

#define BOARD_SIZE 8
#define DARKBROWN (Color){139, 69, 19, 255}
#define SILVER (Color){80, 30, 200, 155}
#define PIECE_RADIUS_FACTOR 0.4f

int main(void)
{
    SetConfigFlags(FLAG_FULLSCREEN_MODE); // Always fullscreen
    InitWindow(0, 0, "Checkers - Responsive");

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int cellSize;
    if (screenHeight < screenWidth)
    {
        cellSize = screenHeight / BOARD_SIZE;
    }
    else
    {
        cellSize = screenWidth / BOARD_SIZE;
    }

    int offsetX = (screenWidth - (cellSize * BOARD_SIZE)) / 2;
    int offsetY = (screenHeight - (cellSize * BOARD_SIZE)) / 2;
    int selectedRow = -1;
    int selectedCol = -1;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SILVER);

        // Draw the board
        for (int row = 0; row < BOARD_SIZE; row++)
        {
            for (int col = 0; col < BOARD_SIZE; col++)
            {
                Color squareColor = ((row + col) % 2 == 0) ? LIGHTGRAY : DARKBROWN;
                int x = offsetX + col * cellSize;
                int y = offsetY + row * cellSize;
                DrawRectangle(x, y, cellSize, cellSize, squareColor);

                // This draws a yellow outline when a piece is selected
                if (row == selectedRow && col == selectedCol)
                {
                    DrawRectangleLinesEx((Rectangle){x + 2, y + 2, cellSize - 4, cellSize - 4}, 4, YELLOW);
                }

                // Draw pieces(only draw on dark squares)
                if ((row + col) % 2 != 0)
                {
                    if (row < 3)
                    {
                        DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR, RED);
                    }
                    else if (row >= 5)
                    {
                        DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize * PIECE_RADIUS_FACTOR, BLACK);
                    }
                }
            }
        }
        // When a piece is clicked
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();
            int col = (mouse.x - offsetX) / cellSize;
            int row = (mouse.y - offsetY) / cellSize;

            if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
            {
                // Check if clicked on a piece
                if ((row + col) % 2 != 0 && (row < 3 || row >= 5))
                {
                    selectedRow = row;
                    selectedCol = col;
                }
                else
                {
                    selectedRow = -1; // Deselect if clicking elsewhere
                    selectedCol = -1;
                }
            }
        }
        EndDrawing();
    }
CloseWindow();
return 0;
}