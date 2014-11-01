CC = g++
CFLAGS = -g -std=gnu++0x -Wall -Wextra
LIBS = -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer
SOURCES = main.cpp functions.cpp snake.cpp timer.cpp pokarm.cpp globals.cpp states.cpp
TARGET = snake
all: $(TARGET)
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LIBS)
clean:
	$(RM) $(TARGET)
