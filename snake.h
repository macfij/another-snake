#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "SDL/SDL.h"
#include "constants.h"

enum Directions {
    RIGHT, LEFT, UP, DOWN,
};

class Snake {
private:
    SDL_Rect snakeBody;
    int w, h;
    int velocity;
    int length;
    int *xPos, *yPos;
    int xVel, yVel;
    bool open;
    int faceVar;
    Directions direction;
    //SDLKey ctrls[4];
public:
    Snake();
    ~Snake();
    void handle_input();
    void move();
    void show();
    void get_position(int*, int*);
    void grow();
    void mouth_change_state(bool);
    int get_dir();
    bool check_collision();
    void mouth_open(int, int, int, int, int);
};
#endif
