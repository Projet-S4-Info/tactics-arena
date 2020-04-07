#include <stdio.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "graphics.h"
#include "substruct.h"
#include "common.h"

#define _X_SIZE_ 30
#define _Y_SIZE_ 30

#define _NB_CLASSES_ 6

CharTexture charTextures[_NB_CLASSES_];

int addCharacterTexture(SDL_Texture * textureN, SDL_Texture * textureE, SDL_Texture * textureS, SDL_Texture * textureW, 
                        SDL_Texture * bigTextureN, SDL_Texture * bigTextureE, SDL_Texture * bigTextureS, SDL_Texture * bigTextureW,
                        char * texture_name)
// Add to the TabTexture table the given texture and its big one
{
	int index = 0;

	while (charTextures[index].textures[0] != NULL)
	{
		index++;
	}

	charTextures[index].textures[N] = textureN;
	charTextures[index].textures[E] = textureE;
    charTextures[index].textures[S] = textureS;
    charTextures[index].textures[W] = textureW;
    charTextures[index].textures[4+N] = bigTextureN;
	charTextures[index].textures[4+E] = bigTextureE;
    charTextures[index].textures[4+S] = bigTextureS;
    charTextures[index].textures[4+W] = bigTextureW;
	charTextures[index].texture_name = texture_name;

	if(verbose)printf("[GRAPHICS] Ajout de la texture de la classe [%s] à l'id %d\n", texture_name, index);

	return index;
}

int loadSprites(SDL_Renderer * renderer, TabTexture * cSprites)
// Load all the textures needed for the characters
{
    int nbSprites = 0;

    freeTextures(cSprites);

    if(verbose)printf("Chargement des textures des personnages...\n");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadImage("../inc/img/heart_16.png")),
                        loadTexture(renderer, loadImage("../inc/img/heart_32.png")),
                        "heart_icon");

    nbSprites = addTextureToTable(  cSprites,
                                    loadTexture(renderer, loadImage("../inc/sprites/base/sprite_sheet/64_64/Sprite_frontview_64.png")),
                                    loadTexture(renderer, loadImage("../inc/sprites/base/sprite_sheet/128_128/Sprite_frontview_128.png")),
                                    "base_model");

    if(verbose)printf("[CHARACTERS] %d texture(s) de personnage(s) et icone(s) chargée(s) !\n", nbSprites+1);
    return nbSprites;
}

void setEntityToTile(Entity * entity, Coord tile)
// Set an entity to a specific tile
{    
    if (tile.x < 0 || tile.x >= _X_SIZE_ || tile.y < 0 || tile.y >= _Y_SIZE_)
    {
        printf("\033[31;01mERRoR\033[00m : Coordinates out of boundaries\n");
        exit(EXIT_FAILURE);
    };

    if (entity == NULL || matrix == NULL)
    {
        printf("\033[31;01mERROR\033[00m : Pointer null on tile or entity\n");
        exit(EXIT_FAILURE);
    }

    matrix[tile.x*_X_SIZE_+tile.y].entity = entity;
}

void moveEntity(Coord from, Coord to)
// Move an entity already set on a tile to a new one
{
    if (from.x < 0 || from.x >= _X_SIZE_ || from.y < 0 || from.y >= _Y_SIZE_ || to.x < 0 || to.x >= _X_SIZE_ || to.y < 0 || to.y >= _Y_SIZE_)
    {
        printf("\033[31;01mERROR\033[00m : Coordinates out of boundaries\n");
        exit(EXIT_FAILURE);
    };

    if (matrix[from.x*_X_SIZE_+from.y].entity == NULL || matrix == NULL)
    {
        printf("\033[31;01mERROR\033[00m : Pointer null on tile or entity\n");
        exit(EXIT_FAILURE);
    }

    matrix[to.x*_X_SIZE_+to.y].entity = matrix[from.x*_X_SIZE_+from.y].entity;
    matrix[from.x*_X_SIZE_+from.y].entity = NULL;
}

void switchEntities(Coord pos1, Coord pos2)
// Switch the positions of the 2 entities
{
    Entity * temp = (*(matrix+pos1.x*_X_SIZE_+pos1.y)).entity;
    (*(matrix+pos1.x*_X_SIZE_+pos1.y)).entity = (*(matrix+pos2.x*_X_SIZE_+pos2.y)).entity;
    (*(matrix+pos2.x*_X_SIZE_+pos2.y)).entity = temp;
}

void createCharacters(Coord pos, int pdv)
// Create a new character at (x,y) pos
{
    Entity * entity = malloc(sizeof(Entity));
    entity->stat_mods[0] = pdv;
    matrix[pos.x*_X_SIZE_+pos.y].entity = entity;
}

int displayCharacters(SDL_Renderer * renderer, TabTexture * cSprites, Entity * entity, int x, int y, int pxBase)
// Display the characters on the map
{
    // Display the character
    if (pxBase == 64)   displaySprite(renderer, getTexture(cSprites, "base_model"), x, y);
    else                displaySprite(renderer, getBigTexture(cSprites, "base_model"), x, y);

    // Display character's life points
    char temp[20];
    sprintf(temp, "%d", entity->stat_mods[pv]);
    if (pxBase == 64)   displaySprite(renderer, getTexture(cSprites, "heart_icon"), x+(pxBase/4), y-(pxBase/4));
    else                displaySprite(renderer, getBigTexture(cSprites, "heart_icon"), x+(pxBase/4), y-(pxBase/4));
    displayText(renderer, x+(pxBase/2), y-(pxBase/4), (pxBase/64)*15, temp, "../inc/font/Pixels.ttf", 255, 0, 0);
    
    return 0;
}
