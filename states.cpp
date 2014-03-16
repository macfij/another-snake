#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "states.h"
#include "globals.h"
#include "functions.h"

Intro::Intro(){
	background = load_Image("images/intro.png");
}

Intro::~Intro(){
	SDL_FreeSurface(background);
}

void Intro::handle_events(){
	while( SDL_PollEvent( &event ) ){
		if( event.type == SDL_QUIT ){
			state = EXIT_STATE;
		}
		if ( ( event.type == SDL_KEYDOWN ) && ( event.key.keysym.sym == SDLK_SPACE ) ){
			nextState = PLAY_STATE;
		}
	}

}

void Intro::logic(){}

void Intro::render(){
	apply_surface(0, 0, background, screen);
}

Lose::Lose(){
	background = load_Image("images/lose.png");
}

Lose::~Lose(){
	SDL_FreeSurface(background);
}

void Lose::logic(){}

void Lose::render(){
	apply_surface(0,0, background, screen);
}

void Lose::handle_events(){
	while( SDL_PollEvent( &event ) ){
		if( event.type == SDL_QUIT ){
			state = EXIT_STATE;
		}
		if ( ( event.type == SDL_KEYDOWN ) ){
			if ( event.key.keysym.sym == SDLK_SPACE ){
				nextState = PLAY_STATE;
			}
			if ( event.key.keysym.sym == SDLK_ESCAPE ){
				nextState = EXIT_STATE;
			}
		}
	}
}

Play::Play(){
	xSnakePos = &xSnake;
	ySnakePos = &ySnake;
	xFoodPos = &xFood;
	yFoodPos = &yFood;
}

Play::~Play(){}

void Play::handle_events(){
	while(SDL_PollEvent(&event)){
		moj.handle_input();
		if(event.type == SDL_QUIT){
			nextState = EXIT_STATE;
		}
	}
}

void Play::logic(){
	moj.move();
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	jedzenie.get_position(xFoodPos, yFoodPos);
	moj.get_position(xSnakePos, ySnakePos);
	moj.mouth_open(moj.get_dir(), xSnake, ySnake, xFood, yFood);

	if( ((xFood >= xSnake) && (xFood <= xSnake+5)) && ((yFood >= ySnake) && (yFood <= ySnake + 5)) ){
		jedzenie.reset_position();
		moj.grow();
		moj.mouth_change_state(false);
	}
	if ( moj.check_collision() == true ){
		nextState = LOSE_STATE;
	}
}

void Play::render(){
	moj.show();
	jedzenie.show();
}
