#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "SDL/SDL.h"
#include "constants.h"
#include <string>

class Snake
{
private:
  SDL_Rect snakeBody;
  int w, h;
  int velocity;
  int length;
  int *xPos, *yPos;
  int xVel, yVel;
  bool open;
  int faceVar;
  std::string direction;
  
public:
  Snake();
  void handle_input();
  void move();
  void show();
  void get_position(int*, int*);
  void grow();
  void mouth_change_state(bool);
  std::string get_dir();
  void check_collision();
};
#endif
