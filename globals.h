#ifndef GLOBALS_H
#define GLOBALS_H

//The header
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "timer.h"
#include <string>

enum PossibleStates{
	NULL_STATE,
	INTRO_STATE,
	PLAY_STATE,
	LOSE_STATE,
	ENTRY_SCORE_STATE,
	HIGH_SCORES_STATE,
	EXIT_STATE,
};

typedef struct scoreEntry {
    int score;
    std::string name;
} scoreEntry;

typedef struct menuEntry {
    const char* msg;
    bool isFocused;
    SDL_Surface* entry;
    SDL_Color color;
} menuEntry;

extern scoreEntry highScores[10];
extern std::string scoresToView[10];

extern SDL_Surface *screen;
extern SDL_Event event;
extern TTF_Font *font;
extern SDL_Surface *viewScore;
extern SDL_Color scoreColor;
extern SDL_Color focusOnColor;
extern int score;

extern int *xFoodPos, *yFoodPos;
extern int xSnake, ySnake, xFood, yFood;
extern int *xSnakePos, *ySnakePos;

extern int frame;
extern bool cap;

extern int state;
extern int nextState;

extern Timer fps;

extern bool paused;
extern char buffer[10];


#endif
