
/* =============== DEPENDENCES =============== */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "game_window.h"
#include "substruct.h"
#include "common.h"
#include "textures.h"
#include "display.h"
#include "grid.h"
#include "text.h"


/* =============== CONSTANTES =============== */


#define _X_SIZE_ 30
#define _Y_SIZE_ 30
#define _NB_CLASSES_ 6
#define _NB_ANIM_ 6


/* =============== VARIABLES =============== */


int indexCharTable = 0;


/* =============== FONCTIONS =============== */


int addCharacterTexture(SDL_Renderer *renderer, char * name)
// Add to the CharTexture table all the animation of a class and its given direction
{
    int dirIndex = N;
    int animIndex = 1;
    char path[STR_LONG];
    char temp[STR_LONG];
    char *dirFolder;

    sprintf(path, "../inc/sprites/%s/sprite_indiv/", name);
    if(verbose) printf("Chargement des textures du dossier : %s\n", path);

	charTextures[indexCharTable].texture_name = name;
    if(verbose) printf("Chargement de la texture : %s\n", strcat(path, "front/Sprite_frontview_64.png"));
    sprintf(path, "../inc/sprites/%s/sprite_indiv/", name);
    charTextures[indexCharTable].front = loadTexture(renderer, loadImage(strcat(path, "front/Sprite_frontview_64.png")));
    sprintf(path, "../inc/sprites/%s/sprite_indiv/", name);

    for (dirIndex = N; dirIndex <= W; dirIndex++)
    {
        if (dirIndex == N) dirFolder = "haut_gauche";
        else if (dirIndex == E) dirFolder = "haut_droit";
        else if (dirIndex == S) dirFolder = "bas_droit";
        else if (dirIndex == W) dirFolder = "bas_gauche";

        for (animIndex = 1; animIndex <= _NB_ANIM_; animIndex++)
        {
            sprintf(temp, "%s%s/Sprite_%s%d.png", path, dirFolder, dirFolder, animIndex);
            if(verbose) printf("Chargement de la texture : %s\n", temp);
            charTextures[indexCharTable].textures[dirIndex][animIndex-1] = loadTexture(renderer, loadImage(temp));
        }
    }

    indexCharTable++;

	if(verbose)printf("[GRAPHICS] Ajout de la texture de la classe [%s] à l'id %d\n", name, indexCharTable);

	return indexCharTable;
}



SDL_Texture * getCharTexture(char *name, Direction direction, int indexAnim)
// Returns the texture of a given class in a given direction at a given animation index
{
    SDL_Texture * result;

    name[0] = tolower(name[0]);
    for (int i=0; i < _NB_CLASSES_; i++)
    {
        if (strcmp(name, charTextures[i].texture_name) == 0)
        {
            result = charTextures[i].textures[direction][indexAnim];
            break;
        }
    }

    return result;
}



SDL_Texture * getCharFrontTexture(char *name)
// Returns the front texture of a given class
{
    name[0] = tolower(name[0]);
    SDL_Texture * result = NULL;

    for (int i=0; i < _NB_CLASSES_; i++)
    {
        if (strcmp(name, charTextures[i].texture_name) == 0)
        {
            result = charTextures[i].front;
            break;
        }
    }

    return result;
}



int loadSprites(SDL_Renderer * renderer, TabTexture * cSprites)
// Load all the textures needed for the characters
{
    int nbSprites = 0;
    int nbChar = 0;

    freeTextures(cSprites);

    if(verbose)printf("Chargement des textures des personnages...\n");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadImage("../inc/img/icons/heart_16.png")),
                        loadTexture(renderer, loadImage("../inc/img/icons/heart_32.png")),
                        "heart_icon");

    addTextureToTable(  cSprites,
                        loadTexture(renderer, loadImage("../inc/img/icons/mv_icon_16.png")),
                        loadTexture(renderer, loadImage("../inc/img/icons/mv_icon_32.png")),
                        "mv_icon");

    nbSprites = addTextureToTable(  cSprites,
                        loadTexture(renderer, loadImage("../inc/img/icons/star_icon_16.png")),
                        loadTexture(renderer, loadImage("../inc/img/icons/star_icon_32.png")),
                        "star_icon");

    addCharacterTexture(renderer, "angel");
    addCharacterTexture(renderer, "berserker");
    addCharacterTexture(renderer, "goliath");
    addCharacterTexture(renderer, "mage");
    addCharacterTexture(renderer, "ranger");
    nbChar = addCharacterTexture(renderer, "valkyrie");

    if(verbose)printf("[CHARACTERS] %d texture(s) d'icone(s) chargée(s) !\n", nbSprites+1);
    if(verbose)printf("[CHARACTERS] %d texture(s) de personnage(s) chargée(s) !\n", nbChar+1);
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

    (*(matrix+to.x*_X_SIZE_+to.y)).entity = malloc(sizeof(Entity*));
    (*(matrix+to.x*_X_SIZE_+to.y)).entity = (*(matrix+from.x*_X_SIZE_+from.y)).entity;
    (*(matrix+from.x*_X_SIZE_+from.y)).entity = NULL;
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
    if (pxBase == 64)   displaySprite(renderer, getCharTexture(entity->cha_class->cla_name, entity->direction, 0), x, y);
    //else                displaySprite(renderer, getBigTexture(cSprites, "base_model"), x, y);

    // Display character's life points
    char temp[STR_SHORT];
    sprintf(temp, "%d", entity->stat_mods[pv]);
    if (pxBase == 64)   displaySprite(renderer, getTexture(cSprites, "heart_icon"), x+(pxBase/4), y-(pxBase/4));
    else                displaySprite(renderer, getBigTexture(cSprites, "heart_icon"), x+(pxBase/4), y-(pxBase/4));
    displayText(renderer, x+(pxBase/2), y-(pxBase/4), (pxBase/64)*15, temp, "../inc/font/Pixels.ttf", 255, 0, 0, TRUE);

    return 0;
}
