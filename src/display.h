#ifndef display_h
#define display_h

/* =============== DEPENDANCES =============== */


#include "struct.h"


/* =============== VARIABLES ================ */


extern int xWinSize, yWinSize;              // x and y sizes of the window
Coord mouse_position;

unsigned long long frames;				// Frames
const int camSpeed;					// Speed of the camera movements


/* =============== FONCTIONS ================ */


void setRendererDriver(SDL_Renderer *renderer);

void displayText(SDL_Renderer *renderer, int x, int y, int size, const char *content, const char *text_police, int r, int g, int b);

int displaySprite(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y);

int closeWindow(SDL_Window *pWindow);

#endif