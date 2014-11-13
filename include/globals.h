#ifndef GLOBALS_H
#define GLOBALS_H

//The header
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "timer.h"
#include <string>
#include <stdlib.h>
#include <vector>
#include <boost/lexical_cast.hpp>

using boost::lexical_cast;


#define FONT_SIZE (SCREEN_WIDTH/float(25.6))
#define BIGGER_FONT_SIZE (SCREEN_WIDTH/float(14.2))
extern TTF_Font* font;
extern TTF_Font* fontBigger;

enum PossibleStates{
    NULL_STATE,
    INTRO_STATE,
    MENU_STATE,
    OPTION_STATE,
    PLAY_STATE,
    LOSE_STATE,
    ENTRY_SCORE_STATE,
    HIGH_SCORES_STATE,
    EXIT_STATE,
};

typedef struct scoreEntry {
    int score;
    char* count;
    std::string name;
    char* scoreChar;
} scoreEntry;

class menuEntry {
public:
    const char* msg;
    bool isFocused;
    SDL_Surface* entry;
    SDL_Color color;
    menuEntry() : msg(NULL), isFocused(false), entry(NULL), color({0,0,0,0}) {}
    menuEntry(const char* msg_, bool isFocused_, 
              SDL_Surface* entry_, SDL_Color color_) : 
              msg(msg_), isFocused(isFocused_), entry(entry_), color(color_) {}
    void update_state(const char* msgNew, bool focusNew, SDL_Color colorNew) {
        msg = msgNew;
        isFocused = focusNew;
        color = colorNew;
    }
};

class optionEntry {
public:
    const char* msg[2];
    bool isFocused;
    SDL_Surface* entry;
    SDL_Color color;
    optionEntry() : isFocused(false), entry(NULL), color({0,0,0,0}) {
        msg[0] = NULL;
        msg[1] = NULL;
    }
    optionEntry(const char* msg1, const char* msg2, bool foc, SDL_Surface* en,
                SDL_Color col) : isFocused(foc), entry(en), color(col) {
        msg[0] = msg1;
        msg[1] = msg2;
    }
};

class optionValue {
public:
    std::string curr;
    SDL_Surface* entry;
    bool enab;
    std::vector<std::string> msg;
    optionValue() : entry(NULL) {}
    optionValue(const char* m1, const char* m2, const char* m3, const char* m4,
                int idx, SDL_Color c, bool f) : enab(f) {
        msg.push_back(lexical_cast<std::string>(m1));
        msg.push_back(lexical_cast<std::string>(m2));
        msg.push_back(lexical_cast<std::string>(m3));
        msg.push_back(lexical_cast<std::string>(m4));
        fill_surface(idx, c);
    }
    optionValue(int m1, int idx, SDL_Color c, bool f) : enab(f) {
        msg.push_back(lexical_cast<std::string>(m1));
        fill_surface(idx, c);
    }
    optionValue(int m1, int m2, int idx, SDL_Color c, bool f) : enab(f) {
        msg.push_back(lexical_cast<std::string>(m1) + "x" +
                      lexical_cast<std::string>(m2));
        fill_surface(idx, c);
    }
    optionValue(const char* m1, int idx, SDL_Color c, bool f) : enab(f) {
        msg.push_back(lexical_cast<std::string>(m1));
        fill_surface(idx, c);
    }
    void fill_surface(int idx, SDL_Color c) {
        this->curr = this->msg[idx];
        this->entry = TTF_RenderText_Solid(font, 
                                           const_cast<const char*>(this->curr.c_str()),
                                           c);

    }
};

extern scoreEntry highScores[10];

extern SDL_Surface* screen;
extern SDL_Event event;
extern SDL_Surface* viewScore;
extern SDL_Color scoreColor;
extern SDL_Color focusOnColor;
extern int score;

extern int* xFoodPos,* yFoodPos;
extern int xSnake, ySnake, xFood, yFood;
extern int* xSnakePos,* ySnakePos;

extern int frame;
extern bool cap;
extern bool wasMenu;

extern int state;
extern int nextState;

extern Timer fps;

extern bool paused;
extern char buffer[10];

extern int SNAKE_CELL_SIZE;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int FRAMES_PER_SECOND;

extern bool sound;
extern bool fullscreen;
extern bool isArrow;

extern int resolution[8][2];
extern int whichRes;

extern Uint8 backgrounds[2][3];
extern bool whichBackground;

#endif
