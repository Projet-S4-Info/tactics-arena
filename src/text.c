
/* =============== DEPENDANCES =============== */


#include <stdio.h>
#include <string.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"
#include "struct.h"
#include "map.h"
#include "menu.h"
#include "characters.h"
#include "common.h"
#include "map_editor.h"
#include "grid.h"
#include "init.h"
#include "gameplay.h"
#include "game_window.h"


/* =============== CONSTANTES ================ */


#define _NB_TEXT_CACHE_ 256     // Max number of cached texts
#define _CACHE_AGE_ 5           // Max age of a cached text before being removed


/* =============== VARIABLES ================ */


TextCache text_cache[_NB_TEXT_CACHE_];
int text_cache_size = 0;


/* =============== FONCTIONS ================ */


bool isInCache(char *content, int size, char *police, SDL_Color color)
// Returns true if the given text is in cache
{
    bool result = TRUE;

    for (int i = 0; i < text_cache_size; i++)
    {
        result = TRUE;
        // Check content
        result = result && (strcmp(content, text_cache[i].content) == 0);
        // Check police
        result = result && (strcmp(police, text_cache[i].police) == 0);
        // Check size
        result = result && (size == text_cache[i].size);
        // Check R color
        result = result && (color.r == text_cache[i].color.r);
        // Check G color
        result = result && (color.g == text_cache[i].color.g);
        // Check B color
        result = result && (color.b == text_cache[i].color.b);

        if (result == TRUE) break;
    }

    if (text_cache_size == 0) result = FALSE;

    return result;
}


err_t addToCache(char *content, int size, char *police, SDL_Color color, SDL_Surface *surface)
// Add the given texture (and its properties) to cache
{
    if (isInCache(content, size, police, color)) return ALREADY_IN_CACHE;

    if (verbose) printf("[TEXT CACHE] Ajout du texte '%s' au cache à l'ID %d ...\n", content, text_cache_size);

    text_cache[text_cache_size].content = content;
    text_cache[text_cache_size].police = police;
    text_cache[text_cache_size].size = size;
    text_cache[text_cache_size].color = color;
    text_cache[text_cache_size].surface = surface;
    text_cache[text_cache_size].time = SDL_GetTicks();

    text_cache_size++;

    if (verbose) printf("[TEXT CACHE] Taille du cache : %d\n", text_cache_size);

    return OK;
}


err_t clearOldCache()
// Clear old cache entries
{
    for (int i = 0; i < text_cache_size; i++)
    {
        
    }
}


SDL_Surface * getTextCache(char *content, int size, char *police, SDL_Color color)
// Returns the texture of a text if it exists in cache
{
    if (!isInCache(content, size, police, color)) exit(EXIT_FAILURE);

    SDL_Surface * surface = NULL;
    bool result = TRUE;

    for (int i = 0; i < text_cache_size; i++)
    {
        result = TRUE;
        // Check content
        result = result && (strcmp(content, text_cache[i].content) == 0);
        // Check police
        result = result && (strcmp(police, text_cache[i].police) == 0);
        // Check size
        result = result && (size == text_cache[i].size);
        // Check R color
        result = result && (color.r == text_cache[i].color.r);
        // Check G color
        result = result && (color.g == text_cache[i].color.g);
        // Check B color
        result = result && (color.b == text_cache[i].color.b);

        if (result == TRUE)
        {
            surface = text_cache[i].surface;
            text_cache[i].time = SDL_GetTicks();
            break;
        }
    }

    return surface;
}



err_t displayText(SDL_Renderer *renderer, int x, int y, int size, char *content, char *text_police, int r, int g, int b)
// Displays text on the window
{
    SDL_Color color = {r, g, b};
    SDL_Rect txtDestRect;

    if (isInCache(content, size, text_police, color))
    {

        // Création de la texture à partir de la surface
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, getTextCache(content, size, text_police, color));
        if(!text_tex){
            fprintf(stderr, "Erreur à la création du rendu du texte : %s\n", SDL_GetError());
            return SDL_ERROR;
        }

        txtDestRect.x = x;
        txtDestRect.y = y;

        SDL_QueryTexture(text_tex, NULL, NULL, &(txtDestRect.w), &(txtDestRect.h));

        /* Ajout du texte en noir */
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderCopy(renderer, text_tex, NULL, &txtDestRect);
    }
    else
    {
        if (verbose) printf("[TEXT] Texte non disponible dans le cache...\n");

        SDL_Surface *text = NULL;
        TTF_Font *police = NULL;

        // Chargement de la police
        if( (police = TTF_OpenFont(text_police, size)) == NULL){
            fprintf(stderr, "Erreur chargement initial font : %s\n", TTF_GetError());
            return SDL_ERROR;
        }

        // Création de la surface à partir du texte
        text = TTF_RenderUTF8_Blended(police, content, color);
        if(!text){
            fprintf(stderr, "Erreur à la création du texte : %s\n", SDL_GetError());
            return SDL_ERROR;
        }

        // Création de la texture à partir de la surface
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text);
        if(!text_tex){
            fprintf(stderr, "Erreur à la création du rendu du texte : %s\n", SDL_GetError());
            return SDL_ERROR;
        }

        addToCache(content, size, text_police, color, text);

        SDL_QueryTexture(text_tex, NULL, NULL, &(txtDestRect.w), &(txtDestRect.h));

        txtDestRect.x = x;
        txtDestRect.y = y;

        /* Ajout du texte en noir */
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderCopy(renderer, text_tex, NULL, &txtDestRect);

        //SDL_FreeSurface(text);
        TTF_CloseFont(police);
        SDL_DestroyTexture(text_tex);
    }

	return OK;
}