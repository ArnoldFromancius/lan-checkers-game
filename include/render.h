
#ifndef RENDER_H
#define RENDER_H


#define DARKBROWN_CUSTOM (Color){139, 69, 19, 255}
#define BACKGROUND_COLOR (Color){80, 30, 200, 155}
#define PIECE_RADIUS_FACTOR 0.4f
#define BOARD_SIZE_T 8



int startMenu();
void drawBoard(int offsetX, int offsetY, int cellSize, int selectedRow, int selectedCol, int Board[BOARD_SIZE_T][BOARD_SIZE_T]);
void drawPieces(int offsetX, int offsetY, int cellSize, int selectedRow, int selectedCol, int Board[BOARD_SIZE_T][BOARD_SIZE_T]);

#endif