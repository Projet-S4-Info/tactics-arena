#ifndef map_h
#define map_h


#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"
#include "struct.h"


extern Coord borderTab[];
extern Coord rangeTab[];
extern Entity * selectedEntity;


int loadMapTextures(SDL_Renderer * renderer);
float crossProduct(Vector AB, Vector AC);
int displayAbilities(SDL_Renderer * renderer);
int displayInterface(SDL_Renderer * renderer);
int displayMap(SDL_Renderer *renderer, int x, int y);
int selectTile(int xpos, int ypos, int mx, int my);

#endif
