#ifndef multi_menu_h
#define multi_menu_h

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"

int displayMenuMulti(int x, int y);
void dispJoinMenu(SDL_Renderer *renderer, int x, int y);
void dispMultiMenu(SDL_Renderer *renderer, int x, int y);
void dispHostMenu(SDL_Renderer *renderer, int x, int y, int index);
void freeMultiMenuTextures();
void loadMultiMenuTextures(SDL_Renderer *renderer);


#endif