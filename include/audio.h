#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"

void InitGameAudio(void);
void CloseGameAudio(void);

void PlayMoveSound(void);
void PlayCaptureSound(void);
void PlayKingSound(void);
void PlayMenuSoundMove(void);
void PlayMenuSoundSelect(void);

#endif
