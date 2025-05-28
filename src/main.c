#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "../include/game_logic.h"
#include "../include/render.h"
#include "../include/audio.h"
#include "../include/log.h"

int DEBUG_MODE = 0; // Set to 1 to enable debug logs

GameState currentState = MENU_STATE;
int gameMode = 0; // 1 = LAN, 2 = CPU
int selectedMenuOption = 0; // 0 = LAN, 1 = CPU
const int totalMenuOptions = 2;


//solution to track position of a selected piece. Struct to soon be used
typedef struct Selection{
	int flag;   //tracks piece selection options: EMPTY_BOX,P1_PAWN, P2_PAWN,king pieces etc...
    int row;    //row of selected piece
	int col;    //self explainatory... i think
}Selection;

int Board[BOARD_SIZE][BOARD_SIZE];

int main(){

    SetConfigFlags(FLAG_FULLSCREEN_MODE); // Always fullscreen
    InitWindow(0, 0, "Checkers");
    InitGameAudio(); //intitialize audio
    Music bgm = LoadMusicStream("assets/sounds/bg.mp3");
    PlayMusicStream(bgm);

    Texture2D background = LoadTexture("./assets/imgs/bg1.png");
    Texture2D splashscreen = LoadTexture("./assets/imgs/splash.png");

    log_info("#MAIN_FUNC raylib_window_initialized...");

    //setup needed game parameters
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int cellSize = (screenHeight < screenWidth) ? screenHeight/BOARD_SIZE : screenWidth/BOARD_SIZE;
    int offsetX = (screenWidth - (cellSize * BOARD_SIZE)) / 2;
    int offsetY = (screenHeight - (cellSize * BOARD_SIZE)) / 2;
    SetTargetFPS(60);
    initBoard(Board);
    log_info("#MAIN_FUNC board initilized...");

    //initialize selection
    Selection selectedPiece;
    selectedPiece.flag=EMPTY_BOX; //no selection.	
    selectedPiece.row= -1; 
    selectedPiece.col= -1;

    //Main game loop
    int winner;
    int row=-1,col=-1;
    while (!WindowShouldClose())
    {
        //play and loop song
        UpdateMusicStream(bgm);
        //Main menu
        if (currentState == MENU_STATE) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(splashscreen, 0, 0, WHITE);
            drawMenu(selectedMenuOption);
            EndDrawing();
            if (IsKeyPressed(KEY_DOWN)) {
                selectedMenuOption = (selectedMenuOption + 1) % totalMenuOptions;
                PlayMenuSoundMove();
            }
            if (IsKeyPressed(KEY_UP)) {
                selectedMenuOption = (selectedMenuOption - 1 + totalMenuOptions) % totalMenuOptions;
                PlayMenuSoundMove();
            }

            if (IsKeyPressed(KEY_ENTER)) {
                gameMode = selectedMenuOption + 1;
                currentState = GAME_STATE;
                PlayMenuSoundSelect();
            }

        }else if(currentState == GAME_STATE){
            BeginDrawing();
            //When a box is clicked
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mouse = GetMousePosition();
                col = (mouse.x - offsetX) / cellSize;
                row = (mouse.y - offsetY) / cellSize;
                log_info("\n#MAIN_FUNC selection made: X:%d Y:%d...",row,col);
                //Logic for steps to take if a box is clicked
                boxClicked(row, col, &selectedPiece.flag, &selectedPiece.row, &selectedPiece.col, Board);
                int winner = checkWinCondition(Board);
                if (winner == 1 || winner == 2) {
                    const char *message = (winner == 1) ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!";
                    log_info("%s", message);

                    // Display win message and wait for ENTER
                    while (!WindowShouldClose()) {
                        BeginDrawing();
                        ClearBackground(RAYWHITE);

                        DrawText(message, screenWidth / 2 - MeasureText(message, 40) / 2, screenHeight / 2 - 20, 40, GREEN);
                        DrawText("Press ENTER to restart, ESC to quit...", screenWidth / 2 - MeasureText("Press ENTER to restart, ESC to quit...", 20) / 2, screenHeight / 2 + 40, 20, DARKGRAY);

                        EndDrawing();

                        if (IsKeyPressed(KEY_ENTER)) {
                            resetGame(Board, &selectedPiece.flag, &selectedPiece.row, &selectedPiece.col);
                            break;
                        }else if (IsKeyPressed(KEY_ESCAPE)) {
                            CloseWindow();
                            exit(0);
                        }

                    }
                }

                log_board_state(Board);   
            }

            //Display changes 
            ClearBackground(BACKGROUND_COLOR);
            DrawTexture(background, 0, 0, WHITE);

            drawBoard(offsetX, offsetY, cellSize, row, col, Board);
            drawPieces(offsetX, offsetY, cellSize, selectedPiece.row, selectedPiece.col, Board);
            EndDrawing();
        }
        //Toggle Debug mode
        if (IsKeyPressed(KEY_F1)) {
            DEBUG_MODE = !DEBUG_MODE;
            log_info("Debug mode %s", DEBUG_MODE ? "enabled" : "disabled");
            
        }
    }
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}