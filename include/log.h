#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "../include/game_logic.h"

extern int DEBUG_MODE;

void log_info(const char *fmt, ...);
void log_warn(const char *fmt, ...);
void log_error(const char *fmt, ...);
void log_board_state(int board[BOARD_SIZE][BOARD_SIZE]);

#endif

