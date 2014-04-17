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
}
;

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination,
        SDL_Rect* clip) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, clip, destination, &offset);
}
;

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
    font = TTF_OpenFont("fonts/Munro.ttf", SCREEN_WIDTH/float(25.6));
    if (font == NULL) {
        return false;
    }
    fontBigger = TTF_OpenFont("fonts/Munro.ttf", SCREEN_WIDTH/float(14.2));
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
    Mix_CloseAudio();
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
        f >> highScores[i].name >> highScores[i].score;
    }
    f.close();
    qsort(highScores, 10, sizeof(scoreEntry), cmpfunc);
}

void update_high_scores() {
    qsort(highScores, 10, sizeof(scoreEntry), cmpfunc);
    std::ofstream f("highscores.txt");
    for (int i = 0; i < 10; i++) {
        f << highScores[i].name << " " << highScores[i].score << std::endl;
        std::stringstream tmp;
        tmp << i+1 << ". " << highScores[i].name << " " << highScores[i].score;
        scoresToView[i] = tmp.str();
    }
    f.close();
}
