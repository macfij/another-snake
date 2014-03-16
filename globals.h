#ifndef GLOBALS_H
#define GLOBALS_H

//The header
#include "SDL/SDL.h"

enum PossibleStates{
	NULL_STATE,
	INTRO_STATE,
	PLAY_STATE,
	LOSE_STATE,
	EXIT_STATE,
};

extern SDL_Surface *screen;
extern SDL_Event event;

extern int *xFoodPos, *yFoodPos;
extern int xSnake, ySnake, xFood, yFood;
extern int *xSnakePos, *ySnakePos;

extern int frame;
extern bool cap;

extern int state;
extern int nextState;


#endif
