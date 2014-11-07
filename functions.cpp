#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <sstream>
#include <fstream>
#include "constants.h"
#include "functions.h"
#include "globals.h"
#include <stdio.h>


SDL_Surface *load_Image(std::string filename) {
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load(filename.c_str());

    if (loadedImage != NULL) {
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);

        if (optimizedImage != NULL) {
            Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF);
            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
        }
    }
    return optimizedImage;
};

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination,
                   SDL_Rect* clip) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, clip, destination, &offset);
}

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
            SDL_SWSURFACE);
//    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
//                SDL_FULLSCREEN);
    if (screen == NULL)
        return false;
    /* ***do Opcji 3***
     informacje = SDL_GetVideoInfo();
     kolorkey = SDL_MapRGB(informacje->vfmt,0xFF,0,0xFF);
     */
    if (TTF_Init() == -1) {
        return false;
    }
    SDL_WM_SetCaption("Another Snake", NULL);
    return true;
}

bool load_files() {
    font = TTF_OpenFont("fonts/Munro.ttf", FONT_SIZE);
    if (font == NULL) return false; 
    fontBigger = TTF_OpenFont("fonts/Munro.ttf", BIGGER_FONT_SIZE);
    if (fontBigger == NULL) return false; 
    /* ***do Opcji 3***
     opened_mouth = load_Image("opened_mouth.png");
     if(opened_mouth == NULL) return false;
     closed_mouth = load_Image("closed_mouth.png");
     if(opened_mouth == NULL) return false;
     */
    return true;
}

void clean_up() {
    /* ***do Opcji 3***
     SDL_FreeSurface(opened_mouth);
     SDL_FreeSurface(closed_mouth);
     */
    TTF_CloseFont(font);
    TTF_CloseFont(fontBigger);
    TTF_Quit();
    Mix_CloseAudio();
    SDL_FreeSurface(screen);
    SDL_Quit();
}

const char* convert_int_to_char(int x, char buffer[]) {
    sprintf(buffer, "Score: %d", x);
    return buffer;
}

int cmpfunc(const void* a, const void* b) {
    scoreEntry* x1 = (scoreEntry*)a;
    scoreEntry* x2 = (scoreEntry*)b;
    return (x2->score - x1->score);
}

void handle_high_scores() {
    std::ifstream f("highscores.txt");
    for (int i = 0; i < 10; i++) {
        highScores[i].count = (char*)malloc(sizeof(char) * 4);
        highScores[i].scoreChar = (char*)malloc(sizeof(char) * 6);
        f >> highScores[i].name >> highScores[i].score;
        sprintf(highScores[i].count, "%d.", i+1);
        sprintf(highScores[i].scoreChar, "%d", highScores[i].score);
    }
    f.close();
    qsort(highScores, 10, sizeof(scoreEntry), cmpfunc);
}

void update_high_scores() {
    qsort(highScores, 10, sizeof(scoreEntry), cmpfunc);
    std::ofstream f("highscores.txt");
    for (int i = 0; i < 10; i++) {
        f << highScores[i].name << " " << highScores[i].score << std::endl;
    }
    f.close();
}

void toggle_fullscreen(SDL_Surface* screen) {
    SDL_Surface* tmp;
    if (fullscreen) {
        tmp = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                    SDL_FULLSCREEN);
        if (tmp) {
            SDL_FreeSurface(screen);
            screen = tmp;
        }
    }
    else {
        tmp = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                    SDL_SWSURFACE);
        if (tmp) {
            SDL_FreeSurface(screen);
            screen = tmp;
        }
    }
}

void update_surface(SDL_Surface** sur, TTF_Font* f, const char* t, SDL_Color c) {
    SDL_Surface* tmp = TTF_RenderText_Solid(f, t, c);
    if (tmp) {
        SDL_FreeSurface(*sur);
        *sur = tmp;
    }
}

void val_update_surface(optionValue* v, TTF_Font* f, SDL_Color c, int isNum, 
                        int optPos, bool focus, bool enable) {
    // do nothing if EXIT option
    if (optPos == 7) return;
    std::string local;
    // case resolution
    if (optPos == 3) {
        if (isNum != -1) {
            local = lexical_cast<std::string>(resolution[isNum][0]) +
                    "x" + lexical_cast<std::string>(resolution[isNum][1]);
            update_surface(&(v->entry), f, local.c_str(), c);
        }
        else {
            update_surface(&(v->entry), f, v->msg[0].c_str(), c);
        }
        return;
    }
    // update int on surface - case Speed and Snake size;
    if (optPos == 0 || optPos == 1) {
        if (isNum != -1) {
            local = lexical_cast<std::string>(isNum);
            update_surface(&(v->entry), f, local.c_str(), c);
            v->curr = local;
        }
        else {
            update_surface(&(v->entry), f, v->curr.c_str(), c);
        }
    }
    // update other value surfaces
    else {
        if (enable) {
            if (focus) {
                update_surface(&(v->entry), f, v->msg[0].c_str(), c);
            }
            else {
                update_surface(&(v->entry), f, v->msg[1].c_str(), c);
            }
        }
        else {
            if (focus) {
                update_surface(&(v->entry), f, v->msg[2].c_str(), c);
            }
            else {
                update_surface(&(v->entry), f, v->msg[3].c_str(), c);
            }
        }

    }
}
