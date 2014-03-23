#ifndef GLOBALS_H
#define GLOBALS_H

//The header
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
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

typedef struct entry {
    int score;
    std::string name;
} scoreEntry;

extern scoreEntry highScores[10];
extern std::string scoresToView[10];

extern SDL_Surface *screen;
extern SDL_Event event;
extern TTF_Font *font;
extern SDL_Surface *viewScore;
extern SDL_Color scoreColor;
extern int score;

extern int *xFoodPos, *yFoodPos;
extern int xSnake, ySnake, xFood, yFood;
extern int *xSnakePos, *ySnakePos;

extern int frame;
extern bool cap;

extern int state;
extern int nextState;


#endif
