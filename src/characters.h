#ifndef characters_h
#define characters_h

#include "struct.h"

int loadSprites(SDL_Renderer * renderer, TabTexture * cSprites);
int setEntityToTile(Entity * entity, Coord tile);
void moveEntity(Coord from, Coord to);
void createCharacters(Coord pos, int pdv);
int displayCharacters(SDL_Renderer * renderer, TabTexture * cSprites, Entity * entity, int x, int y, int pxBase);
void switchEntities(Coord pos1, Coord pos2);

#endif