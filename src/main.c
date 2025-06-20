#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
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
bool serverTurn = true;
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
    initCpu();
    log_info("#MAIN_FUNC board initilized...");

    //initialize selection
    Selection selectedPiece;
    selectedPiece.flag=EMPTY_BOX; //no selection.	
    selectedPiece.row= -1; 
    selectedPiece.col= -1;

    //Main game loop
    int winner=-1;
    int PlayerTurn=2;
    int row=-1,col=-1;
    bool serverInitPlay=true; //need for the first ever move by server to keep screen updated
    //timer settings    
    float turnStartTime = 0.0f;
    const float TURN_TIME_LIMIT = 60.0f; // seconds
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
                
                if(PlayerTurn == 2){    //user turn
                    //When a box is clicked
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        Vector2 mouse = GetMousePosition();
                        col = (mouse.x - offsetX) / cellSize;
                        row = (mouse.y - offsetY) / cellSize;
                        log_info("\n#MAIN_FUNC selection made: X:%d Y:%d...",row,col);
                        //Logic for steps to take if a box is clicked
                        MovePacket move; //only necessary for networking but is needed argument
                        boxClicked(row, col, &selectedPiece.flag, &selectedPiece.row, &selectedPiece.col, Board, &PlayerTurn, &move);    
                         if(PlayerTurn == 1){turnStartTime = GetTime();}
                    }
                }else if(PlayerTurn == 1){  //cpu turn
                    cpuMakeMove(Board,&PlayerTurn);
                    turnStartTime = GetTime();
                }
                
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

                //log_board_state(Board);  
                //Display changes 
                ClearBackground(BACKGROUND_COLOR);
                DrawTexture(background, 0, 0, WHITE);

                drawBoard(offsetX, offsetY, cellSize, row, col, Board);
                drawPieces(offsetX, offsetY, cellSize, selectedPiece.row, selectedPiece.col, Board);

                // --- TURN TEXT ---
                const char* turnMsg = (PlayerTurn == 1) ? "PLAYER 1' TURN" : "PLAYER 2' TURN";
                Color turnColor = (PlayerTurn == 1) ? BLUE : RED;
                DrawText(turnMsg, 20, 20, 30, turnColor);

                // --- TIMER BAR ---
                float timeElapsed = GetTime() - turnStartTime;
                float timeLeft = TURN_TIME_LIMIT - timeElapsed;
                if (timeLeft < 0) timeLeft = 0;

                int barMaxWidth = 200;
                int barHeight = 18;
                int barX = 20;
                int barY = 65;
                int barWidth = (int)((timeLeft / TURN_TIME_LIMIT) * barMaxWidth);

                DrawRectangle(barX, barY, barMaxWidth, barHeight, DARKGRAY);
                DrawRectangle(barX, barY, barWidth, barHeight, SKYBLUE);
                DrawRectangleLines(barX, barY, barMaxWidth, barHeight, RAYWHITE);

                //Draw timer text next to bar
                char timerText[64];
                snprintf(timerText, sizeof(timerText), "Time: %.1fs", timeLeft);
                DrawText(timerText, barX + barMaxWidth + 10, barY, 20, LIGHTGRAY);
                drawPlayerPieceCounts(Board, PlayerTurn);
                EndDrawing();
            
            }else if(gameMode == 1){ //lan mode
                
                MovePacket move;
                //if we are server
                if(isServer){
                    log_info("ServerMode: connected to client...");
                    if(serverTurn){//Our turn
                        //waiting for mouse click
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                            Vector2 mouse = GetMousePosition();
                            col = (mouse.x - offsetX) / cellSize;
                            row = (mouse.y - offsetY) / cellSize;
                            log_info("\n#MAIN_FUNC selection made: X:%d Y:%d...",row,col);
                            //Logic for steps to take if a box is clicked
                            move.flag = 0;
                            boxClicked(row, col, &selectedPiece.flag, &selectedPiece.row, &selectedPiece.col, Board, &PlayerTurn, &move);
                            
                            //send updated board to client
                            if(move.flag){
                                int sock = getSocketFD();
                                sendMove(sock, &move);
                                serverTurn = false;
                            }
                        
                            //check for game over
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

                    }else if(!serverTurn){//Clients turn
                        //Receive updated board from client
                        int sock = getSocketFD();
                        recvMove(sock, &move);
                        applyMovePacket(Board, &move);
                        switchPlayer(&PlayerTurn);
                        serverTurn = true;
                    }
                }else{  //if we are client
                    log_info("ClientMode: connected to server...");
                    if(serverTurn && serverInitPlay){//Servers turn- first ever move
                        serverInitPlay = false;
                        //this part just makes sure that the board is atleast drawn once before waiting for a move from server
                    }
                    else if(serverTurn && !serverInitPlay){//Servers turn- every followup move
                        //Receive updated board from server
                        int sock = getSocketFD();
                        recvMove(sock, &move);
                        applyMovePacket(Board, &move);
                        switchPlayer(&PlayerTurn);
                        serverTurn = false;
                    }
                    else if(!serverTurn){//Our turn
                        move.flag=0;
                        //waiting for mouse click
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                            Vector2 mouse = GetMousePosition();
                            col = (mouse.x - offsetX) / cellSize;
                            row = (mouse.y - offsetY) / cellSize;
                            log_info("\n#MAIN_FUNC selection made: X:%d Y:%d...",row,col);
                            //Logic for steps to take if a box is clicked
                            boxClicked(row, col, &selectedPiece.flag, &selectedPiece.row, &selectedPiece.col, Board, &PlayerTurn, &move);
                            //send updated board to server
                            if(move.flag){
                                int sock = getSocketFD();
                                sendMove(sock, &move);
                                serverTurn = true;
                            }

                            //check for game over
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
                    }
                }
                //Display changes 
                ClearBackground(BACKGROUND_COLOR);
                DrawTexture(background, 0, 0, WHITE);
                drawBoard(offsetX, offsetY, cellSize, row, col, Board);
                drawPieces(offsetX, offsetY, cellSize, selectedPiece.row, selectedPiece.col, Board);
                const char* turnMsg = (PlayerTurn == 1) ? "PLAYER 1 TURN" : "PLAYER 2 TURN";
                DrawText(turnMsg, 20, 20, 30, (PlayerTurn == 1) ? BLUE : RED);
                EndDrawing();   
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
                    isServer=false;                    
                    currentState = NETWORK_CONNECT_STATE;
                }else{  //server host
                    isServer=true;
                    currentState = NETWORK_CONNECT_STATE;
                }
                PlayMenuSoundSelect();
            }else if (IsKeyPressed(KEY_M)) {   //return to main menu
                isServer=false;
                currentState = MENU_STATE;
            }
        }else if(currentState==NETWORK_CONNECT_STATE){
            if (isServer){  //server mode
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTexture(splashscreen, 0, 0, WHITE);
                drawLanServerConnect();
                EndDrawing();
                networkSetup(isServer, get_local_ip());
                if (isClientConnected()) {
                    // Proceed to game
                    currentState = GAME_STATE;
                    gameMode=1;
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
                        gameMode=1;
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