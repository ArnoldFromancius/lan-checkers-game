#Compiler setup
CC = gcc

# Detect Platform And Use Appropriate Flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    CFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11
else ifeq ($(UNAME_S), Windows)
    CFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
endif

#Setup Target
TARGET = ./src/game
SRC = ./src/main.c

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
