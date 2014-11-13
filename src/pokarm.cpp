#include "constants.h"
#include "pokarm.h"
#include "globals.h"
#include "SDL/SDL.h"

Pokarm::Pokarm() {
    w = SNAKE_CELL_SIZE - 2;
    h = SNAKE_CELL_SIZE - 2;
    srand(2014);
    Pokarm::reset_position();
}

void Pokarm::reset_position() {
    xPos = rand() % 640;
    xPos = xPos - (xPos % (SNAKE_CELL_SIZE + SNAKE_CELL_SEPARATION)) + 1;
    yPos = rand() % 480;
    yPos = yPos - (yPos % (SNAKE_CELL_SIZE + SNAKE_CELL_SEPARATION)) + 1;
}

void Pokarm::get_position(int* x, int* y) {
    *x = xPos;
    *y = yPos;
}

void Pokarm::show() {
    point.w = w;
    point.h = h;
    point.x = xPos;
    point.y = yPos;
    SDL_FillRect(screen, &point,
            SDL_MapRGB(screen->format, 0x000000, 0x000000, 255));
}
