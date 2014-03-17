#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include "constants.h"
#include "functions.h"
#include "globals.h"

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
    if (screen == NULL)
        return false;
    /* ***do Opcji 3***
     informacje = SDL_GetVideoInfo();
     kolorkey = SDL_MapRGB(informacje->vfmt,0xFF,0,0xFF);
     */
    SDL_WM_SetCaption("Wonsz apgrejded", NULL);
    return true;
}

bool load_files() {
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
    SDL_Quit();
}
