#ifndef textures_h
#define textures_h

/* =============== DEPENDENCES =============== */


#include "struct.h"


/* =============== CONSTANTES =============== */


#define _NB_MAX_TEXTURES_ 50
#define _NB_CLASSES_ 6


/* =============== VARIABLES =============== */


extern int pxBase;


/* =============== STRUCTURES =============== */


typedef enum {BLANK, BLOCK, BLUE, RED, WATER, SAND, ICE, SNOW} block_id;


/* =============== DICTIONNAIRES DE TEXTURES =============== */


extern TabTexture cSprites[_NB_MAX_TEXTURES_];
extern TabTexture textures[_NB_MAX_TEXTURES_];
extern CharTexture charTextures[_NB_CLASSES_];
extern AnimTexture animTextures[NB_AB];


/* =============== FONCTIONS =============== */

// Optimise les surfaces lors du chargement des images
SDL_Surface *optimize(SDL_Surface *surf);

// Détruit l'ensemble d'un dictionnaire de textures
void freeTextures(TabTexture * textures);

// Ajoute deux textures (64x64 et 128x128) au tableau de dictionnaire en paramètre et lui attribue un nom
int addTextureToTable(TabTexture * texturesTable, SDL_Texture * texture, SDL_Texture * big_texture, char * texture_name);

// Recupère la texture (64x64) en fonction de son nom
SDL_Texture * getTexture(TabTexture * textures, const char * texture_name);

// Récupère la texture (128x128) en fonction de son nom
SDL_Texture * getBigTexture(TabTexture * textures, const char * texture_name);

// Crée une surface à partir d'une image
SDL_Surface * loadImage(char * img);

// Crée une texture à partir d'une surface
SDL_Texture * loadTexture(SDL_Renderer * renderer, SDL_Surface * surface);

#endif