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
#include "SDL/SDL_ttf.h"
#include <string.h>
#include <stdlib.h>


GameState* currState;

void change_state() {
    if (nextState != NULL_STATE) {
        if (nextState != EXIT_STATE) {
            delete currState;
        }
        switch (nextState) {
        case PLAY_STATE:
            currState = new Play();
            break;
        case LOSE_STATE:
            currState = new Lose();
            break;
        case ENTRY_SCORE_STATE:
            currState = new EnterScore();
            break;
        case HIGH_SCORES_STATE:
            currState = new ShowHighScores();
            break;
        }
        state = nextState;
        nextState = NULL_STATE;
    }
}

int main(int argc, char* args[]) {

    if (init() == false) {
        return 1;
    }
    if (load_files() == false) {
        return 1;
    }

    currState = new Intro();
    state = INTRO_STATE;



    while (state != EXIT_STATE) {
        fps.start();

        currState->handle_events();
        currState->logic();
        change_state();
        currState->render();
        if (SDL_Flip(screen) == -1) {
            return 1;
        }
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
        }
    }
    clean_up();
    return 0;
}
