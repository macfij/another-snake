#ifndef _POKARM_H_
#define _POKARM_H_

#include "SDL/SDL.h"

class Food {
private:
    SDL_Rect point;
    int xPos;
    int yPos;
    int w, h;

public:
    Food();
    void get_position(int*, int*);
    void reset_position();
    void show();
};
#endif
