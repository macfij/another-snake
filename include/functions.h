#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>
#include "globals.h"
#include <fstream>
#include <stdlib.h>

SDL_Surface* load_Image(std::string filename);
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination,
                   SDL_Rect* clip = NULL);
bool init();
bool load_files();
void clean_up();
const char* convert_int_to_char(int x, char buffer[]);
int cmpfunc(const void* a, const void* b);
void handle_high_scores();
void update_high_scores();
void toggle_fullscreen(SDL_Surface*);
void update_surface(SDL_Surface** sur, TTF_Font* f, const char* t, SDL_Color c);
void val_update_surface(optionValue* v, TTF_Font* f, SDL_Color c, int isNum, 
                        int optPos, bool focus, bool enable);

#endif

