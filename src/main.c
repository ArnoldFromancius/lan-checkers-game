#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <raylib.h>
#include "../include/game_logic.h"
#include "../include/networking.h"
#include "../include/render.h"
#include "../include/audio.h"
#include "../include/cpu.h"
#include "../include/log.h"

int DEBUG_MODE = 0; // Set to 1 to enable debug logs

GameState currentState = MENU_STATE;
int gameMode = 0; // 1 = LAN, 2 = CPU
int selectedMenuOption = 0; // 0 = LAN, 1 = CPU
int selectedLanOption = 0; // 0 = server, 1 = client
bool isServer = false;
const int totalMenuOptions = 2;

//setup ip address input
char clientIpBuffer[MAX_IP_LEN] = "127.0.0.1";

//solution to track position of a selected piece. 
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
    bool musicPlaying = true;
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
    int PlayerTurn=2;
    int row=-1,col=-1;
    while (!WindowShouldClose())
    {
        //play and loop songif (IsKeyPressed(KEY_M))
        if (IsKeyPressed(KEY_SPACE)){
            if (musicPlaying)
            {
                StopMusicStream(bgm);
                musicPlaying = false;
            }
            else
            {
                PlayMusicStream(bgm);
                musicPlaying = true;
            }
        }

        if (musicPlaying)
        {
            UpdateMusicStream(bgm);
        }

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
                if(selectedMenuOption == 0){ //lan play
                    currentState = NETWORK_SETUP_STATE;
                }else{
                    currentState = GAME_STATE;
                }
                PlayMenuSoundSelect();
            }

        }else if(currentState == GAME_STATE){
                
            if (gameMode == 2) { //cpu mode
                BeginDrawing();
                //When a box is clicked
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    Vector2 mouse = GetMousePosition();
                    col = (mouse.x - offsetX) / cellSize;
                    row = (mouse.y - offsetY) / cellSize;
                    log_info("\n#MAIN_FUNC selection made: X:%d Y:%d...",row,col);
                    //Logic for steps to take if a box is clicked
                    boxClicked(row, col, &selectedPiece.flag, &selectedPiece.row, &selectedPiece.col, Board, &PlayerTurn);
                    int winner = checkWinCondition(Board);
                    if (winner == 1 || winner == 2) {
                        const char *message1 = (winner == 1) ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!";
                        const char *message2 =  "Press ENTER to restart, ESC to quit...";
                        
                        int message1FontSize = 50;
                        int message2FontSize = 35;
                        int message1Width = MeasureText(message1, message1FontSize);
                        int message2Width = MeasureText(message2, message2FontSize);
    
                        // Display win message and wait for ENTER
                        while (!WindowShouldClose()) {
                            BeginDrawing();
                            ClearBackground(RAYWHITE);
                            DrawTexture(background, 0, 0, WHITE);
                            DrawText(message1, screenWidth/2 - message1Width/2, 400, message1FontSize, BLUE);
                            DrawText(message2, screenWidth/2 - message2Width/2, 480, message2FontSize, WHITE);

                            EndDrawing();

                            if (IsKeyPressed(KEY_ENTER)) {
                                resetGame(Board, &selectedPiece.flag, &selectedPiece.row, &selectedPiece.col);
                                initBoard(Board);
                                break;
                            }else if (IsKeyPressed(KEY_ESCAPE)) {
                                CloseWindow();
                                exit(0);
                            }

                        }
                    }

                    log_board_state(Board);   
                }

                /*CPU moves
                if(PlayerTurn == 1){
                    Move cpuMove = getRandomCpuMove(Board, P2_PAWN);
                    if (cpuMove.fromRow != -1) {
                        log_info("cpu mov; fro r;%d c;%d to r;%d c;%d",cpuMove.fromRow,cpuMove.toCol,cpuMove.toRow,cpuMove.toCol);
                        //boxClicked(cpuMove.fromRow,cpuMove.fromCol,&selectedPiece.flag,&selectedPiece.row,&selectedPiece.col,Board,&PlayerTurn);
                        //selectedPiece.flag=P2_PAWN;
                        //selectedPiece.row=cpuMove.fromRow;
                        //selectedPiece.col=cpuMove.fromCol;
                        //boxClicked(cpuMove.fromRow,cpuMove.fromCol,&selectedPiece.flag,&selectedPiece.row,&selectedPiece.col,Board,&PlayerTurn);
                        //selectedPiece.flag=EMPTY_BOX;
                    } else {
                        // CPU has no moves — treat as loss
                        int winner = checkWinCondition(Board);
                        if (winner == 1 || winner == 2) {
                            const char *message1 = (winner == 1) ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!";
                            const char *message2 =  "Press ENTER to restart, ESC to quit...";
                            
                            int message1FontSize = 50;
                            int message2FontSize = 35;
                            int message1Width = MeasureText(message1, message1FontSize);
                            int message2Width = MeasureText(message2, message2FontSize);
        
                            // Display win message and wait for ENTER
                            while (!WindowShouldClose()) {
                                BeginDrawing();
                                ClearBackground(RAYWHITE);
                                DrawTexture(background, 0, 0, WHITE);
                                DrawText(message1, screenWidth/2 - message1Width/2, 400, message1FontSize, BLUE);
                                DrawText(message2, screenWidth/2 - message2Width/2, 480, message2FontSize, WHITE);

                                EndDrawing();

                                if (IsKeyPressed(KEY_ENTER)) {
                                    resetGame(Board, &selectedPiece.flag, &selectedPiece.row, &selectedPiece.col);
                                    initBoard(Board);
                                    break;
                                }else if (IsKeyPressed(KEY_ESCAPE)) {
                                    CloseWindow();
                                    exit(0);
                                }

                            }
                        }
                    }
                    PlayerTurn = 2; //player 2' turn
                    
                }*/
                
                //Display changes 
                ClearBackground(BACKGROUND_COLOR);
                DrawTexture(background, 0, 0, WHITE);
                drawBoard(offsetX, offsetY, cellSize, row, col, Board);
                drawPieces(offsetX, offsetY, cellSize, selectedPiece.row, selectedPiece.col, Board);
                EndDrawing();
            }else if(gameMode == 1){ //lan mode
                exit(0);
            }
        }else if(currentState == NETWORK_SETUP_STATE){
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(splashscreen, 0, 0, WHITE);
            drawLanMenu(selectedLanOption);
            EndDrawing();
            
            if (IsKeyPressed(KEY_DOWN)) {
                selectedLanOption = (selectedLanOption + 1) % totalMenuOptions;
                PlayMenuSoundMove();
            }
            if (IsKeyPressed(KEY_UP)) {
                selectedLanOption = (selectedLanOption - 1 + totalMenuOptions) % totalMenuOptions;
                PlayMenuSoundMove();
            }

            if (IsKeyPressed(KEY_ENTER)) {
                gameMode = selectedLanOption + 1;
                if(selectedLanOption == 1){ //client connect
                    currentState = NETWORK_CONNECT_STATE;
                }else{  //server host
                    isServer=true;
                    currentState = NETWORK_CONNECT_STATE;
                }
                PlayMenuSoundSelect();
            }else if (IsKeyPressed(KEY_M)) {   //return to main menu
                currentState = MENU_STATE;
            }
        }else if(currentState==NETWORK_CONNECT_STATE){
            if (isServer){  //server mode
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTexture(splashscreen, 0, 0, WHITE);
                drawLanServerConnect();
                EndDrawing();
                if(networkSetup(isServer, get_local_ip())){
                    currentState = GAME_STATE;
                }
                if (IsKeyPressed(KEY_M)) {   //return to main menu
                    currentState = MENU_STATE;
                }
            }else{  //client mode
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTexture(splashscreen, 0, 0, WHITE);
                drawLanClientConnect(clientIpBuffer);
                EndDrawing();

                // Handle character input
                int key = GetCharPressed();
                int ipCursor = strlen(clientIpBuffer);
                bool isTypingIp = true;   
                while (key > 0) {
                    if (ipCursor < MAX_IP_LEN - 1 && (key == '.' || isdigit(key))) {
                        clientIpBuffer[ipCursor++] = (char)key;
                        clientIpBuffer[ipCursor] = '\0';
                    }
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE) && ipCursor > 0) {
                    ipCursor--;
                    clientIpBuffer[ipCursor] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER)) {
                    // attempt connection using clientIpBuffer
                    if (networkSetup(isServer,clientIpBuffer)){
                        //if client connected successfully
                        currentState = GAME_STATE;
                    }
                }

                if (IsKeyPressed(KEY_ESCAPE)) {
                    exit(0);
                }
                if (IsKeyPressed(KEY_M)) {   //return to main menu
                    currentState = MENU_STATE;
                }
                
            }
        }
        //Toggle Debug mode
        if (IsKeyPressed(KEY_F1)) {
            DEBUG_MODE = !DEBUG_MODE;
            log_info("Debug mode %s", DEBUG_MODE ? "enabled" : "disabled");
            
        }
    }
    UnloadMusicStream(bgm);
    CloseGameAudio();
    CloseWindow();
    return 0;
}