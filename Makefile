# ==============================
# Compiler & Debugger Setup
# ==============================
CC = gcc
DD = gdb -tui

# ==============================
# Platform Detection
# ==============================
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    PLATFORM = PLATFORM_DESKTOP
    SYS_LIBS = -lm -lpthread -ldl -lrt -lX11 -lGL
else ifeq ($(UNAME_S), Windows)
    PLATFORM = PLATFORM_DESKTOP
    SYS_LIBS = -lopengl32 -lgdi32 -lwinmm
endif

# ==============================
# Raylib Setup (auto fetch/build)
# ==============================
RAYLIB_DIR = external/raylib
RAYLIB_SRC = $(RAYLIB_DIR)/src
RAYLIB_LIB = $(RAYLIB_SRC)/libraylib.a
CFLAGS = -I$(RAYLIB_SRC)
LDFLAGS = $(RAYLIB_LIB) $(SYS_LIBS)

# Clone and build Raylib if not found
$(RAYLIB_LIB):
	@echo ">>> Raylib not found, cloning and building locally..."
	mkdir -p external
	cd $(RAYLIB_SRC) && make PLATFORM=$(PLATFORM)

# ==============================
# Build Setup
# ==============================
TARGET = ./bin/game
SRC = ./src/main.c ./src/render.c ./src/game_logic.c ./src/log.c ./src/audio.c ./src/networking.c ./src/cpu.c

# ==============================
# Build Rules
# ==============================
$(TARGET): $(SRC) $(RAYLIB_LIB)
	@mkdir -p ./bin
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(LDFLAGS)

# ==============================
# Commands
# ==============================
all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	if [ -d "$(RAYLIB_SRC)" ]; then \
		cd $(RAYLIB_SRC) && make clean || true; \
	fi

debug: $(TARGET)
	$(DD) $(TARGET)

.PHONY: all run clean debug

