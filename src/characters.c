#include <stdio.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "graphics.h"
#include "substruct.h"
#include "common.h"

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

void setEntityToTile(Tile * grid, Entity * entity, Coord tile, int xSize, int ySize)
// Set an entity to a specific tile
{    
    if (tile.x < 0 || tile.x >= xSize || tile.y < 0 || tile.y >= ySize)
    {
        printf("\033[31;01mERRoR\033[00m : Coordinates out of boundaries\n");
        exit(EXIT_FAILURE);
    };

    if (entity == NULL || grid == NULL)
    {
        printf("\033[31;01mERROR\033[00m : Pointer null on tile or entity\n");
        exit(EXIT_FAILURE);
    }

    grid[tile.x*xSize+tile.y].entity = entity;
}

void moveEntity(Tile * grid, Coord from, Coord to, int xSize, int ySize)
// Move an entity already set on a tile to a new one
{
    if (from.x < 0 || from.x >= xSize || from.y < 0 || from.y >= ySize || to.x < 0 || to.x >= xSize || to.y < 0 || to.y >= ySize)
    {
        printf("\033[31;01mERROR\033[00m : Coordinates out of boundaries\n");
        exit(EXIT_FAILURE);
    };

    if (grid[from.x*xSize+from.y].entity == NULL || grid == NULL)
    {
        printf("\033[31;01mERROR\033[00m : Pointer null on tile or entity\n");
        exit(EXIT_FAILURE);
    }

    grid[to.x*xSize+to.y].entity = grid[from.x*xSize+from.y].entity;
    grid[from.x*xSize+from.y].entity = NULL;
}

void createCharacters(Tile * grid, Coord pos, int xSize, int ySize, int pdv)
// Create a new character at (x,y) pos
{
    Entity * entity = malloc(sizeof(Entity));
    entity->stat_mods[0] = pdv;
    grid[pos.x*xSize+pos.y].entity = entity;
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
