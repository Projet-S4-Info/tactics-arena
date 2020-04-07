#ifndef characters_h
#define characters_h

#include "struct.h"

extern CharTexture charTextures[];

int addCharacterTexture(SDL_Renderer *renderer, char * name);
SDL_Texture * getCharTexture(char *name, Direction direction, int indexAnim);
SDL_Texture * getCharFrontTexture(char *name);
int loadSprites(SDL_Renderer * renderer, TabTexture * cSprites);
int setEntityToTile(Entity * entity, Coord tile);
void moveEntity(Coord from, Coord to);
void createCharacters(Coord pos, int pdv);
int displayCharacters(SDL_Renderer * renderer, TabTexture * cSprites, Entity * entity, int x, int y, int pxBase);
void switchEntities(Coord pos1, Coord pos2);

#endif