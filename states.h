#ifndef STATES_H_
#define STATES_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "globals.h"
#include "snake.h"
#include "pokarm.h"
#include <string>

class GameState {
public:
    virtual ~GameState() {
    }
    ;
    virtual void logic() = 0;
    virtual void render() = 0;
    virtual void handle_events() = 0;
};

class Intro: public GameState {
public:
    Intro();
    ~Intro();
    void logic();
    void render();
    void handle_events();
private:
    Mix_Music* music;
    SDL_Surface* msgAnother;
    SDL_Surface* msgSnake;
    SDL_Surface* msgSpace;
    int posCounter;
    int spacebarCounter;

};

class Menu: public GameState {
public:
    Menu();
    ~Menu();
    void logic();
    void render();
    void handle_events();
    int menu_focus();
private:
    menuEntry menuEntries[4];
    Mix_Chunk* switchSound;
    Mix_Chunk* selectSound;
};

class Play: public GameState {
public:
    Play();
    ~Play();
    void logic();
    void render();
    void handle_events();
    int pause_focus();
private:
    Snake moj;
    Pokarm jedzenie;
    menuEntry pauseEntries[3];
    Mix_Chunk* eatSound;
    Mix_Chunk* switchSound;
    Mix_Chunk* selectSound;
    Mix_Chunk* deathSound;
};

class Option: public GameState {
public:
    Option();
    ~Option();
    void logic();
    void render();
    void handle_events();
    option option_focus();
    
private:
    option optionPosition;
    option newPosition;
    menuEntry optionEntries[4][2];
    Mix_Chunk* switchSound;
    Mix_Chunk* selectSound;
};

class Lose: public GameState {
public:
    Lose();
    ~Lose();
    void logic();
    void render();
    void handle_events();
private:
    SDL_Surface* loseMsg;
    SDL_Surface* pressMsg;
    SDL_Surface* againMsg;
};

class EnterScore: public GameState {
public:
    EnterScore();
    ~EnterScore();
    void logic();
    void render();
    void handle_events();
private:
    SDL_Surface* message;
    SDL_Surface* name;
    std::string nameContainer;
};

class ShowHighScores: public GameState {
public:
    ShowHighScores();
    ~ShowHighScores();
    void logic();
    void render();
    void handle_events();
private:
    SDL_Surface* entries[10][3];
    SDL_Surface* message;
    SDL_Surface* playAgain;
};
#endif
