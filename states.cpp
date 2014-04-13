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

Intro::Intro() {
    Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
    music = Mix_LoadMUS("sounds/theme.wav");
    background = load_Image("images/background.png");
    msgAnother = NULL;
    msgSnake = NULL;
    msgSpace = NULL;
    posCounter = 400;
    spacebarCounter = 0;
}

Intro::~Intro() {
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_FreeSurface(background);
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
                if (spacebarCounter == 0 && posCounter == 100) {
                    nextState = MENU_STATE;
                }
                spacebarCounter++;
            }
            if (spacebarCounter == 1) {
                posCounter = 100;
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
    if (posCounter != 100) {
        posCounter--;
    }
}

void Intro::render() {
    msgAnother = TTF_RenderText_Solid(font, "Another", scoreColor);
    msgSnake = TTF_RenderText_Solid(fontBigger, "SNAKE", scoreColor);
    msgSpace = TTF_RenderText_Solid(font, "Press Spacebar", scoreColor);
    apply_surface(0, 0, background, screen);
    apply_surface((SCREEN_WIDTH - msgAnother->w) / 2, posCounter, msgAnother, screen);
    apply_surface((SCREEN_WIDTH - msgSnake->w) / 2, posCounter+20, msgSnake, screen);
    if (posCounter == 100) {
        apply_surface((SCREEN_WIDTH - msgSpace->w) / 2, 400, msgSpace, screen);
    }
}

Menu::Menu() {
    Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
    background = load_Image("images/background.png");
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
    SDL_FreeSurface(background);
    Mix_FreeChunk(selectSound);
    Mix_FreeChunk(switchSound);
    Mix_CloseAudio();
}

void Menu::logic() {

}

void Menu::handle_events() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
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
                    nextState = EXIT_STATE;
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
    apply_surface(0, 0, background, screen);
    if (menuEntries[0].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[0].entry->w) / 2,
                    ((SCREEN_HEIGHT - menuEntries[0].entry->h) / 2) - 40, menuEntries[0].entry, screen);
    }
    if (menuEntries[1].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[1].entry->w) / 2,
                    ((SCREEN_HEIGHT - menuEntries[1].entry->h) / 2) - 20, menuEntries[1].entry, screen);
    }
    if (menuEntries[2].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[2].entry->w) / 2,
                    ((SCREEN_HEIGHT - menuEntries[2].entry->h) / 2), menuEntries[2].entry, screen);
    }
    if (menuEntries[3].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[3].entry->w) / 2,
                    ((SCREEN_HEIGHT - menuEntries[3].entry->h) / 2) + 20, menuEntries[3].entry, screen);
    }
}

Lose::Lose() {
    background = load_Image("images/background.png");
    loseMsg = NULL;
    pressMsg = NULL;
    againMsg = NULL;
}

Lose::~Lose() {
    SDL_FreeSurface(background);
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
    apply_surface(0, 0, background, screen);
    apply_surface((SCREEN_WIDTH - loseMsg->w) / 2, 100, loseMsg, screen);
    apply_surface((SCREEN_WIDTH - pressMsg->w) / 2, 150, pressMsg, screen);
    apply_surface((SCREEN_WIDTH - againMsg->w) / 2, 170, againMsg, screen);
}

void Lose::handle_events() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
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
    pauseBackground = load_Image("images/background.png");
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
    Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
    eatSound = Mix_LoadWAV("sounds/eat.wav");
    switchSound = Mix_LoadWAV("sounds/switch.wav");
    selectSound = Mix_LoadWAV("sounds/selected.wav");
}

Play::~Play() {
    SDL_FreeSurface(pauseBackground);
    Mix_FreeChunk(eatSound);
    Mix_FreeChunk(switchSound);
    Mix_FreeChunk(selectSound);
    Mix_CloseAudio();
}

void Play::handle_events() {
    while (SDL_PollEvent(&event)) {
        moj.handle_input();
        if (event.type == SDL_QUIT) {
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
    SDL_FillRect(screen, &screen->clip_rect,
            SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    jedzenie.get_position(xFoodPos, yFoodPos);
    moj.get_position(xSnakePos, ySnakePos);
    moj.mouth_open(moj.get_dir(), xSnake, ySnake, xFood, yFood);

    if (((xFood >= xSnake) && (xFood <= xSnake + 5))
            && ((yFood >= ySnake) && (yFood <= ySnake + 5))) {
        score += 5;
        jedzenie.reset_position();
        moj.grow();
        moj.mouth_change_state(false);
        Mix_PlayChannel(-1, eatSound, 0);
    }
    if (moj.check_collision() == true) {
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
        apply_surface(0,0, pauseBackground, screen);
        pauseEntries[0].entry = TTF_RenderText_Solid(font, pauseEntries[0].msg, pauseEntries[0].color);
        pauseEntries[1].entry = TTF_RenderText_Solid(font, pauseEntries[1].msg, pauseEntries[1].color);
        pauseEntries[2].entry = TTF_RenderText_Solid(font, pauseEntries[2].msg, pauseEntries[2].color);
        if (pauseEntries[0].entry != NULL) {
            apply_surface((SCREEN_WIDTH - pauseEntries[0].entry->w) / 2,
                    ((SCREEN_HEIGHT - pauseEntries[0].entry->h) / 2) - 10, pauseEntries[0].entry, screen);
        }
        if (pauseEntries[1].entry != NULL) {
            apply_surface((SCREEN_WIDTH - pauseEntries[1].entry->w) / 2,
                    ((SCREEN_HEIGHT - pauseEntries[1].entry->h) / 2) + 10, pauseEntries[1].entry, screen);
        }
        if (pauseEntries[2].entry != NULL) {
            apply_surface((SCREEN_WIDTH - pauseEntries[2].entry->w) / 2,
                    ((SCREEN_HEIGHT - pauseEntries[2].entry->h) / 2) + 30, pauseEntries[2].entry, screen);
        }
    }
    else if (paused == false) {
        viewScore = TTF_RenderText_Solid(font, convert_int_to_char(score, buffer), scoreColor);
        apply_surface(280, 20, viewScore, screen);
        moj.show();
        jedzenie.show();
    }
}

EnterScore::EnterScore() {
    background = load_Image("images/background.png");
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

void EnterScore::logic() {}

void EnterScore::handle_events() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
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
                if ((event.key.keysym.unicode >= (Uint16)'0') && (event.key.keysym.unicode <= (Uint16)'9')) {
                    nameContainer += (char)event.key.keysym.unicode;
                }
                else if ((event.key.keysym.unicode >= (Uint16)'A') && (event.key.keysym.unicode <= (Uint16)'Z')) {
                    nameContainer += (char)event.key.keysym.unicode;
                }
                else if ((event.key.keysym.unicode >= (Uint16)'a') && (event.key.keysym.unicode <= (Uint16)'z')) {
                    nameContainer += (char)event.key.keysym.unicode;
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
    apply_surface(0, 0, background, screen);
    apply_surface((SCREEN_WIDTH - message->w) / 2, ((SCREEN_HEIGHT / 2) - message->h) / 2, message, screen);
    if (name != NULL) {
        apply_surface((SCREEN_WIDTH - name->w) / 2, (((SCREEN_HEIGHT / 2) - name->h) / 2)+100, name, screen);
    }
}

ShowHighScores::ShowHighScores() {
    background = load_Image("images/background.png");
    message = NULL;
    playAgain = NULL;
    for (int i = 0; i < 10; i++) {
        entries[i] = NULL;
    }
    handle_high_scores();
}

ShowHighScores::~ShowHighScores() {
    SDL_FreeSurface(background);
    for (int i = 0; i < 10; i++) {
        SDL_FreeSurface(entries[i]);
    }
}

void ShowHighScores::logic() {}

void ShowHighScores::handle_events() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            nextState = EXIT_STATE;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {
                if (wasMenu == false) {
                    nextState = PLAY_STATE;
                }
                else if (wasMenu == true) {
                    nextState = MENU_STATE;
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
    apply_surface(0, 0, background, screen);
    apply_surface((SCREEN_WIDTH - message->w) / 2, (((SCREEN_HEIGHT / 2) - message->h) / 2) - 70, message, screen);
    apply_surface(200, 400, playAgain, screen);



    for (int i = 0; i < 10; i++) {
        entries[i] = TTF_RenderText_Solid(font, scoresToView[i].c_str(), scoreColor);
        if (entries[i] != NULL) {
            apply_surface(100, 100+(i*20), entries[i], screen);
        }
    }
}
