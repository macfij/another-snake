#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "globals.h"

SDL_Event event;
SDL_Surface *screen = NULL;
TTF_Font *font = NULL;
SDL_Surface *viewScore = NULL;
SDL_Color scoreColor = {0, 255, 0, 0};
SDL_Color focusOnColor = {255, 255, 255};

int *xFoodPos, *yFoodPos;
int xSnake, ySnake, xFood, yFood;
int *xSnakePos, *ySnakePos;
int score = 0;
int frame = 0;
bool cap = true;

int state = NULL_STATE;
int nextState = NULL_STATE;

scoreEntry highScores[10];
std::string scoresToView[10];

Timer fps;

bool paused = false;

char buffer[10];
