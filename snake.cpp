#include "SDL/SDL.h"
#include "constants.h"
#include "snake.h"
#include "globals.h"
#include <iostream>

Snake::Snake()
{
  w=5;
  h=5;
  open = false;
  faceVar = -1;
  length = SNAKE_LENGTH;
  velocity = SNAKE_VELOCITY;
  xPos = new int[length];
  yPos = new int[length];
  for(int i=0; i<length; i++){
    *(xPos+i)=318;    
    *(yPos+i)=240;
  }
  xVel = velocity;
  yVel = 0;
  direction = "RIGHT";
}

void Snake::check_collision()
{
	int xCopyHead = xPos[0];
	int yCopyHead = yPos[0];
	for (int j = 1; j < length; j++){
		if (xCopyHead == xPos[j] && yCopyHead == yPos[j])
			std::cout<<"duap"<<std::endl;
	}
}

void Snake::handle_input()
{
  if(event.type==SDL_KEYDOWN){
    switch(event.key.keysym.sym){
    case SDLK_w:
    	direction = "UP";
      if (yVel == 0)
	yVel = -velocity;
      xVel = 0;
      break;
    case SDLK_s:
    	direction = "DOWN";
      if (yVel == 0)
	yVel = +velocity;
      xVel = 0;
      break;
    case SDLK_a:
    	direction = "LEFT";
      if (xVel == 0)
	xVel = -velocity;
      yVel = 0;
      break;
    case SDLK_d:
    	direction = "RIGHT";
      if (xVel == 0)
	xVel = +velocity;
      yVel = 0;
      break;
    case SDLK_f:
      open = !open;
    }
  }
}

void Snake::move()
{
  
  for(int i=length-1; i>0; i--){
    *(xPos+i)=*(xPos+i-1);
    *(yPos+i)=*(yPos+i-1);
  }
  
  if (*xPos >= SCREEN_WIDTH) *xPos = 0;
  else if (*xPos < 0) *xPos = SCREEN_WIDTH - 4;
  else *xPos += xVel;

  if (*yPos >= SCREEN_HEIGHT) *yPos = 0;
  else if (*yPos < 0) *yPos = SCREEN_HEIGHT + yVel;
  else *yPos += yVel;
}

void Snake::show(){
  
  for(int i=1; i<length; i++){
    snakeBody.x=*(xPos+i);
    snakeBody.y=*(yPos+i);
    snakeBody.w=w;
    snakeBody.h=h;
    SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, (*(xPos+i)%255), (*(yPos+i)%255), 128));
  } 
  // OPCJA 2.5
  if(xVel != 0){
    if(xVel >= 0) faceVar=-1; else faceVar = 1; 
    if(open){
      
      snakeBody.x = *xPos;
      snakeBody.y = *yPos-2;
      snakeBody.w = 8;
      snakeBody.h = 7; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, (*xPos%255), (*yPos%255), 128));
      
      //Nadglowie duze
      snakeBody.x = *xPos+3+faceVar*3;
      snakeBody.y = *yPos-2;
      snakeBody.w = 2;
      snakeBody.h = 2; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Nadglowie male
      snakeBody.x = *xPos+3+faceVar*1;
      snakeBody.y = *yPos-2;
      snakeBody.w = 2;
      snakeBody.h = 1; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Oko
      snakeBody.x = *xPos+3+faceVar*2;
      snakeBody.y = *yPos+1;
      snakeBody.w = 2;
      snakeBody.h = 2; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Paszcza
      snakeBody.x = *xPos+2-faceVar*2;
      snakeBody.y = *yPos;
      snakeBody.w = 4;
      snakeBody.h = 4; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
    }
    else{
      snakeBody.x = *xPos;
      snakeBody.y = *yPos;
      snakeBody.w = 8;
      snakeBody.h = 5; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, (*xPos%255), (*yPos%255), 128));
      
      //Oko
      snakeBody.x = *xPos+3+faceVar*2;
      snakeBody.y = *yPos+1;
      snakeBody.w = 2;
      snakeBody.h = 2; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Paszcza
      snakeBody.x = *xPos+2-faceVar*2;
      snakeBody.y = *yPos+3;
      snakeBody.w = 4;
      snakeBody.h = 1; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    }
  }
  else{
    if(yVel>=0) faceVar = -1; else faceVar = 1;
    if(open){
      snakeBody.x = *xPos-2;
      snakeBody.y = *yPos-3;
      snakeBody.w = 7;
      snakeBody.h = 8; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, (*xPos%255), (*yPos%255), 128));
      
      //Nadglowie duze
      snakeBody.x = *xPos-2;
      snakeBody.y = *yPos+faceVar*2;
      snakeBody.w = 2;
      snakeBody.h = 2; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Nadglowie male
      snakeBody.x = *xPos-2;
      snakeBody.y = *yPos+faceVar;
      snakeBody.w = 1;
      snakeBody.h = 2; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Oko
      snakeBody.x = *xPos+1;
      snakeBody.y = *yPos+faceVar*2;
      snakeBody.w = 2;
      snakeBody.h = 2; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Paszcza
      snakeBody.x = *xPos;
      snakeBody.y = *yPos-1-faceVar*2;
      snakeBody.w = 4;
      snakeBody.h = 4; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    }
    else{
      //Wypelnienie glowy
      snakeBody.x = *xPos;
      snakeBody.y = *yPos-3;
      snakeBody.w = 5;
      snakeBody.h = 8; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, (*xPos%255), (*yPos%255), 128));
      
      //Oko
      snakeBody.x = *xPos+1;
      snakeBody.y = *yPos+faceVar*2;
      snakeBody.w = 2;
      snakeBody.h = 2; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Paszcza
      snakeBody.x = *xPos+3;
      snakeBody.y = *yPos-1-faceVar*2;
      snakeBody.w = 1;
      snakeBody.h = 4; 
      SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    }
  }
  
}

void Snake::get_position(int* x, int* y)
{
  *x = *xPos;
  *y = *yPos;
}

void Snake::grow()
{
  length++;

  int* xOld = xPos;
  int* yOld = yPos;
  
  int* xNew = new int[length];
  int* yNew = new int[length];

  for(int i=0; i<length-1; i++){
    *(xNew+i) = *(xPos+i);
    *(yNew+i) = *(yPos+i);
  }
  *(xNew+length-1) = *(xNew+length-2);
  *(yNew+length-1) = *(yNew+length-2);

  xPos = xNew;
  yPos = yNew;

  delete [] xOld;
  delete [] yOld;
}

void Snake::mouth_change_state(bool isOpen)
{
	this->open = isOpen;
}

std::string Snake::get_dir()
{
	return this->direction;
}
