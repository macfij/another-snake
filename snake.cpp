#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include "constants.h"
#include "snake.h"
#include "functions.h"
#include "globals.h"

Snake::Snake()
{
  w=5;
  h=5;
  open = false;
  czynnik = -1;
  length = SNAKE_LENGTH;
  xTablica = &pozycja_x[0];
  yTablica = &pozycja_y[0];
  for(int i=0; i<SNAKE_LENGTH; i++){
    *(xTablica+i)=318;    
    *(yTablica+i)=240;
  }
  xVel = 6;
  yVel = 0;
}

void Snake::handle_input()
{
  if(event.type==SDL_KEYDOWN){
    switch(event.key.keysym.sym){
    case SDLK_w:
      if (yVel == 0)
	yVel = -6;
      xVel = 0;
      break;
    case SDLK_s:
      if (yVel == 0)
	yVel = +6;
      xVel = 0;
      break;
    case SDLK_a:
      if (xVel == 0)
	xVel = -6;
      yVel = 0;
      break;
    case SDLK_d:
      if (xVel == 0)
	xVel = +6;
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
    *(xTablica+i)=*(xTablica+i-1);
    *(yTablica+i)=*(yTablica+i-1);
  }
  
  if (*xTablica >= SCREEN_WIDTH) *xTablica = 0;
  else if (*xTablica < 0) *xTablica = SCREEN_WIDTH - 4;
  else *xTablica += xVel;
  
  if (*yTablica >= SCREEN_HEIGHT) *yTablica = 0;
  else if (*yTablica < 0) *yTablica = SCREEN_HEIGHT + yVel;
  else *yTablica += yVel;
}

void Snake::show(){
  
  for(int i=1; i<length; i++){
    waz.x=*(xTablica+i);
    waz.y=*(yTablica+i);
    waz.w=w;
    waz.h=h;
    SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, (*(xTablica+i)%255), (*(yTablica+i)%255), 128));
  } 
  // OPCJA 2.5
  if(xVel != 0){
    if(xVel >= 0) czynnik=-1; else czynnik = 1; 
    if(open){
      
      waz.x = *xTablica;
      waz.y = *yTablica-2;
      waz.w = 8;
      waz.h = 7; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, (*xTablica%255), (*yTablica%255), 128));
      
      //Nadglowie duze
      waz.x = *xTablica+3+czynnik*3;
      waz.y = *yTablica-2;
      waz.w = 2;
      waz.h = 2; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Nadglowie male
      waz.x = *xTablica+3+czynnik*1;
      waz.y = *yTablica-2;
      waz.w = 2;
      waz.h = 1; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Oko
      waz.x = *xTablica+3+czynnik*2;
      waz.y = *yTablica+1;
      waz.w = 2;
      waz.h = 2; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Paszcza
      waz.x = *xTablica+2-czynnik*2;
      waz.y = *yTablica;
      waz.w = 4;
      waz.h = 4; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
    }
    else{
      waz.x = *xTablica;
      waz.y = *yTablica;
      waz.w = 8;
      waz.h = 5; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, (*xTablica%255), (*yTablica%255), 128));
      
      //Oko
      waz.x = *xTablica+3+czynnik*2;
      waz.y = *yTablica+1;
      waz.w = 2;
      waz.h = 2; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Paszcza
      waz.x = *xTablica+2-czynnik*2;
      waz.y = *yTablica+3;
      waz.w = 4;
      waz.h = 1; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    }
  }
  else{
    if(yVel>=0) czynnik = -1; else czynnik = 1;
    if(open){
      waz.x = *xTablica-2;
      waz.y = *yTablica-3;
      waz.w = 7;
      waz.h = 8; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, (*xTablica%255), (*yTablica%255), 128));
      
      //Nadglowie duze
      waz.x = *xTablica-2;
      waz.y = *yTablica+czynnik*2;
      waz.w = 2;
      waz.h = 2; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Nadglowie male
      waz.x = *xTablica-2;
      waz.y = *yTablica+czynnik;
      waz.w = 1;
      waz.h = 2; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Oko
      waz.x = *xTablica+1;
      waz.y = *yTablica+czynnik*2;
      waz.w = 2;
      waz.h = 2; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
      
      //Paszcza
      waz.x = *xTablica;
      waz.y = *yTablica-1-czynnik*2;
      waz.w = 4;
      waz.h = 4; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    }
    else{
      //Wypelnienie glowy
      waz.x = *xTablica;
      waz.y = *yTablica-3;
      waz.w = 5;
      waz.h = 8; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, (*xTablica%255), (*yTablica%255), 128));

      //Oko
      waz.x = *xTablica+1;
      waz.y = *yTablica+czynnik*2;
      waz.w = 2;
      waz.h = 2; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

      //Paszcza
      waz.x = *xTablica+3;
      waz.y = *yTablica-1-czynnik*2;
      waz.w = 1;
      waz.h = 4; 
      SDL_FillRect(screen, &waz, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    }
  }
            
      /* opcja 3 *
	 if(open){
	 SDL_SetColorKey(opened_mouth,0x0000FF00, kolorkey);
	 apply_surface(pozycja_x[0], pozycja_y[0]-2, opened_mouth, screen);
	 }
	 else{
	 
	 apply_surface(pozycja_x[0], pozycja_y[0]-2, closed_mouth, screen);
	 }
      */
    }

void Snake::get_position(int* x, int* y)
{
  *x = *xTablica;
  //std::cout<<"POZYCJA POTWORA W POTWORZE: "<<x<<" = "<<pozycja_x[0]<<std::endl;
  *y = *yTablica;
}

void Snake::grow()
{
  length++;


  int* drugi_waz_x = new int[length];
  int* drugi_waz_y = new int[length];
  for(int i=0; i<length-1; i++){
    *(drugi_waz_x+i) = *(xTablica+i);
    *(drugi_waz_y+i) = *(yTablica+i);
  }
  *(drugi_waz_x+length-1) = *(drugi_waz_x+length-2);
  *(drugi_waz_y+length-1) = *(drugi_waz_y+length-2);
  xTablica = drugi_waz_x;
  yTablica = drugi_waz_y;
}
