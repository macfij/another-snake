CC = g++
CFLAGS = -g 
LIBS = -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer
SOURCES = main.cpp functions.cpp snake.cpp timer.cpp pokarm.cpp globals.cpp states.cpp
TARGET = snake
all: $(TARGET)
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LIBS)
clean:
	$(RM) $(TARGET)
