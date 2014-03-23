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
}

Play::~Play() {
}

void Play::handle_events() {
    while (SDL_PollEvent(&event)) {
        moj.handle_input();
        if (event.type == SDL_QUIT) {
            nextState = EXIT_STATE;
        }
    }
}

void Play::logic() {
    moj.move();
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
    const char* c = (const char*)convert_int_to_char(score);
    viewScore = TTF_RenderText_Solid(font, c, scoreColor);
//    if (viewScore == NULL) {
//        std::cout
//    }
    apply_surface(280, 20, viewScore, screen);
    moj.show();
    jedzenie.show();
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
        }
    }
}

void ShowHighScores::render() {
    message = TTF_RenderText_Solid(font, "High Scores", scoreColor);
    apply_surface(0, 0, background, screen);
    apply_surface((SCREEN_WIDTH - message->w) / 2, ((SCREEN_HEIGHT / 2) - message->h) / 2, message, screen);



    for (int i = 0; i < 10; i++) {
        entries[i] = TTF_RenderText_Solid(font, scoresToView[i].c_str(), scoreColor);
        if (entries[i] != NULL) {
            apply_surface(100, 200+(i*20), entries[i], screen);
        }
    }
}
