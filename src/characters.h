#ifndef characters_h
#define characters_h

#include "struct.h"

int loadSprites(SDL_Renderer * renderer, TabTexture * cSprites);
int setEntityToTile(Tile * grid, Entity * entity, Coord tile, int xSize, int ySize);
void moveEntity(Tile * grid, Coord from, Coord to, int xSize, int ySize);
void createCharacters(Tile * grid, Coord pos, int xSize, int ySize, int pdv);
int displayCharacters(SDL_Renderer * renderer, TabTexture * cSprites, Entity * entity, int x, int y, int pxBase);

#endif