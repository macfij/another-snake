#ifndef _POKARM_H_
#define _POKARM_H_

#include "SDL/SDL.h"

class Pokarm {
private:
    SDL_Rect point;
    int xPos;
    int yPos;
    int w, h;

public:
    Pokarm();
    void get_position(int*, int*);
    void reset_position();
    void show();
};
#endif
