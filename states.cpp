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

#define OPTIONS_XPOS ((SCREEN_WIDTH - 140)/2)
#define OPTIONS_YPOS (0.5*(SCREEN_HEIGHT - 9*optionEntries[0].entry->h))
#define OPTIONS_H (optionEntries[0].entry->h)
#define OPTIONS_SPACING (SCREEN_WIDTH/5)

Intro::Intro() {
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("sounds/theme.wav");
    msgAnother = TTF_RenderText_Solid(font, "Another", scoreColor);
    msgSnake = TTF_RenderText_Solid(fontBigger, "SNAKE", scoreColor);
    msgSpace = TTF_RenderText_Solid(font, "Press Spacebar", scoreColor);
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
                if (!spacebarCounter && posCounter == (SCREEN_WIDTH / float(6.4))) {
                    nextState = MENU_STATE;
                }
                ++spacebarCounter;
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
    if (!Mix_PlayingMusic()) {
        Mix_PlayMusic(music, 1);
    }
    if (posCounter != (SCREEN_WIDTH / float(6.4))) {
        --posCounter;
    }
}

void Intro::render() {
    SDL_FillRect(screen, &screen->clip_rect,
                 SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    apply_surface((SCREEN_WIDTH - msgAnother->w) / 2, 
                   posCounter, msgAnother, screen);
    apply_surface((SCREEN_WIDTH - msgSnake->w) / 2, 
                  posCounter + (SCREEN_WIDTH / float(32)), msgSnake, screen);
    if (posCounter == (SCREEN_WIDTH / float(6.4))) {
        apply_surface((SCREEN_WIDTH - msgSpace->w) / 2, 
                      (SCREEN_WIDTH / float(1.6)), msgSpace, screen);
    }
}

Menu::Menu() {
    menuEntries = new menuEntry[4];
    switchSound = Mix_LoadWAV("sounds/switch.wav");
    selectSound = Mix_LoadWAV("sounds/selected.wav");
    menuEntries[0] = menuEntry("NEW GAME", true, NULL, focusOnColor);
    menuEntries[1] = menuEntry("Options", false, NULL, scoreColor);
    menuEntries[2] = menuEntry("High Scores", false, NULL, scoreColor);
    menuEntries[3] = menuEntry("Exit", false, NULL, scoreColor);
    menuEntries[0].entry = TTF_RenderText_Solid(font, menuEntries[0].msg, menuEntries[0].color);
    menuEntries[1].entry = TTF_RenderText_Solid(font, menuEntries[1].msg, menuEntries[1].color);
    menuEntries[2].entry = TTF_RenderText_Solid(font, menuEntries[2].msg, menuEntries[2].color);
    menuEntries[3].entry = TTF_RenderText_Solid(font, menuEntries[3].msg, menuEntries[3].color);
    handle_high_scores();
    update_high_scores();
}

Menu::~Menu() {
    Mix_FreeChunk(selectSound);
    Mix_FreeChunk(switchSound);
    for (int i = 0; i < 4; ++i) {
        SDL_FreeSurface(menuEntries[i].entry);
    }
    delete [] menuEntries;
}

void Menu::logic() {}

void Menu::handle_events() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Mix_CloseAudio();
            nextState = EXIT_STATE;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (menu_focus()) {
            case 0:
                if (event.key.keysym.sym == SDLK_UP || 
                    event.key.keysym.sym == SDLK_DOWN) {
                    menuEntries[0].update_state("New Game", false, scoreColor);
                    update_surface(&menuEntries[0].entry, font, 
                                   menuEntries[0].msg, menuEntries[0].color);
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    if (sound) {
                        Mix_PlayChannel(-1, switchSound, 0);
                    }
                    menuEntries[3].update_state("EXIT", true, focusOnColor);
                    update_surface(&menuEntries[3].entry, font, 
                                   menuEntries[3].msg, menuEntries[3].color);
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    if (sound) {
                        Mix_PlayChannel(-1, switchSound, 0);
                    }
                    menuEntries[1].update_state("OPTIONS", true, focusOnColor);
                    update_surface(&menuEntries[1].entry, font, 
                                   menuEntries[1].msg, menuEntries[1].color);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (sound) {
                        Mix_PlayChannel(-1, selectSound, 0);
                        SDL_Delay(300);
                    }
                    nextState = PLAY_STATE;
                }
                break;
            case 1:
                if (event.key.keysym.sym == SDLK_UP ||
                    event.key.keysym.sym == SDLK_DOWN) {
                    menuEntries[1].update_state("Options", false, scoreColor);
                    update_surface(&menuEntries[1].entry, font, 
                                   menuEntries[1].msg, menuEntries[1].color);
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    if (sound) {
                        Mix_PlayChannel(-1, switchSound, 0);
                    }
                    menuEntries[0].update_state("NEW GAME", true, focusOnColor);
                    update_surface(&menuEntries[0].entry, font, 
                                   menuEntries[0].msg, menuEntries[0].color);
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    if (sound) {
                        Mix_PlayChannel(-1, switchSound, 0);
                    }
                    menuEntries[2].update_state("HIGH SCORES", true, focusOnColor);
                    update_surface(&menuEntries[2].entry, font, 
                                   menuEntries[2].msg, menuEntries[2].color);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (sound) {
                        Mix_PlayChannel(-1, selectSound, 0);
                        SDL_Delay(300);
                    }
                    nextState = OPTION_STATE;
                }
                break;
            case 2:
                if (event.key.keysym.sym == SDLK_UP || 
                    event.key.keysym.sym == SDLK_DOWN) {
                    menuEntries[2].update_state("High Scores", false, scoreColor);
                    update_surface(&menuEntries[2].entry, font, 
                                   menuEntries[2].msg, menuEntries[2].color);
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    if (sound) {
                        Mix_PlayChannel(-1, switchSound, 0);
                    }
                    menuEntries[1].update_state("OPTIONS", true, focusOnColor);
                    update_surface(&menuEntries[1].entry, font, 
                                   menuEntries[1].msg, menuEntries[1].color);
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    if (sound) {
                        Mix_PlayChannel(-1, switchSound, 0);
                    }
                    menuEntries[3].update_state("EXIT", true, focusOnColor);
                    update_surface(&menuEntries[3].entry, font, 
                                   menuEntries[3].msg, menuEntries[3].color);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (sound) {
                        Mix_PlayChannel(-1, selectSound, 0);
                        SDL_Delay(300);
                    }
                    wasMenu = true;
                    nextState = HIGH_SCORES_STATE;
                }
                break;
            case 3:
                if (event.key.keysym.sym == SDLK_UP || 
                    event.key.keysym.sym == SDLK_DOWN) {
                    menuEntries[3].update_state("Exit", false, scoreColor);
                    update_surface(&menuEntries[3].entry, font, 
                                   menuEntries[3].msg, menuEntries[3].color);
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    if (sound) {
                        Mix_PlayChannel(-1, switchSound, 0);
                    }
                    menuEntries[2].update_state("HIGH SCORES", true, focusOnColor);
                    update_surface(&menuEntries[2].entry, font, 
                                   menuEntries[2].msg, menuEntries[2].color);
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    if (sound) {
                        Mix_PlayChannel(-1, switchSound, 0);
                    }
                    menuEntries[0].update_state("NEW GAME", true, focusOnColor);
                    update_surface(&menuEntries[0].entry, font, 
                                   menuEntries[0].msg, menuEntries[0].color);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (sound) {
                        Mix_PlayChannel(-1, selectSound, 0);
                        SDL_Delay(300);
                    }
                    Mix_CloseAudio();
                    nextState = EXIT_STATE;
                }
                break;
            }
        }
    }

}

int Menu::menu_focus() {
    if (menuEntries[0].isFocused) { return 0; }
    else if (menuEntries[1].isFocused) { return 1; }
    else if (menuEntries[2].isFocused) { return 2; }
    else { return 3; }
}

void Menu::render() {
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    if (menuEntries[0].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[0].entry->w) / 2,
                      ((SCREEN_HEIGHT - menuEntries[0].entry->h) / 2) - 
                      (SCREEN_WIDTH / float(16)), menuEntries[0].entry, screen);
    }
    if (menuEntries[1].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[1].entry->w) / 2,
                      ((SCREEN_HEIGHT - menuEntries[1].entry->h) / 2) - 
                      (SCREEN_WIDTH / float(32)), menuEntries[1].entry, screen);
    }
    if (menuEntries[2].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[2].entry->w) / 2, 
                      ((SCREEN_HEIGHT - menuEntries[2].entry->h) / 2),
                      menuEntries[2].entry, screen);
    }
    if (menuEntries[3].entry != NULL) {
        apply_surface((SCREEN_WIDTH - menuEntries[3].entry->w) / 2,
                      ((SCREEN_HEIGHT - menuEntries[3].entry->h) / 2) + 
                      (SCREEN_WIDTH / float(32)), menuEntries[3].entry, screen);
    }
}

//Options have following format: [Option] : [Value]
//optionEntries holds [Option] strings and vals holds [Value] strings
Option::Option() {
    switchSound = Mix_LoadWAV("sounds/switch.wav");
    selectSound = Mix_LoadWAV("sounds/selected.wav");
    optionPosition = 0;
    optionEntries = new optionEntry[8];
    optionEntries[0] = optionEntry("Speed", "SPEED", true, NULL, focusOnColor);
    optionEntries[1] = optionEntry("Snake size", "SNAKE SIZE", false, NULL, scoreColor);
    optionEntries[2] = optionEntry("Sound", "SOUND", false, NULL, scoreColor);
    optionEntries[3] = optionEntry("Resolution", "RESOLUTION", false, NULL, scoreColor);
    optionEntries[4] = optionEntry("Full Screen", "FULL SCREEN", false, NULL, scoreColor);
    optionEntries[5] = optionEntry("Controls", "CONTROLS", false, NULL, scoreColor);
    optionEntries[6] = optionEntry("Background", "BACKGROUND", false, NULL, scoreColor);
    optionEntries[7] = optionEntry("Exit", "EXIT", false, NULL, scoreColor);
    for (int i = 0; i < 8; i++) {
        optionEntries[i].entry = TTF_RenderText_Solid(font, 
                                 optionEntries[i].msg[optionEntries[i].isFocused], 
                                 optionEntries[i].color);
    }
    vals.push_back(new optionValue(FRAMES_PER_SECOND, 0, 
                                   optionEntries[0].color, false));
    vals.push_back(new optionValue(SNAKE_CELL_SIZE, 0, 
                                   optionEntries[1].color, false));
    vals.push_back(new optionValue("ON", "On", "OFF", "Off", 1, 
                                   optionEntries[2].color, sound));
    vals.push_back(new optionValue(resolution[whichRes][0],resolution[whichRes][1], 
                                   0, optionEntries[3].color, false));
    vals.push_back(new optionValue("ON", "On", "OFF", "Off", 3,
                                   optionEntries[4].color, fullscreen));
    vals.push_back(new optionValue("ARROWS", "Arrows", "WSAD", "Wsad", 3,
                                   optionEntries[5].color, isArrow));
    vals.push_back(new optionValue("WHITE", "White","BLACK", "Black", 3, 
                                   optionEntries[6].color, whichBackground));
}

Option::~Option() {
    for (int i = 0; i < 7; ++i) {
        SDL_FreeSurface(optionEntries[i].entry);
        SDL_FreeSurface(vals[i]->entry);
        delete vals[i];
    }
    // EXIT surface has no corresponding value and was not freed within loop
    SDL_FreeSurface(optionEntries[7].entry);
    delete [] optionEntries;
    //delete [] vals;
    Mix_FreeChunk(selectSound);
    Mix_FreeChunk(switchSound);
}

void Option::logic() {}

void Option::handle_events() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            if (sound) {
                Mix_PlayChannel(-1, selectSound, 0);
                SDL_Delay(300);
            }
            Mix_CloseAudio();
            nextState = EXIT_STATE;
        }
        if (event.type == SDL_KEYDOWN) {

            optionPosition = option_focus();
            if (event.key.keysym.sym == SDLK_UP ||
                event.key.keysym.sym == SDLK_DOWN) {
                if (sound) {
                    Mix_PlayChannel(-1, switchSound, 0);
                }
                // unfocus current option and its value
                optionEntries[optionPosition].isFocused = false;
                optionEntries[optionPosition].color = scoreColor;
                // update option
                update_surface(&optionEntries[optionPosition].entry, font,
                               optionEntries[optionPosition].msg[0],
                               optionEntries[optionPosition].color);
                // update option value
                if (optionPosition != 7) {
                    val_update_surface(vals[optionPosition], font, 
                                       optionEntries[optionPosition].color, -1,
                                       optionPosition, false, vals[optionPosition]->enab);
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    if (optionPosition > 0)
                        --optionPosition;
                    else
                        // if on highest option, jump to the lowest one
                        optionPosition = 7;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    if (optionPosition < 7)
                        ++optionPosition;
                    else
                        // if on lowest option, jump to the highest one
                        optionPosition = 0;
                }
                // focus on new option indicated by optionPosition
                optionEntries[optionPosition].isFocused = true;
                optionEntries[optionPosition].color = focusOnColor;
                update_surface(&optionEntries[optionPosition].entry, font,
                               optionEntries[optionPosition].msg[1],
                               optionEntries[optionPosition].color);
                if (optionPosition != 7) {
                    val_update_surface(vals[optionPosition], font, 
                                       optionEntries[optionPosition].color, -1,
                                       optionPosition, true, vals[optionPosition]->enab);
                }
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                if (sound) {
                    Mix_PlayChannel(-1, switchSound, 0);
                }
                switch (optionPosition) {
                case 0:
                    if (FRAMES_PER_SECOND < 100)
                        FRAMES_PER_SECOND += 5;
                    val_update_surface(vals[0], font, 
                                   optionEntries[0].color, FRAMES_PER_SECOND,
                                   optionPosition, true, vals[0]->enab);
                    break;
                case 1:
                    if (SNAKE_CELL_SIZE < 30)
                        SNAKE_CELL_SIZE += 1;
                    val_update_surface(vals[1], font, 
                                   optionEntries[1].color, SNAKE_CELL_SIZE,
                                   optionPosition, true, vals[1]->enab);
                    break;
                case 2:
                    sound = true;
                    vals[2]->enab = true;
                    val_update_surface(vals[2], font, 
                                   optionEntries[2].color, -1,
                                   optionPosition, true, vals[2]->enab);
                    break;
                case 3:
                    if (whichRes < 7)
                        whichRes += 1;
                    val_update_surface(vals[3], font, 
                                   optionEntries[3].color, whichRes,
                                   optionPosition, true, vals[3]->enab);
                    break;
                case 4:
                    fullscreen = true;
                    vals[4]->enab = true;
                    val_update_surface(vals[4], font, 
                                   optionEntries[4].color, -1,
                                   optionPosition, true, vals[4]->enab);
                    break;
                case 5:
                    isArrow = true;
                    vals[5]->enab = true;
                    val_update_surface(vals[5], font, 
                                   optionEntries[5].color, -1,
                                   optionPosition, true, vals[5]->enab);
                    break;
                case 6:
                    whichBackground = true;
                    vals[6]->enab = true;
                    val_update_surface(vals[6], font, 
                                   optionEntries[6].color, -1,
                                   optionPosition, true, vals[6]->enab);
                    break;
                }
            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                if (sound) {
                    Mix_PlayChannel(-1, switchSound, 0);
                }
                switch (optionPosition) {
                case 0:
                    if (FRAMES_PER_SECOND > 5)
                        FRAMES_PER_SECOND -= 5;
                    val_update_surface(vals[0], font, 
                                   optionEntries[0].color, FRAMES_PER_SECOND,
                                   optionPosition, true, vals[0]->enab);
                    break;
                case 1:
                    if (SNAKE_CELL_SIZE > 2)
                        SNAKE_CELL_SIZE -= 1;
                    val_update_surface(vals[1], font, 
                                   optionEntries[1].color, SNAKE_CELL_SIZE,
                                   optionPosition, true, vals[1]->enab);
                    break;
                case 2:
                    sound = false;
                    vals[2]->enab = false;
                    val_update_surface(vals[2], font, 
                                   optionEntries[2].color, -1,
                                   optionPosition, true, vals[2]->enab);
                    break;
                case 3:
                    if (whichRes > 0)
                        whichRes -= 1;
                    break;
                case 4:
                    fullscreen = false;
                    vals[4]->enab = false;
                    val_update_surface(vals[4], font, 
                                   optionEntries[4].color, -1,
                                   optionPosition, true, vals[4]->enab);
                    break;
                case 5:
                    isArrow = false;
                    vals[5]->enab = false;
                    val_update_surface(vals[5], font, 
                                   optionEntries[5].color, -1,
                                   optionPosition, true, vals[5]->enab);
                    break;
                case 6:
                    whichBackground = false;
                    vals[6]->enab = false;
                    val_update_surface(vals[6], font, 
                                   optionEntries[6].color, -1,
                                   optionPosition, true, vals[6]->enab);
                    break;
                }
            }
            if (event.key.keysym.sym == SDLK_RETURN) {
                if (optionPosition == 7) {
                    if (sound) {
                        Mix_PlayChannel(-1, selectSound, 0);
                        SDL_Delay(300);
                    }
                    SCREEN_WIDTH = resolution[whichRes][0];
                    SCREEN_HEIGHT = resolution[whichRes][1];
                    if (SCREEN_WIDTH != 640 && SCREEN_HEIGHT != 480) {
                        toggle_fullscreen(screen);
                        TTF_CloseFont(font);
                        TTF_CloseFont(fontBigger);
                        font = TTF_OpenFont("fonts/Munro.ttf", FONT_SIZE);
                        fontBigger = TTF_OpenFont("fonts/Munro.ttf", BIGGER_FONT_SIZE);
                    }
                    nextState = MENU_STATE;
                }
            }
        }
    }
}

int Option::option_focus() {
    for (int i = 0; i < 8; i++) {
        if (optionEntries[i].isFocused)
            return i;
    }
    return 0;
}

void Option::render() {
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    for (int i = 0; i < 7; ++i) {
        if (optionEntries[i].entry != NULL) {
            apply_surface((SCREEN_WIDTH - optionEntries[0].entry->w - 
                          (SCREEN_WIDTH / 10)) / 2, OPTIONS_YPOS + i * OPTIONS_H,
                          optionEntries[i].entry, screen);
        }
        if (vals[i]->entry != NULL) {
            apply_surface(OPTIONS_XPOS + OPTIONS_SPACING, 
                          OPTIONS_YPOS + i * OPTIONS_H, 
                          vals[i]->entry, screen);
        }
    }
    // Again, the left one is EXIT
    if (optionEntries[7].entry != NULL) {
        apply_surface((SCREEN_WIDTH - optionEntries[0].entry->w - 
                      (SCREEN_WIDTH / 10)) / 2, OPTIONS_YPOS + 7 * OPTIONS_H,
                      optionEntries[7].entry, screen);
    }
}

Lose::Lose() {
    loseMsg = TTF_RenderText_Solid(fontBigger, "YOU LOSE!", scoreColor);
    pressMsg = TTF_RenderText_Solid(font, "Press Spacebar", scoreColor);
    againMsg = TTF_RenderText_Solid(font, "to play again", scoreColor);
}

Lose::~Lose() {
    SDL_FreeSurface(loseMsg);
    SDL_FreeSurface(pressMsg);
    SDL_FreeSurface(againMsg);
}

void Lose::logic() {}

void Lose::render() {
    SDL_FillRect(screen, &screen->clip_rect,
                 SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    apply_surface((SCREEN_WIDTH - loseMsg->w) / 2,
                  (SCREEN_WIDTH / float(6.4)), loseMsg, screen);
    apply_surface((SCREEN_WIDTH - pressMsg->w) / 2,
                  (SCREEN_WIDTH / float(4.25)), pressMsg, screen);
    apply_surface((SCREEN_WIDTH - againMsg->w) / 2,
                  (SCREEN_WIDTH / float(3.7)), againMsg, screen);
}

void Lose::handle_events() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Mix_CloseAudio();
            state = EXIT_STATE;
        }
        if (event.type == SDL_KEYDOWN) {
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
    //handle_high_scores();
    pauseEntries = new menuEntry[3];
    pauseEntries[0] = menuEntry("CONTINUE", true, NULL, focusOnColor);
    pauseEntries[1] = menuEntry("Main Menu", false, NULL, scoreColor);
    pauseEntries[2] = menuEntry("Exit", false, NULL, scoreColor);
    pauseEntries[0].entry = TTF_RenderText_Solid(font, pauseEntries[0].msg, pauseEntries[0].color);
    pauseEntries[1].entry = TTF_RenderText_Solid(font, pauseEntries[1].msg, pauseEntries[1].color);
    pauseEntries[2].entry = TTF_RenderText_Solid(font, pauseEntries[2].msg, pauseEntries[2].color);
    viewScore = TTF_RenderText_Solid(font, convert_int_to_char(score, buffer), scoreColor);
    eatSound = Mix_LoadWAV("sounds/eat.wav");
    switchSound = Mix_LoadWAV("sounds/switch.wav");
    selectSound = Mix_LoadWAV("sounds/selected.wav");
    deathSound = Mix_LoadWAV("sounds/death.wav");
    ericMusic = Mix_LoadMUS("sounds/eric_dizzy.wav");
}

Play::~Play() {
    SDL_FreeSurface(viewScore);
    for (int i = 0; i < 3; ++i) {
        SDL_FreeSurface(pauseEntries[i].entry);
    }
    delete [] pauseEntries;
    Mix_FreeChunk(eatSound);
    Mix_FreeChunk(switchSound);
    Mix_FreeChunk(selectSound);
    Mix_FreeChunk(deathSound);
    Mix_FreeMusic(ericMusic);
}

void Play::handle_events() {
    while (SDL_PollEvent(&event)) {
        moj.handle_input();
        if (event.type == SDL_QUIT) {
            Mix_CloseAudio();
            nextState = EXIT_STATE;
        }
        if (paused) {
            if (event.type == SDL_KEYDOWN) {
                switch (pause_focus()) {
                case 2:  // main menu
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        paused = false;
                        if (sound) {
                            Mix_PlayChannel(-1, selectSound, 0);
                            SDL_Delay(300);
                        }
                        nextState = MENU_STATE;
                        score = 0;
                    }
                    if (event.key.keysym.sym == SDLK_UP || 
                        event.key.keysym.sym == SDLK_DOWN) {
                        pauseEntries[1].update_state("Main Menu", false, scoreColor);
                        update_surface(&pauseEntries[1].entry, font,
                                       pauseEntries[1].msg, pauseEntries[1].color);
                        if (sound) {
                            Mix_PlayChannel(-1, switchSound, 0);
                        }
                    }
                    if (event.key.keysym.sym == SDLK_DOWN) {
                        pauseEntries[2].update_state("EXIT", true, focusOnColor);
                        update_surface(&pauseEntries[2].entry, font, 
                                       pauseEntries[2].msg, pauseEntries[2].color);
                    }
                    if (event.key.keysym.sym == SDLK_UP) {
                        pauseEntries[0].update_state("CONTINUE", true, focusOnColor);
                        update_surface(&pauseEntries[0].entry, font,
                                       pauseEntries[0].msg, pauseEntries[0].color);
                    }
                    break;
                case 1:  // continue
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (sound) {
                            Mix_PlayChannel(-1, selectSound, 0);
                        }
                        paused = false;
                    }
                    if (event.key.keysym.sym == SDLK_UP || 
                        event.key.keysym.sym == SDLK_DOWN) {
                        pauseEntries[0].update_state("Continue", false, scoreColor);
                        update_surface(&pauseEntries[0].entry, font,
                                       pauseEntries[0].msg, pauseEntries[0].color);
                        if (sound) {
                            Mix_PlayChannel(-1, switchSound, 0);
                        }
                    }
                    if (event.key.keysym.sym == SDLK_DOWN) {
                        pauseEntries[1].update_state("MAIN MENU", true, focusOnColor);
                        update_surface(&pauseEntries[1].entry, font,
                                       pauseEntries[1].msg, pauseEntries[1].color);
                    }
                    if (event.key.keysym.sym == SDLK_UP) {
                        pauseEntries[2].update_state("EXIT", true, focusOnColor);
                        update_surface(&pauseEntries[2].entry, font,
                                       pauseEntries[2].msg, pauseEntries[2].color);
                    }
                    break;
                case 0:  // exit
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (sound) {
                            Mix_PlayChannel(-1, selectSound, 0);
                            SDL_Delay(300);
                        }
                        Mix_CloseAudio();
                        nextState = EXIT_STATE;
                    }
                    if (event.key.keysym.sym == SDLK_DOWN || 
                        event.key.keysym.sym == SDLK_UP) {
                        pauseEntries[2].update_state("Exit", false, scoreColor);
                        update_surface(&pauseEntries[2].entry, font,
                                       pauseEntries[2].msg, pauseEntries[2].color);
                        if (sound) {
                            Mix_PlayChannel(-1, switchSound, 0);
                        }
                    }
                    if (event.key.keysym.sym == SDLK_UP) {
                        pauseEntries[1].update_state("MAIN MENU", true, focusOnColor);
                        update_surface(&pauseEntries[1].entry, font,
                                       pauseEntries[1].msg, pauseEntries[1].color);
                    }
                    if (event.key.keysym.sym == SDLK_DOWN) {
                        pauseEntries[0].update_state("CONTINUE", true, focusOnColor);
                        update_surface(&pauseEntries[0].entry, font,
                                       pauseEntries[0].msg, pauseEntries[0].color);
                    }
                    break;
                }

            }
        }
    }
}

int Play::pause_focus() {
    if (pauseEntries[1].isFocused) { return 2; }
    if (pauseEntries[0].isFocused) { return 1; }
    else { return 0; }
}

void Play::logic() {
    if (!paused) {
        moj.move();
        if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(ericMusic, -1);
        }
        else {
            Mix_ResumeMusic();
        }
    }
    else {
        Mix_PauseMusic();
    }
    SDL_FillRect(screen, &screen->clip_rect,
                 SDL_MapRGB(screen->format, backgrounds[whichBackground][0], 
                                            backgrounds[whichBackground][1],
                                            backgrounds[whichBackground][2])
                );
    jedzenie.get_position(xFoodPos, yFoodPos);
    moj.get_position(xSnakePos, ySnakePos);
    moj.mouth_open(moj.get_dir(), xSnake, ySnake, xFood, yFood);

    if (((xFood >= xSnake) && (xFood <= xSnake + SNAKE_CELL_SIZE))
            && ((yFood >= ySnake) && (yFood <= ySnake + SNAKE_CELL_SIZE))) {
        score += FRAMES_PER_SECOND / 4;
        update_surface(&viewScore, font, convert_int_to_char(score, buffer), 
                       scoreColor);
        jedzenie.reset_position();
        moj.grow();
        moj.mouth_change_state(false);
        if (sound) {
            Mix_PlayChannel(-1, eatSound, 0);
        }
    }
    if (moj.check_collision()) {
        if (sound) {
            Mix_PlayChannel(-1, deathSound, 0);
            SDL_Delay(300);
        }
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
    if (paused) {
        SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
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
    else if (!paused) {
        apply_surface((SCREEN_WIDTH / float(2.3)), (SCREEN_WIDTH / float(32)), viewScore, screen);
        moj.show();
        jedzenie.show();
    }
}

EnterScore::EnterScore() {
    nameContainer = "";
    name = NULL;
    message = TTF_RenderText_Solid(font, "New high score! enter name:", scoreColor);
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
            if ((event.key.keysym.sym == SDLK_RETURN) && 
                (nameContainer.length() >= 3)) {
                highScores[9].name = nameContainer;
                update_high_scores();
                nextState = HIGH_SCORES_STATE;
            }
            if (nameContainer.length() <= 14) {
                if ((event.key.keysym.unicode >= (Uint16) '0') && 
                    (event.key.keysym.unicode <= (Uint16) '9')) {
                    nameContainer += (char) event.key.keysym.unicode;
                }
                else if ((event.key.keysym.unicode >= (Uint16) 'A') && 
                         (event.key.keysym.unicode <= (Uint16) 'Z')) {
                    nameContainer += (char) event.key.keysym.unicode;
                }
                else if ((event.key.keysym.unicode >= (Uint16) 'a') && 
                         (event.key.keysym.unicode <= (Uint16) 'z')) {
                    nameContainer += (char) event.key.keysym.unicode;
                }
            }
            if ((event.key.keysym.sym == SDLK_BACKSPACE) && 
                (nameContainer.length() > 0)) {
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
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    apply_surface((SCREEN_WIDTH - message->w) / 2,
                  ((SCREEN_HEIGHT / 2) - message->h) / 2, message, screen);
    if (name != NULL) {
        apply_surface((SCREEN_WIDTH - name->w) / 2, 
                      (((SCREEN_HEIGHT / 2) - name->h) / 2) + 
                      (SCREEN_WIDTH / float(6.4)), name, screen);
    }
}

ShowHighScores::ShowHighScores() {
    message = TTF_RenderText_Solid(font, "High Scores", scoreColor);
    if (!wasMenu) {
        playAgain = TTF_RenderText_Solid(font, 
                                         "Press spacebar to play again", 
                                         scoreColor);
    }
    else if (wasMenu) {
        playAgain = TTF_RenderText_Solid(font, 
                                         "Press spacebar to go back to main menu", 
                                         scoreColor);
    }
    for (int i = 0; i < 10; i++) {
        entries[i][0] = TTF_RenderText_Solid(font, highScores[i].count, scoreColor);
        entries[i][1] = TTF_RenderText_Solid(font, highScores[i].name.c_str(), scoreColor);
        entries[i][2] = TTF_RenderText_Solid(font, highScores[i].scoreChar, scoreColor);
    }
    //handle_high_scores();
}

ShowHighScores::~ShowHighScores() {
    SDL_FreeSurface(message);
    SDL_FreeSurface(playAgain);
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
                if (!wasMenu) {
                    nextState = PLAY_STATE;
                }
                else if (wasMenu) {
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
    SDL_FillRect(screen, &screen->clip_rect, 
                 SDL_MapRGB(screen->format, 0x01, 0x01, 0x01));
    apply_surface((SCREEN_WIDTH - message->w) / 2, 
                  (((SCREEN_HEIGHT / 2) - message->h) / 2) - 
                  (SCREEN_WIDTH / float(9)), message, screen);
    apply_surface((SCREEN_WIDTH - playAgain->w) / 2, 
                  (SCREEN_WIDTH / float(1.6)), playAgain, screen);

    for (int i = 0; i < 10; ++i) {
        if (entries[i][0] != NULL) {
            apply_surface((SCREEN_WIDTH / float(3.2)), 
                          (SCREEN_WIDTH / float(6.4)) +
                          (i * (SCREEN_WIDTH / float(32))), entries[i][0], screen);
        }
        if (entries[i][1] != NULL) {
            apply_surface((SCREEN_WIDTH / float(3.2)) + (SCREEN_WIDTH / float(12.8)),
                          (SCREEN_WIDTH / float(6.4)) + (i * (SCREEN_WIDTH / float(32))),
                          entries[i][1], screen);
        }
        if (entries[i][2] != NULL) {
            apply_surface((SCREEN_WIDTH / float(3.2)) + (SCREEN_WIDTH / float(3.2)),
                          (SCREEN_WIDTH / float(6.4)) + (i * (SCREEN_WIDTH / float(32))),
                          entries[i][2], screen);
        }
    }
}
