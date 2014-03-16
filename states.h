#ifndef STATES_H_
#define STATES_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "globals.h"
#include "snake.h"
#include "pokarm.h"

class GameState{
public:
	virtual ~GameState(){};
	virtual void logic() = 0;
	virtual void render() = 0;
	virtual void handle_events() = 0;
};

class Intro : public GameState{
public:
	Intro();
	~Intro();
	void logic();
	void render();
	void handle_events();
private:
	SDL_Surface* background;
};

class Play : public GameState{
public:
	Play();
	~Play();
	void logic();
	void render();
	void handle_events();
private:
		Snake moj;
		Pokarm jedzenie;
};

class Lose : public GameState{
public:
	Lose();
	~Lose();
	void logic();
	void render();
	void handle_events();
private:
	SDL_Surface* background;
};

#endif
