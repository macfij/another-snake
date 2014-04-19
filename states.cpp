#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "states.h"
#include "globals.h"
#include "functions.h"
#include "constants.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>
//#include <stdio.h>
#include <sstream>

#define OPTIONS_XPOS (0.3*(SCREEN_WIDTH - optionEntries[0].entry->w))
#define OPTIONS_YPOS (0.5*(SCREEN_HEIGHT - 5*optionEntries[0].entry->h))
#define OPTIONS_H optionEntries[0].entry->h


Intro::Intro() {
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("sounds/theme.wav");
    msgAnother = NULL;
    msgSnake = NULL;
    msgSpace = NULL;
    posCounter = 400;
    spacebarCounter = 0;
}

Intro::~Intro() {
    Mix_FreeMusic(music);
    SDL_FreeSurface(msgAnother);
    SDL_FreeSurface(msgSnake);
    SDL_FreeSurface(msgSpace);
}

void Intro::handle_events() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            state = EXIT_STATE;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                if (spacebarCounter == 0 && posCounter == (SCREEN_WIDTH / float(6.4))) {
                    nextState = MENU_STATE;
                }
                spacebarCounter++;
            }
            if (spacebarCounter == 1) {
                posCounter = (SCREEN_WIDTH / float(6.4));
            }
            if (spacebarCounter == 2) {
                nextState = MENU_STATE;
            }
        }
    }
}

void Intro::logic() {
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(music, 1);
    }
    if (posCounter != (SCREEN_WIDTH / float(6.4))) {
        posCounter--;
    }
}

void Intro::render() {
    msgAnother = TTF_RenderText_Solid(font, "Another", scoreColor);
    msgSnake = TTF_RenderText_Solid(fontBigger, "SNAKE", scoreColor);
    msgSpace = TTF_RenderText_Solid(font, "Press Spacebar", scoreColor);
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    apply_surface((SCREEN_WIDTH - msgAnother->w) / 2, posCounter, msgAnother, screen);
    apply_surface((SCREEN_WIDTH - msgSnake->w) / 2, posCounter + (SCREEN_WIDTH / float(32)), msgSnake, screen);
    if (posCounter == (SCREEN_WIDTH / float(6.4))) {
        apply_surface((SCREEN_WIDTH - msgSpace->w) / 2, (SCREEN_WIDTH / float(1.6)), msgSpace, screen);
    }
}

Menu::Menu() {
    switchSound = Mix_LoadWAV("sounds/switch.wav");
    selectSound = Mix_LoadWAV("sounds/selected.wav");
    menuEntries[0].msg = "NEW GAME";
    menuEntries[0].color = focusOnColor;
    menuEntries[0].entry = NULL;
    menuEntries[0].isFocused = true;
    menuEntries[1].msg = "Options";
    menuEntries[1].entry = NULL;
    menuEntries[1].isFocused = false;
    menuEntries[1].color = scoreColor;
    menuEntries[2].msg = "High Scores";
    menuEntries[2].entry = NULL;
    menuEntries[2].isFocused = false;
    menuEntries[2].color = scoreColor;
    menuEntries[3].msg = "Exit";
    menuEntries[3].entry = NULL;
    menuEntries[3].isFocused = false;
    menuEntries[3].color = scoreColor;
    handle_high_scores();
    update_high_scores();
}

Menu::~Menu() {
    Mix_FreeChunk(selectSound);
    Mix_FreeChunk(switchSound);
}

void Menu::logic() {
    
}

void Menu::handle_events() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Mix_CloseAudio();
            nextState = EXIT_STATE;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (menu_focus()) {
            case 0:
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
                    SDL_FreeSurface(menuEntries[0].entry);
                    menuEntries[0].msg = "New Game";
                    menuEntries[0].isFocused = false;
                    menuEntries[0].color = scoreColor;
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    Mix_PlayChannel(-1, switchSound, 0);
                    SDL_FreeSurface(menuEntries[3].entry);
                    menuEntries[3].msg = "EXIT";
                    menuEntries[3].isFocused = true;
                    menuEntries[3].color = focusOnColor;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    Mix_PlayChannel(-1, switchSound, 0);
                    SDL_FreeSurface(menuEntries[1].entry);
                    menuEntries[1].msg = "OPTIONS";
                    menuEntries[1].isFocused = true;
                    menuEntries[1].color = focusOnColor;
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    Mix_PlayChannel(-1, selectSound, 0);
                    SDL_Delay(300);
                    nextState = PLAY_STATE;
                }
                break;
            case 1:
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
                    SDL_FreeSurface(menuEntries[1].entry);
                    menuEntries[1].msg = "Options";
                    menuEntries[1].isFocused = false;
                    menuEntries[1].color = scoreColor;
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    Mix_PlayChannel(-1, switchSound, 0);
                    SDL_FreeSurface(menuEntries[0].entry);
                    menuEntries[0].msg = "NEW GAME";
                    menuEntries[0].color = focusOnColor;
                    menuEntries[0].isFocused = true;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    Mix_PlayChannel(-1, switchSound, 0);
                    SDL_FreeSurface(menuEntries[2].entry);
                    menuEntries[2].msg = "HIGH SCORES";
                    menuEntries[2].color = focusOnColor;
                    menuEntries[2].isFocused = true;
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    Mix_PlayChannel(-1, selectSound, 0);
                    SDL_Delay(300);
                    nextState = OPTION_STATE;
                }
                break;
            case 2:
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
                    SDL_FreeSurface(menuEntries[2].entry);
                    menuEntries[2].msg = "High Scores";
                    menuEntries[2].color = scoreColor;
                    menuEntries[2].isFocused = false;
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    Mix_PlayChannel(-1, switchSound, 0);
                    SDL_FreeSurface(menuEntries[1].entry);
                    menuEntries[1].msg = "OPTIONS";
                    menuEntries[1].isFocused = true;
                    menuEntries[1].color = focusOnColor;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    Mix_PlayChannel(-1, switchSound, 0);
                    SDL_FreeSurface(menuEntries[3].entry);
                    menuEntries[3].msg = "EXIT";
                    menuEntries[3].isFocused = true;
                    menuEntries[3].color = focusOnColor;
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    Mix_PlayChannel(-1, selectSound, 0);
                    SDL_Delay(300);
                    wasMenu = true;
                    nextState = HIGH_SCORES_STATE;
                }
                break;
            case 3:
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
                    SDL_FreeSurface(menuEntries[3].entry);
                    menuEntries[3].msg = "Exit";
                    menuEntries[3].isFocused = false;
                    menuEntries[3].color = scoreColor;
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    Mix_PlayChannel(-1, switchSound, 0);
                    SDL_FreeSurface(menuEntries[2].entry);
                    menuEntries[2].msg = "HIGH SCORES";
                    menuEntries[2].color = focusOnColor;
                    menuEntries[2].isFocused = true;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    Mix_PlayChannel(-1, switchSound, 0);
                    SDL_FreeSurface(menuEntries[0].entry);
                    menuEntries[0].msg = "NEW GAME";
                    menuEntries[0].color = focusOnColor;
                    menuEntries[0].isFocused = true;
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    Mix_PlayChannel(-1, selectSound, 0);
                    SDL_Delay(300);
                    Mix_CloseAudio();
                    nextState = EXIT_STATE;
                }
                break;
            }
        }
    }
    
}

int Menu::menu_focus() {
    if (menuEntries[0].isFocused == true) {
        return 0;
    }
    else if (menuEntries[1].isFocused == true) {
        return 1;
    }
    else if (menuEntries[2].isFocused == true) {
        return 2;
    }
    else {
        return 3;
    }
}

void Menu::render() {
    menuEntries[0].entry = TTF_RenderText_Solid(font, menuEntries[0].msg, menuEntries[0].color);
    menuEntries[1].entry = TTF_RenderText_Solid(font, menuEntries[1].msg, menuEntries[1].color);
    menuEntries[2].entry = TTF_RenderText_Solid(font, menuEntries[2].msg, menuEntries[2].color);
    menuEntries[3].entry = TTF_RenderText_Solid(font, menuEntries[3].msg, menuEntries[3].color);
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    if (menuEntries[0].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[0].entry->w) / 2,
		      ((SCREEN_HEIGHT - menuEntries[0].entry->h) / 2) - (SCREEN_WIDTH / float(16)), menuEntries[0].entry,
		      screen);
    }
    if (menuEntries[1].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[1].entry->w) / 2,
		      ((SCREEN_HEIGHT - menuEntries[1].entry->h) / 2) - (SCREEN_WIDTH / float(32)), menuEntries[1].entry,
		      screen);
    }
    if (menuEntries[2].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[2].entry->w) / 2, ((SCREEN_HEIGHT - menuEntries[2].entry->h) / 2),
		      menuEntries[2].entry, screen);
    }
    if (menuEntries[3].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[3].entry->w) / 2,
		      ((SCREEN_HEIGHT - menuEntries[3].entry->h) / 2) + (SCREEN_WIDTH / float(32)), menuEntries[3].entry,
		      screen);
    }
}

Option::Option() {
    switchSound = Mix_LoadWAV("sounds/switch.wav");
    selectSound = Mix_LoadWAV("sounds/selected.wav");
    optionPosition = 0;
    optionEntries[0].msg[0] = "Speed";
    optionEntries[0].msg[1] = "SPEED";
    optionEntries[0].color = focusOnColor;
    optionEntries[0].entry = NULL;
    optionEntries[0].isFocused = true;
    optionEntries[1].msg[0] = "Snake size";
    optionEntries[1].msg[1] = "SNAKE SIZE";
    optionEntries[1].entry = NULL;
    optionEntries[1].isFocused = false;
    optionEntries[1].color = scoreColor;
    optionEntries[2].msg[0] = "Sound";
    optionEntries[2].msg[1] = "SOUND";
    optionEntries[2].entry = NULL;
    optionEntries[2].isFocused = false;
    optionEntries[2].color = scoreColor;
    optionEntries[3].msg[0] = "Exit";
    optionEntries[3].msg[1] = "EXIT";
    optionEntries[3].entry = NULL;
    optionEntries[3].isFocused = false;
    optionEntries[3].color = scoreColor;
}

Option::~Option() {
    Mix_FreeChunk(selectSound);
    Mix_FreeChunk(switchSound);
}

void Option::logic() {
    
}

void Option::handle_events() {
    // std::string Result2;
    //std::ostringstream convert2;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Mix_PlayChannel(-1, selectSound, 0);
            SDL_Delay(300);
            Mix_CloseAudio();
            nextState = EXIT_STATE;
        }
        if (event.type == SDL_KEYDOWN) {
            optionPosition = option_focus();
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
		optionEntries[optionPosition].isFocused = false;
		optionEntries[optionPosition].color = scoreColor;
		if (event.key.keysym.sym == SDLK_UP) {
		    if (optionPosition > 0)
			optionPosition--;
		    else
			optionPosition = 3;
		}
		if (event.key.keysym.sym == SDLK_DOWN) {
		    if (optionPosition < 3)
			optionPosition++;
		    else
			optionPosition = 0;
		}
		optionEntries[optionPosition].isFocused = true;
		optionEntries[optionPosition].color = focusOnColor;
	    }
	    if (event.key.keysym.sym == SDLK_RIGHT){
		switch(optionPosition){
		case 0:
		    if (FRAMES_PER_SECOND < 100) FRAMES_PER_SECOND += 5;
		    break;
		case 1:
		    if (SNAKE_CELL_SIZE < 30) SNAKE_CELL_SIZE += 1;
		    break;
		case 2:
		    sound = true;
		    break;
		}
	    }
	    if (event.key.keysym.sym == SDLK_LEFT){
		switch(optionPosition){
		case 0:
		    if (FRAMES_PER_SECOND > 10) FRAMES_PER_SECOND -= 5;
		    break;
		case 1:
		    if (SNAKE_CELL_SIZE > 2) SNAKE_CELL_SIZE -= 1;
		    break;
		case 2:
		    sound = false;
		    break;
		}
	    }
	    if (event.key.keysym.sym == SDLK_RETURN){
		if(optionPosition == 3) nextState = MENU_STATE;
	    }
	}
    }
}



int Option::option_focus() {
    for (int i = 0; i < 4; i++)	if(optionEntries[i].isFocused) return i;
    return 0;
}

void Option::render() {
    optionEntries[0].entry = TTF_RenderText_Solid(font, optionEntries[0].msg[optionEntries[0].isFocused],
						  optionEntries[0].color);
    optionEntries[1].entry = TTF_RenderText_Solid(font, optionEntries[1].msg[optionEntries[1].isFocused],
						  optionEntries[1].color);
    optionEntries[2].entry = TTF_RenderText_Solid(font, optionEntries[2].msg[optionEntries[2].isFocused],
						  optionEntries[2].color);
    optionEntries[3].entry = TTF_RenderText_Solid(font, optionEntries[3].msg[optionEntries[3].isFocused],
						  optionEntries[3].color);

    std::stringstream ss;
    std::string str;
    
    ss << FRAMES_PER_SECOND;
    str = ss.str();
    optionValues[0] = TTF_RenderText_Solid(font, str.c_str(), optionEntries[0].color);

    ss.str(std::string());
    ss.clear();
    ss << SNAKE_CELL_SIZE;
    str = ss.str();
    optionValues[1] = TTF_RenderText_Solid(font, str.c_str(), optionEntries[1].color);

    if(sound){
	if(optionEntries[2].isFocused)
	    optionValues[2] = TTF_RenderText_Solid(font, "ON", optionEntries[2].color);
	else
	    optionValues[2] = TTF_RenderText_Solid(font, "On", optionEntries[2].color);
    }
    else{
	if(optionEntries[2].isFocused)
	    optionValues[2] = TTF_RenderText_Solid(font, "OFF", optionEntries[2].color);
	else
	    optionValues[2] = TTF_RenderText_Solid(font, "Off", optionEntries[2].color);
    }
 
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));

    if (optionEntries[0].entry != NULL){
	apply_surface(OPTIONS_XPOS, OPTIONS_YPOS, optionEntries[0].entry, screen);
    }
    if (optionEntries[1].entry != NULL){
	apply_surface(OPTIONS_XPOS, OPTIONS_YPOS + OPTIONS_H, optionEntries[1].entry, screen);
    }
    if (optionEntries[2].entry != NULL){
	apply_surface(OPTIONS_XPOS, OPTIONS_YPOS + 2*OPTIONS_H, optionEntries[2].entry, screen);
    }
    if (optionEntries[3].entry != NULL){
	apply_surface(OPTIONS_XPOS, OPTIONS_YPOS + 4*OPTIONS_H, optionEntries[3].entry, screen);
    }

    if (optionValues[0] != NULL){
	apply_surface(SCREEN_WIDTH-2*OPTIONS_XPOS, OPTIONS_YPOS, optionValues[0], screen);
    }
    if (optionValues[1] != NULL){
	apply_surface(SCREEN_WIDTH-2*OPTIONS_XPOS, OPTIONS_YPOS+OPTIONS_H, optionValues[1], screen);
    }
    if (optionValues[0] != NULL){
	apply_surface(SCREEN_WIDTH-2*OPTIONS_XPOS, OPTIONS_YPOS+2*OPTIONS_H, optionValues[2], screen);
    }


    
    /* if (optionEntries[3][1].entry != NULL) {
       apply_surface((SCREEN_WIDTH - optionEntries[3][0].entry->w) / 2 + 200,
       ((SCREEN_HEIGHT - optionEntries[3][0].entry->h) / 2) + (SCREEN_WIDTH/float(32)), optionEntries[3][1].entry, screen);
       } */
}

Lose::Lose() {
    loseMsg = NULL;
    pressMsg = NULL;
    againMsg = NULL;
}

Lose::~Lose() {
    SDL_FreeSurface(loseMsg);
    SDL_FreeSurface(pressMsg);
    SDL_FreeSurface(againMsg);
}

void Lose::logic() {
}

void Lose::render() {
    loseMsg = TTF_RenderText_Solid(fontBigger, "YOU LOSE!", scoreColor);
    pressMsg = TTF_RenderText_Solid(font, "Press Spacebar", scoreColor);
    againMsg = TTF_RenderText_Solid(font, "to play again", scoreColor);
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    apply_surface((SCREEN_WIDTH - loseMsg->w) / 2, (SCREEN_WIDTH / float(6.4)), loseMsg, screen);
    apply_surface((SCREEN_WIDTH - pressMsg->w) / 2, (SCREEN_WIDTH / float(4.25)), pressMsg, screen);
    apply_surface((SCREEN_WIDTH - againMsg->w) / 2, (SCREEN_WIDTH / float(3.7)), againMsg, screen);
}

void Lose::handle_events() {
    while (SDL_PollEvent(&event)) {
	if (event.type == SDL_QUIT) {
	    Mix_CloseAudio();
	    state = EXIT_STATE;
	}
	if ((event.type == SDL_KEYDOWN)) {
	    if (event.key.keysym.sym == SDLK_SPACE) {
		nextState = PLAY_STATE;
	    }
	    if (event.key.keysym.sym == SDLK_ESCAPE) {
		nextState = EXIT_STATE;
	    }
	}
    }
}

Play::Play() {
    xSnakePos = &xSnake;
    ySnakePos = &ySnake;
    xFoodPos = &xFood;
    yFoodPos = &yFood;
    handle_high_scores();
    pauseEntries[0].color = focusOnColor;
    pauseEntries[0].entry = NULL;
    pauseEntries[0].isFocused = true;
    pauseEntries[0].msg = "CONTINUE";
    pauseEntries[1].msg = "Main Menu";
    pauseEntries[1].color = scoreColor;
    pauseEntries[1].isFocused = false;
    pauseEntries[1].entry = NULL;
    pauseEntries[2].color = scoreColor;
    pauseEntries[2].entry = NULL;
    pauseEntries[2].isFocused = false;
    pauseEntries[2].msg = "Exit";
    eatSound = Mix_LoadWAV("sounds/eat.wav");
    switchSound = Mix_LoadWAV("sounds/switch.wav");
    selectSound = Mix_LoadWAV("sounds/selected.wav");
    deathSound = Mix_LoadWAV("sounds/death.wav");
}

Play::~Play() {
    Mix_FreeChunk(eatSound);
    Mix_FreeChunk(switchSound);
    Mix_FreeChunk(selectSound);
    Mix_FreeChunk(deathSound);
}

void Play::handle_events() {
    while (SDL_PollEvent(&event)) {
	moj.handle_input();
	if (event.type == SDL_QUIT) {
	    Mix_CloseAudio();
	    nextState = EXIT_STATE;
	}
	if (paused == true) {
	    if (event.type == SDL_KEYDOWN) {
		switch (pause_focus()) {
		case 2:  // main menu
		    if (event.key.keysym.sym == SDLK_RETURN) {
			paused = false;
			Mix_PlayChannel(-1, selectSound, 0);
			SDL_Delay(300);
			nextState = MENU_STATE;
		    }
		    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
			SDL_FreeSurface(pauseEntries[1].entry);
			pauseEntries[1].msg = "Main Menu";
			pauseEntries[1].color = scoreColor;
			pauseEntries[1].isFocused = false;
			Mix_PlayChannel(-1, switchSound, 0);
		    }
		    if (event.key.keysym.sym == SDLK_DOWN) {
			SDL_FreeSurface(pauseEntries[2].entry);
			pauseEntries[2].msg = "EXIT";
			pauseEntries[2].color = focusOnColor;
			pauseEntries[2].isFocused = true;
		    }
		    if (event.key.keysym.sym == SDLK_UP) {
			SDL_FreeSurface(pauseEntries[0].entry);
			pauseEntries[0].msg = "CONTINUE";
			pauseEntries[0].color = focusOnColor;
			pauseEntries[0].isFocused = true;
		    }
		    break;
		case 1:  // continue
		    if (event.key.keysym.sym == SDLK_RETURN) {
			Mix_PlayChannel(-1, selectSound, 0);
			paused = false;
		    }
		    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
			SDL_FreeSurface(pauseEntries[0].entry);
			pauseEntries[0].msg = "Continue";
			pauseEntries[0].isFocused = false;
			pauseEntries[0].color = scoreColor;
			Mix_PlayChannel(-1, switchSound, 0);
		    }
		    if (event.key.keysym.sym == SDLK_DOWN) {
			SDL_FreeSurface(pauseEntries[1].entry);
			pauseEntries[1].msg = "MAIN MENU";
			pauseEntries[1].color = focusOnColor;
			pauseEntries[1].isFocused = true;
		    }
		    if (event.key.keysym.sym == SDLK_UP) {
			SDL_FreeSurface(pauseEntries[2].entry);
			pauseEntries[2].msg = "EXIT";
			pauseEntries[2].color = focusOnColor;
			pauseEntries[2].isFocused = true;
		    }
		    break;
		case 0:  // exit
		    if (event.key.keysym.sym == SDLK_RETURN) {
			Mix_PlayChannel(-1, selectSound, 0);
			SDL_Delay(300);
			Mix_CloseAudio();
			nextState = EXIT_STATE;
		    }
		    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_UP) {
			SDL_FreeSurface(pauseEntries[2].entry);
			pauseEntries[2].msg = "Exit";
			pauseEntries[2].isFocused = false;
			pauseEntries[2].color = scoreColor;
			Mix_PlayChannel(-1, switchSound, 0);
		    }
		    if (event.key.keysym.sym == SDLK_UP) {
			SDL_FreeSurface(pauseEntries[1].entry);
			pauseEntries[1].msg = "MAIN MENU";
			pauseEntries[1].color = focusOnColor;
			pauseEntries[1].isFocused = true;
		    }
		    if (event.key.keysym.sym == SDLK_DOWN) {
			SDL_FreeSurface(pauseEntries[0].entry);
			pauseEntries[0].msg = "CONTINUE";
			pauseEntries[0].color = focusOnColor;
			pauseEntries[0].isFocused = true;
		    }
		    break;
		}
		
	    }
	}
    }
}

int Play::pause_focus() {
    if (pauseEntries[1].isFocused == true) {
	return 2;
    }
    if (pauseEntries[0].isFocused == true) {
	return 1;
    }
    else {
	return 0;
    }
}

void Play::logic() {
    if (paused == false) {
	moj.move();
    }
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    jedzenie.get_position(xFoodPos, yFoodPos);
    moj.get_position(xSnakePos, ySnakePos);
    moj.mouth_open(moj.get_dir(), xSnake, ySnake, xFood, yFood);
    
    if (((xFood >= xSnake) && (xFood <= xSnake + SNAKE_CELL_SIZE))
	&& ((yFood >= ySnake) && (yFood <= ySnake + SNAKE_CELL_SIZE))) {
	score += 5;
	jedzenie.reset_position();
	moj.grow();
	moj.mouth_change_state(false);
	Mix_PlayChannel(-1, eatSound, 0);
    }
    if (moj.check_collision() == true) {
	Mix_PlayChannel(-1, deathSound, 0);
	SDL_Delay(300);
	if (score > highScores[9].score) {
	    highScores[9].score = score;
	    nextState = ENTRY_SCORE_STATE;
	}
	else {
	    nextState = LOSE_STATE;
	}
	score = 0;
    }
}

void Play::render() {
    if (paused == true) {
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
	pauseEntries[0].entry = TTF_RenderText_Solid(font, pauseEntries[0].msg, pauseEntries[0].color);
	pauseEntries[1].entry = TTF_RenderText_Solid(font, pauseEntries[1].msg, pauseEntries[1].color);
	pauseEntries[2].entry = TTF_RenderText_Solid(font, pauseEntries[2].msg, pauseEntries[2].color);
	if (pauseEntries[0].entry != NULL) {
	    apply_surface((SCREEN_WIDTH - pauseEntries[0].entry->w) / 2,
			  ((SCREEN_HEIGHT - pauseEntries[0].entry->h) / 2) - (SCREEN_WIDTH / float(64)),
			  pauseEntries[0].entry, screen);
	}
	if (pauseEntries[1].entry != NULL) {
	    apply_surface((SCREEN_WIDTH - pauseEntries[1].entry->w) / 2,
			  ((SCREEN_HEIGHT - pauseEntries[1].entry->h) / 2) + (SCREEN_WIDTH / float(64)),
			  pauseEntries[1].entry, screen);
	}
	if (pauseEntries[2].entry != NULL) {
	    apply_surface((SCREEN_WIDTH - pauseEntries[2].entry->w) / 2,
			  ((SCREEN_HEIGHT - pauseEntries[2].entry->h) / 2) + (SCREEN_WIDTH / float(21)),
			  pauseEntries[2].entry, screen);
	}
    }
    else if (paused == false) {
	viewScore = TTF_RenderText_Solid(font, convert_int_to_char(score, buffer), scoreColor);
	apply_surface((SCREEN_WIDTH / float(2.3)), (SCREEN_WIDTH / float(32)), viewScore, screen);
	moj.show();
	jedzenie.show();
    }
}

EnterScore::EnterScore() {
    nameContainer = "";
    name = NULL;
    message = NULL;
    SDL_EnableUNICODE(SDL_ENABLE);
}

EnterScore::~EnterScore() {
    SDL_FreeSurface(name);
    SDL_FreeSurface(message);
    SDL_EnableUNICODE(SDL_DISABLE);
}

void EnterScore::logic() {
}

void EnterScore::handle_events() {
    while (SDL_PollEvent(&event)) {
	if (event.type == SDL_QUIT) {
	    Mix_CloseAudio();
	    state = EXIT_STATE;
	}
	if (event.type == SDL_KEYDOWN) {
	    std::string tmp = nameContainer;
	    if ((event.key.keysym.sym == SDLK_RETURN) && (nameContainer.length() >= 3)) {
		highScores[9].name = nameContainer;
		update_high_scores();
		nextState = HIGH_SCORES_STATE;
	    }
	    if (nameContainer.length() <= 14) {
		if ((event.key.keysym.unicode >= (Uint16) '0') && (event.key.keysym.unicode <= (Uint16) '9')) {
		    nameContainer += (char) event.key.keysym.unicode;
		}
		else if ((event.key.keysym.unicode >= (Uint16) 'A') && (event.key.keysym.unicode <= (Uint16) 'Z')) {
		    nameContainer += (char) event.key.keysym.unicode;
		}
		else if ((event.key.keysym.unicode >= (Uint16) 'a') && (event.key.keysym.unicode <= (Uint16) 'z')) {
		    nameContainer += (char) event.key.keysym.unicode;
		}
	    }
	    if ((event.key.keysym.sym == SDLK_BACKSPACE) && (nameContainer.length() > 0)) {
		nameContainer.erase(nameContainer.length() - 1);
	    }
	    if (nameContainer != tmp) {
		SDL_FreeSurface(name);
		name = TTF_RenderText_Solid(font, nameContainer.c_str(), scoreColor);
	    }
	}
    }
}

void EnterScore::render() {
    message = TTF_RenderText_Solid(font, "New high score! enter name:", scoreColor);
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    apply_surface((SCREEN_WIDTH - message->w) / 2, ((SCREEN_HEIGHT / 2) - message->h) / 2, message, screen);
    if (name != NULL) {
	apply_surface((SCREEN_WIDTH - name->w) / 2, (((SCREEN_HEIGHT / 2) - name->h) / 2) + (SCREEN_WIDTH / float(6.4)),
		      name, screen);
    }
}

ShowHighScores::ShowHighScores() {
    message = NULL;
    playAgain = NULL;
    for (int i = 0; i < 10; i++) {
	entries[i][0] = NULL;
	entries[i][1] = NULL;
	entries[i][2] = NULL;
    }
    handle_high_scores();
}

ShowHighScores::~ShowHighScores() {
    for (int i = 0; i < 10; i++) {
	SDL_FreeSurface(entries[i][0]);
	SDL_FreeSurface(entries[i][1]);
	SDL_FreeSurface(entries[i][2]);
	free(highScores[i].count);
	free(highScores[i].scoreChar);
    }
}

void ShowHighScores::logic() {
}

void ShowHighScores::handle_events() {
    while (SDL_PollEvent(&event)) {
	if (event.type == SDL_QUIT) {
	    Mix_CloseAudio();
	    nextState = EXIT_STATE;
	}
	if (event.type == SDL_KEYDOWN) {
	    if (event.key.keysym.sym == SDLK_SPACE) {
		if (wasMenu == false) {
		    nextState = PLAY_STATE;
		}
		else if (wasMenu == true) {
		    nextState = MENU_STATE;
		    wasMenu = false;
		}
	    }
	    if (event.key.keysym.sym == SDLK_ESCAPE) {
		nextState = EXIT_STATE;
	    }
	}
    }
}

void ShowHighScores::render() {
    message = TTF_RenderText_Solid(font, "High Scores", scoreColor);
    if (wasMenu == false) {
	playAgain = TTF_RenderText_Solid(font, "Press spacebar to play again", scoreColor);
    }
    else if (wasMenu == true) {
	playAgain = TTF_RenderText_Solid(font, "Press spacebar to go back to main menu", scoreColor);
    }
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    apply_surface((SCREEN_WIDTH - message->w) / 2, (((SCREEN_HEIGHT / 2) - message->h) / 2) - (SCREEN_WIDTH / float(9)),
		  message, screen);
    apply_surface((SCREEN_WIDTH - playAgain->w) / 2, (SCREEN_WIDTH / float(1.6)), playAgain, screen);
    
    for (int i = 0; i < 10; i++) {
	entries[i][0] = TTF_RenderText_Solid(font, highScores[i].count, scoreColor);
	entries[i][1] = TTF_RenderText_Solid(font, highScores[i].name.c_str(), scoreColor);
	entries[i][2] = TTF_RenderText_Solid(font, highScores[i].scoreChar, scoreColor);
	if (entries[i][0] != NULL) {
	    apply_surface((SCREEN_WIDTH / float(3.2)), (SCREEN_WIDTH / float(6.4)) + (i * (SCREEN_WIDTH / float(32))),
			  entries[i][0], screen);
	}
	if (entries[i][1] != NULL) {
	    apply_surface((SCREEN_WIDTH / float(3.2)) + (SCREEN_WIDTH / float(12.8)),
			  (SCREEN_WIDTH / float(6.4)) + (i * (SCREEN_WIDTH / float(32))), entries[i][1], screen);
	}
	if (entries[i][2] != NULL) {
	    apply_surface((SCREEN_WIDTH / float(3.2)) + (SCREEN_WIDTH / float(3.2)),
			  (SCREEN_WIDTH / float(6.4)) + (i * (SCREEN_WIDTH / float(32))), entries[i][2], screen);
	}
    }
}
