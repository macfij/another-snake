#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "timer.h"
#include "pokarm.h"
#include "snake.h"
#include "constants.h"
#include "globals.h"
#include "functions.h"
#include "states.h"
#include <string>
#include <iostream>


GameState* currState;

void change_state(){
	if( nextState != NULL_STATE ){
		if( nextState != EXIT_STATE ){
			delete currState;
		}

		switch( nextState ){
			case PLAY_STATE:
				currState = new Play();
				break;
			case LOSE_STATE:
				currState = new Lose();
				break;
		}

		state = nextState;
		nextState = NULL_STATE;
	}
}

int main(int argc, char* args[]){

	if( init() == false ){
		return 1;
	}
	if( load_files() == false ){
		return 1;
	}

	Timer fps;
	Timer myTimer;

	currState = new Intro();
	state = INTRO_STATE;

	while( state != EXIT_STATE ){
		myTimer.start();
		fps.start();

		currState->handle_events();
		currState->logic();
		change_state();
		currState->render();

		if( SDL_Flip( screen ) == -1 ){
			return 1;
		}

		if(fps.get_ticks()<1000/FRAMES_PER_SECOND){
			SDL_Delay((1000/FRAMES_PER_SECOND)-fps.get_ticks());
		}

	}
	clean_up();
	return 0;
	}
