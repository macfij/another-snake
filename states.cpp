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
    background = load_Image("images/intro.png");
}

Intro::~Intro() {
    SDL_FreeSurface(background);
}

void Intro::handle_events() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            state = EXIT_STATE;
        }
        if ((event.type == SDL_KEYDOWN)
                && (event.key.keysym.sym == SDLK_SPACE)) {
            nextState = PLAY_STATE;
        }
    }
}

void Intro::logic() {
}

void Intro::render() {
    apply_surface(0, 0, background, screen);
}

Lose::Lose() {
    background = load_Image("images/lose.png");
}

Lose::~Lose() {
    SDL_FreeSurface(background);
}

void Lose::logic() {
}

void Lose::render() {
    apply_surface(0, 0, background, screen);
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
    pauseEntries[1].color = scoreColor;
    pauseEntries[1].entry = NULL;
    pauseEntries[1].isFocused = false;
    pauseEntries[1].msg = "Exit";
}

Play::~Play() {
    SDL_FreeSurface(pauseBackground);
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
                case 1:
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        paused = false;
                    }
                    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
                        SDL_FreeSurface(pauseEntries[0].entry);
                        SDL_FreeSurface(pauseEntries[1].entry);
                        pauseEntries[0].msg = "Continue";
                        pauseEntries[1].msg = "EXIT";
                        pauseEntries[0].isFocused = false;
                        pauseEntries[0].color = scoreColor;
                        pauseEntries[1].isFocused = true;
                        pauseEntries[1].color = focusOnColor;
                    }
                    break;
                case 0:
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        nextState = EXIT_STATE;
                    }
                    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_UP) {
                        SDL_FreeSurface(pauseEntries[0].entry);
                        SDL_FreeSurface(pauseEntries[1].entry);
                        pauseEntries[0].msg = "CONTINUE";
                        pauseEntries[1].msg = "Exit";
                        pauseEntries[0].isFocused = true;
                        pauseEntries[0].color = focusOnColor;
                        pauseEntries[1].isFocused = false;
                        pauseEntries[1].color = scoreColor;
                    }
                    break;
                }

            }
        }
    }
}

int Play::pause_focus() {
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
        if (pauseEntries[0].entry != NULL) {
            apply_surface((SCREEN_WIDTH - pauseEntries[0].entry->w) / 2,
                    ((SCREEN_HEIGHT - pauseEntries[0].entry->h) / 2) - 10, pauseEntries[0].entry, screen);
        }
        if (pauseEntries[1].entry != NULL) {
            apply_surface((SCREEN_WIDTH - pauseEntries[1].entry->w) / 2,
                    ((SCREEN_HEIGHT - pauseEntries[1].entry->h) / 2) + 10, pauseEntries[1].entry, screen);
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
                nextState = PLAY_STATE;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                nextState = EXIT_STATE;
            }
        }
    }
}

void ShowHighScores::render() {
    message = TTF_RenderText_Solid(font, "High Scores", scoreColor);
    playAgain = TTF_RenderText_Solid(font, "Press spacebar to play again", scoreColor);
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
