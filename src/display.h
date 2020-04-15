#ifndef display_h
#define display_h

/* =============== DEPENDANCES =============== */


#include "struct.h"


/* =============== VARIABLES ================ */


extern int xWinSize, yWinSize;              // x and y sizes of the window
extern const int camSpeed;
extern Coord mouse_position;
extern Log logs[];


/* =============== FONCTIONS ================ */


err_t setRendererDriver(SDL_Renderer *renderer);

err_t displayText(SDL_Renderer *renderer, int x, int y, int size, const char *content, const char *text_police, int r, int g, int b);

err_t displaySprite(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y);

err_t closeWindow(SDL_Window *pWindow);

err_t addLog(char * message);

err_t displayLog(SDL_Renderer * renderer, Coord pos);

err_t removeOldLogs(Uint32 time);

err_t addMsgToChatTab(char * msg, int chatTabIndex);

#endif