
#ifndef RENDER_H
#define RENDER_H

#include "game_logic.h"

#define DARKBROWN_CUSTOM (Color){139, 69, 19, 255}
#define BACKGROUND_COLOR (Color){80, 30, 200, 155}
#define PIECE_RADIUS_FACTOR 0.4f



int startMenu();
void drawBoard(int offsetX, int offsetY, int cellSize, int selectedRow, int selectedCol, int Board[BOARD_SIZE][BOARD_SIZE]);
void drawPieces(int offsetX, int offsetY, int cellSize, int selectedRow, int selectedCol, int Board[BOARD_SIZE][BOARD_SIZE]);

#endif