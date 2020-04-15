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
extern bool hover_next_turn;
extern int hover_tchat;
extern int hover_passive_help;
extern Coord mouse_position;
extern char description[100];
extern int isChatActive;
<<<<<<< HEAD
extern int chatTabIndex;

extern char chat[STR_LONG];
=======
extern Direction camMove;
>>>>>>> 5e2e708aaf78ac975f52d7831233997ab2bd4503


/* =============== FONCTIONS =============== */


int createGameWindow(int x, int y);
int displayMenu(int x, int y);

#endif
