CC = g++
INC = -I"include/"
CFLAGS = -g -std=gnu++0x -Wall -Wextra
LIBS = -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer 
EXE = bin/snake
SRC = $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
OBJS = $(subst src/,obj/,$(SRC))

all : $(EXE)

$(EXE) : $(OBJS)
	@echo "Linking object files to binary" $@ "..."
	@$(CC) $(CFLAGS) $^ -o $@ $(INC) $(LIBS)

obj/%.o : src/%.cpp
	@echo "Compiling file:" $< "..."
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC) $(LIBS)

clean :
	@echo "Removing object files and binary..."
	@rm -f obj/*
	@rm -f bin/snake
