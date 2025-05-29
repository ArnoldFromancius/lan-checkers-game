#Compiler setup
CC = gcc
#Debugger setup
DD = gdb -tui

# Detect Platform And Use Appropriate Flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    CFLAGS = -g -lraylib -lm -lpthread -ldl -lrt -lX11 
else ifeq ($(UNAME_S), Windows)
    CFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
endif

#Setup Target
TARGET = ./bin/game
SRC = ./src/main.c ./src/render.c ./src/game_logic.c ./src/log.c ./src/audio.c ./src/networking.c

#Build Target
$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS)


#Initialize Target With Make Command
all: 
	$(TARGET)

#Run Project
run: $(TARGET)
	./$(TARGET)

#Cleanup Project
clean: 
	rm -f $(TARGET)

#Debug Project 
debug: $(TARGET)
	$(DD) $(TARGET)

