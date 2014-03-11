#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "SDL/SDL.h"
#include "constants.h"

class Snake
{
private:
  SDL_Rect waz;
  int w, h;
  int pozycja_x[SNAKE_LENGTH], pozycja_y[SNAKE_LENGTH];
  int *xTablica;
  int *yTablica;
  int xVel, yVel;
  bool open;
  int czynnik;
  int length;
  
public:
  Snake();
  void handle_input();
  void move();
  void show();
  void get_position(int*, int*);
  void grow();
};
#endif
