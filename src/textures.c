/* =============== DEPENDENCES =============== */


#include <stdio.h>
#include <string.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "struct.h"


/* =============== CONSTANTES =============== */


#define _NB_MAX_TEXTURES_ 50
#define _NB_CLASSES_ 6
#define _X_SIZE_ 30
#define _Y_SIZE_ 30


/* =============== DICTIONNAIRES DE TEXTURES =============== */


TabTexture cSprites[_NB_MAX_TEXTURES_];
TabTexture textures[_NB_MAX_TEXTURES_];
CharTexture charTextures[_NB_CLASSES_];


/* =============== VARIABLES =============== */


int pxBase = 64;							// Resolution of a bloc texture (can be 64 or 128)


/* =============== FONCTIONS =============== */


SDL_Surface *optimize(SDL_Surface *surf)
// Optimizes the surface for the display format
{
	SDL_Surface *opt = SDL_ConvertSurfaceFormat(surf, surf->format->format, 0);

	if (opt)
	{
		SDL_FreeSurface(surf);
		return opt;
	}
	
	return surf;
}



void freeTextures(TabTexture * textures)
// Free all the textures in the given textures table
{
	int nbTextures = sizeof(textures) / sizeof(SDL_Texture*);

	for (int i=0; i < nbTextures; i++)
	{
		if (textures[i].texture != NULL)
		{
			SDL_DestroyTexture(textures[i].texture);
			textures[i].texture = NULL;
		}

		if (textures[i].big_texture != NULL)
		{
			SDL_DestroyTexture(textures[i].big_texture);
			textures[i].big_texture = NULL;
		}
	}
}



int addTextureToTable(TabTexture * texturesTable, SDL_Texture * texture, SDL_Texture * big_texture, char * texture_name)
// Add to the TabTexture table the given texture and its big one
{
	int index = 0;

	while (texturesTable[index].texture != NULL)
	{
		index++;
	}

	texturesTable[index].texture = texture;
	texturesTable[index].big_texture = big_texture;
	texturesTable[index].texture_name = texture_name;

	if (verbose == 2) printf("\033[36;01m[TEXTURES]\033[00m : Ajout de la texture [%s] à l'id %d\n", texture_name, index);

	return index;
}



SDL_Texture * getTexture(TabTexture * textures, const char * texture_name)
// Return the texture associated with its name
{
	int index = 0;

	while (textures[index].texture != NULL)
	{
		index++;
	}

	for (int i=0; i < index; i++)
	{
		if (strcmp(textures[i].texture_name, texture_name) == 0)
		{
			return textures[i].texture;
		}
	}

	exit(EXIT_FAILURE);
}



SDL_Texture * getBigTexture(TabTexture * textures, const char * texture_name)
// Return the texture associated with its name
{
	int index = 0;

	while (textures[index].texture != NULL)
	{
		index++;
	}

	for (int i=0; i < index; i++)
	{
		if (strcmp(textures[i].texture_name, texture_name) == 0)
		{
			return textures[i].big_texture;
		}
	}

	exit(EXIT_FAILURE);
}



SDL_Surface * loadImage(const char * img)
// Load a PNG image into a surface
{
	SDL_RWops *rwop = NULL;
	SDL_Surface *surface = NULL;

	rwop=SDL_RWFromFile(img, "rb");
	surface=IMG_LoadPNG_RW(rwop);
	if(!surface) {
		printf("\033[31;01m[TEXTURES ERROR]\033[00m : Erreur lors du chargement de l'image %s : %s\n", img, IMG_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_FreeRW(rwop);
	rwop = NULL;

	return optimize(surface);
}



SDL_Texture * loadTexture(SDL_Renderer * renderer, SDL_Surface * surface)
// Create a texture from a surface
{
	SDL_Texture * texture = NULL;

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(!texture){
		printf("\033[31;01m[TEXTURES ERROR]\033[00m : Erreur lors de la creation de la texture : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_FreeSurface(surface);
	surface = NULL;

	return texture;
}