#OBJS specifies which files to compile as part of the project
OBJS = src/main.cpp src/circle.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
all : $(OBJS)
	clang++ $(OBJS) -ggdb -Wall -lSDL2 -o $(OBJ_NAME)