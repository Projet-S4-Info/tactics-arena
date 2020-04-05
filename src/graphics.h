#ifndef graphics_h
#define graphics_h

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"
#include "struct.h"

extern int xWinSize, yWinSize;
extern int selected_ability;
extern int hover_ability;
extern char description[100];

void setRendererDriver(SDL_Renderer *renderer);
void freeTextures(TabTexture * textures);
int addTextureToTable(TabTexture * texturesTable, SDL_Texture * texture, SDL_Texture * big_texture, const char * texture_name);
SDL_Texture * getTexture(TabTexture textures[], const char * texture_name);
SDL_Texture * getBigTexture(TabTexture * textures, const char * texture_name);
SDL_Surface * loadImage(const char * img);
SDL_Texture * loadTexture(SDL_Renderer * renderer, SDL_Surface * surface);
void displayText(SDL_Renderer *renderer, int x, int y, int size, const char *content, const char *text_police, int r, int g, int b);
int displaySprite(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y);
int closeWindow(SDL_Window *pWindow);
int createGameWindow(int x, int y);
int displayMenu(int x, int y);

#endif
