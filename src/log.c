#include "../include/log.h"
#include "../include/game_logic.h"

static FILE *log_file = NULL;

static void open_log_file() {
    if (!log_file) {
        log_file = fopen("./debug/debug.log", "a");
        if (!log_file) {
            fprintf(stderr, "LOGGING ERROR: Could not open debug.log\n");
        }
    }
}

static void log_prefix(const char *level) {
    if (!log_file) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(log_file, "[%02d:%02d:%02d] [%s] ",
        t->tm_hour, t->tm_min, t->tm_sec, level);
}

static void log_write(const char *level, const char *fmt, va_list args) {
    open_log_file();
    if (!log_file) return; // <- fail safely if file couldn't open

    log_prefix(level);
    vfprintf(log_file, fmt, args);
    fprintf(log_file, "\n");
    fflush(log_file);
}


void log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_write("INFO", fmt, args);
    va_end(args);
}

void log_warn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_write("WARN", fmt, args);
    va_end(args);
}

void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_write("ERROR", fmt, args);
    va_end(args);
}


void log_board_state(int board[BOARD_SIZE][BOARD_SIZE]) {
    log_info("[BOARD STATE]");
    for (int row = 0; row < BOARD_SIZE; row++) {
        char line[256] = {0}; // Enough for one row
        char *ptr = line;

        for (int col = 0; col < BOARD_SIZE; col++) {
            ptr += sprintf(ptr, "%d ", board[row][col]);
        }

        log_info("%s", line);
    }
    log_info("----------------------");
}

