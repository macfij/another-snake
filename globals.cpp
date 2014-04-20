#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "globals.h"

SDL_Event event;
SDL_Surface *screen = NULL;
TTF_Font *font = NULL;
TTF_Font *fontBigger = NULL;
SDL_Surface *viewScore = NULL;
SDL_Color scoreColor = {0, 255, 0, 0};
SDL_Color focusOnColor = {255, 255, 255};

int *xFoodPos, *yFoodPos;
int xSnake, ySnake, xFood, yFood;
int *xSnakePos, *ySnakePos;
int score = 0;
int frame = 0;
bool cap = true;
bool wasMenu = false;

int state = NULL_STATE;
int nextState = NULL_STATE;

scoreEntry highScores[10];
std::string scoresToView[10];

Timer fps;

bool paused = false;

char buffer[10];

int SNAKE_CELL_SIZE = 10;
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int FRAMES_PER_SECOND = 20;

int resolution[8][2] = {{640, 480}, {800, 600}, {1024, 768}, {1280, 720},
			{1152, 864}, {1280, 900}, {1600, 1200}, {1920, 1080}};

int whichRes = 0;

bool sound = true;
