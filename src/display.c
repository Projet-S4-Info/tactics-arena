
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


/* =============== CONSTANTES ================ */


#define _NB_MAX_MAPS_ 50
#define _X_SIZE_ 30                         // | Size of the grid
#define _Y_SIZE_ 30                         // |
#define _FPS_ 60							// Define at which frequency the game has to refresh


/* =============== VARIABLES ================ */


unsigned long long frames = 0;				// Frames
const int camSpeed = 10;					// Speed of the camera movements

int xWinSize, yWinSize;						// x and y sizes of the window
Coord mouse_position;


/* =============== FONCTIONS ================ */


void setRendererDriver(SDL_Renderer *renderer)
// Set the default renderer driver to OpenGL for MacOS compatibility
{
	SDL_RendererInfo *global_renderer_info = malloc(sizeof(SDL_RendererInfo));

	if (SDL_GetRendererInfo(renderer, global_renderer_info) != 0)
    {
        printf("[GRAPHICS] Erreur lors de l'obtention des informations du renderer : %s\n", SDL_GetError());
		return;
    } else {
		if(verbose)printf("Driver utilisé par le renderer : %s\n", global_renderer_info->name);
		if(verbose)printf("Résolution maximale des textures : %dpx / %dpx\n", global_renderer_info->max_texture_width, global_renderer_info->max_texture_height);
	}

	if (!strcmp(global_renderer_info->name, "opengl")){
		if (SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl"))
		{
			if(verbose)printf("[GRAPHICS] Le driver utilisé est maintenant OpenGL\n");
		} else {
			printf("[GRAPHICS] Erreur lors du changement de driver : %s\n", SDL_GetError());
			return;
		}
	}
	
	free(global_renderer_info);
}



void displayText(SDL_Renderer *renderer, int x, int y, int size, const char *content, const char *text_police, int r, int g, int b)
// Displays text on the window
{
	SDL_Surface *text = NULL;
	TTF_Font *police = NULL;
	SDL_Rect txtDestRect;

	SDL_Color color = {r, g, b};

	// Chargement de la police
	if( (police = TTF_OpenFont(text_police, size)) == NULL){
		fprintf(stderr, "Erreur chargement initial font : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	// Création de la surface à partir du texte
	text = TTF_RenderUTF8_Blended(police, content, color);
	if(!text){
		fprintf(stderr, "Erreur à la création du texte : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	// Création de la texture à partir de la surface
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text);
	if(!text_tex){
		fprintf(stderr, "Erreur à la création du rendu du texte : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_FreeSurface(text);
	SDL_QueryTexture(text_tex, NULL, NULL, &(txtDestRect.w), &(txtDestRect.h));

	txtDestRect.x = x;
	txtDestRect.y = y;

	/* Ajout du texte en noir */
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_RenderCopy(renderer, text_tex, NULL, &txtDestRect);

	//SDL_RenderPresent(renderer);

	TTF_CloseFont(police);
}



int displaySprite(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y)
// Display a sprite on the window
{
	SDL_Rect imgDestRect;
	
	imgDestRect.x = x;
	imgDestRect.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &(imgDestRect.w), &(imgDestRect.h));
	SDL_RenderCopy(renderer, texture, NULL, &imgDestRect);
	frames++;

	return 1;
}



int closeWindow(SDL_Window *pWindow)
// Kill and close the window
{
	SDL_DestroyWindow(pWindow);
	TTF_Quit();
	Mix_Quit();
  	SDL_Quit();

    return 0;
}