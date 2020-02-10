#ifndef graphics_h
#define graphics_h

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"

void displayText(SDL_Renderer *renderer, TTF_Font *police, int x, int y, int size, char *content, char *text_police, int r, int g, int b);
int displaySprite(SDL_Renderer *renderer, char *sprite, int x, int y);
int closeWindow(SDL_Window *pWindow);
int createWindow(int x, int y, char *title);

#endif