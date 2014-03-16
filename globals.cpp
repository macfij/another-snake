#include "SDL/SDL.h"
#include "globals.h"

SDL_Event event;
SDL_Surface *screen = NULL;

int *xFoodPos, *yFoodPos;
int xSnake, ySnake, xFood, yFood;
int *xSnakePos, *ySnakePos;

int frame = 0;
bool cap = true;

int state = NULL_STATE;
int nextState = NULL_STATE;
