#ifndef game_window_h
#define game_window_h


/* =============== DEPENDENCES =============== */


#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"
#include "struct.h"


/* =============== VARIABLES =============== */


extern int selected_ability;
extern int hover_ability;
extern int hover_passive_help;
extern Coord mouse_position;
extern char description[100];


/* =============== FONCTIONS =============== */


int createGameWindow(int x, int y);
int displayMenu(int x, int y);

#endif
