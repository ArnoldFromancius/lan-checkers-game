#include "../include/audio.h"

static Sound moveSound;
static Sound captureSound;
static Sound menuSound1;
static Sound menuSound2;
static Sound kingSound;

void InitGameAudio(void) {
    InitAudioDevice();
    moveSound = LoadSound("assets/sounds/move.wav");
    captureSound = LoadSound("assets/sounds/capture.wav");
    kingSound = LoadSound("assets/sounds/king.wav");
    menuSound1 = LoadSound("assets/sounds/menu_move.wav");
    menuSound2 = LoadSound("assets/sounds/menu_select.wav");
}

void CloseGameAudio(void) {
    UnloadSound(moveSound);
    UnloadSound(captureSound);
    UnloadSound(menuSound1);
    UnloadSound(menuSound2);
    UnloadSound(kingSound);
    CloseAudioDevice();
}

void PlayMoveSound(void) {
    PlaySound(moveSound);
}
 
void PlayKingSound(void) {
    PlaySound(kingSound);
}

void PlayCaptureSound(void) {
    PlaySound(captureSound);
}

void PlayMenuSoundMove(void) {
    PlaySound(menuSound1);
}

void PlayMenuSoundSelect(void) {
    PlaySound(menuSound2);
}