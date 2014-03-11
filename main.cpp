#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "timer.h"
#include "pokarm.h"
#include "snake.h"
#include "constants.h"
#include "globals.h"
#include "functions.h"
#include <string>
#include <iostream>


int main(int argc, char* args[]){

	bool quit = false;
	  if(init()==false) return 1;
	  if(load_files()==false) return 1;
	  
	  Timer fps;
	  Timer myTimer;
	  
	  Snake moj;
	  Pokarm jedzenie;
	  
	  xSnakePos = &xSnake;
	  ySnakePos = &ySnake;
	  xFoodPos = &xFood;
	  yFoodPos = &yFood;
	  while(quit==false){

	myTimer.start(); 
	fps.start();       
	while(SDL_PollEvent(&event)){
	  moj.handle_input();

	   if(event.type==SDL_KEYDOWN){
		 if(event.key.keysym.sym == SDLK_y) jedzenie.reset_position();
	   }
	  
	  if(event.type == SDL_QUIT){
		quit=true;
	  }
	}

	moj.move();
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	jedzenie.get_position(xFoodPos, yFoodPos);
	moj.get_position(xSnakePos, ySnakePos);

	if( ((xFood >= xSnake) && (xFood <= xSnake+5)) && ((yFood >= ySnake) && (yFood <= ySnake + 5)) ){
	  jedzenie.reset_position();
	  moj.grow();
	}
	moj.show();
	jedzenie.show();

	if(SDL_Flip(screen)==-1) return 1;

	if(fps.get_ticks()<1000/FRAMES_PER_SECOND){
	  SDL_Delay((1000/FRAMES_PER_SECOND)-fps.get_ticks());
	}

	  }
	  clean_up();
	  return 0;
	}
