OBJS = main.cpp
CC = g++
COMPILER_FLAGS = -w 
LINKER_FLAGS = -lSDL2 -lSDL2_ttf
OBJ_NAME = game

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)