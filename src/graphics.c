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

#define _NB_MAX_MAPS_ 50
#define _X_SIZE_ 30
#define _Y_SIZE_ 30

Tile grid[_X_SIZE_][_Y_SIZE_];			/**< Contains the pointer to the start of the matrix */
Tile *matrix = &grid[0][0];

// x and y sizes of the window
int xWinSize, yWinSize;

// Selected ability
int selected_ability = 0;
// Hover ability
int hover_ability = -1;
// Fin de tour
int end_of_turn = 0;

// Ability description
char description[100];

TabTexture cSprites[50];

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

	if(verbose)printf("[GRAPHICS] Ajout de la texture [%s] à l'id %d\n", texture_name, index);

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
		printf("[GRAPHICS] loadImage error while loading %s : %s\n", img, IMG_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_FreeRW(rwop);

	return surface;
}

SDL_Texture * loadTexture(SDL_Renderer * renderer, SDL_Surface * surface)
// Create a texture from a surface
{
	SDL_Texture * texture = NULL;

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(!texture){
		fprintf(stderr, "[GRAPHICS] loadTexture error while creating texture : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	return texture;
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

int createGameWindow(int x, int y)
// Create a window with with x*y size (in px)
{
    // Le pointeur vers la fenetre
	SDL_Window* pWindow = NULL;
	SDL_Renderer *renderer=NULL;

	// Resolution of a bloc texture (can be 64 or 128)
	int PX = 64;

	// x and y pos where map is displayed
	int XPOS = 50;
	int YPOS = 50;

    /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0 ) {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

	/* Initialisation TTF */
	if(TTF_Init() == -1) {
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	/* Création de la fenêtre */
	pWindow = SDL_CreateWindow("Tactics Arena", 	SDL_WINDOWPOS_UNDEFINED,
												  	SDL_WINDOWPOS_UNDEFINED,
												  	x,
												  	y,
												  	SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if(!pWindow){
		fprintf(stderr, "Erreur à la création de la fenetre : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		fprintf(stderr, "Erreur à la création du renderer\n");
		exit(EXIT_FAILURE);
	}

	setRendererDriver(renderer);

	// Launcher icon
    SDL_SetWindowIcon(pWindow, loadImage("../inc/img/TacticsArena.png"));

	if( pWindow )
	{
		SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);

		// Chargement des textures
		loadMapTextures(renderer);
		loadSprites(renderer, cSprites);
		int start_seconds = SDL_GetTicks()/1000;
		int load_index = 0;
		while((SDL_GetTicks()/1000)-start_seconds < 3)
		{
			load_index++;
			SDL_SetRenderDrawColor(renderer, 21, 126, 172, 255);
			SDL_RenderClear(renderer);
			displayText(renderer, 200, yWinSize/2+120, 40, "Chargement des textures du jeu...", "../inc/font/Pixels.ttf", 255, 255, 255);
			displayText(renderer, 200, yWinSize/2, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255);
			if (load_index == 1)		displayText(renderer, xWinSize/2, yWinSize/3*2, 60, "Ooo", "../inc/font/Aqua.ttf", 255, 255, 255);
			else if (load_index == 2)	displayText(renderer, xWinSize/2, yWinSize/3*2, 60, "oOo", "../inc/font/Aqua.ttf", 255, 255, 255);
			else if (load_index == 3)	displayText(renderer, xWinSize/2, yWinSize/3*2, 60, "ooO", "../inc/font/Aqua.ttf", 255, 255, 255);
			SDL_Delay(100);
			SDL_RenderPresent(renderer);
			SDL_Delay(900);
		}

		/* Le fond de la fenêtre sera blanc */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		loadMap(matrix, "map_plains");

		SDL_Delay(1);

		displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);

		SDL_RenderPresent(renderer);

		Entity * tempEntity = NULL;

		int running = 1;
		while(running) {
			SDL_Event e;
			while(SDL_PollEvent(&e)) {
				switch(e.type) {
					case SDL_QUIT: running = 0;
					break;
					case SDL_WINDOWEVENT:
						switch(e.window.event){
							case SDL_WINDOWEVENT_EXPOSED:
							case SDL_WINDOWEVENT_SIZE_CHANGED:
							case SDL_WINDOWEVENT_RESIZED:
							case SDL_WINDOWEVENT_HIDDEN:
							case SDL_WINDOWEVENT_SHOWN:

								SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);

								/* Le fond de la fenêtre sera blanc */
                				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
								SDL_RenderClear(renderer);

								SDL_Delay(1);

								displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);

								SDL_RenderPresent(renderer);

							break;
						}
					break;
					case SDL_MOUSEBUTTONDOWN:

						if(verbose)printf("X: %d | Y: %d\n", e.motion.x, e.motion.y);		// Debug console pos x & y on term
						//if (e.motion.x <= 10*64+XPOS && e.motion.y <= 10*64+YPOS && e.motion.x >= XPOS && e.motion.y >= YPOS){
							selectTile(matrix, XPOS, YPOS, e.motion.x, e.motion.y, PX, _X_SIZE_, _Y_SIZE_);
							displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
						//}

						// Compétences et actions
						tempEntity = getEntity(matrix, getSelectedPos());
						if (tempEntity != NULL)
						{
							if (e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-16)
							{
								if (e.motion.x >= 16 && e.motion.x <= 80)	selected_ability = 0;
								if (e.motion.x >= 96 && e.motion.x <= 160)	selected_ability = tempEntity->cha_class->cla_abilities[0].ab_id;
								if (e.motion.x >= 176 && e.motion.x <= 240)	selected_ability = tempEntity->cha_class->cla_abilities[1].ab_id;
								if (e.motion.x >= 256 && e.motion.x <= 320)	selected_ability = tempEntity->cha_class->cla_abilities[2].ab_id;
								if (e.motion.x >= 336 && e.motion.x <= 400)	selected_ability = tempEntity->cha_class->cla_abilities[3].ab_id;
								if(verbose)printf("Selected ability : %d\n", selected_ability);
								displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
							}
						}


					break;
					case SDL_MOUSEWHEEL:
						if (e.wheel.y > 0)		// Scroll UP
						{
							if (PX == 64){
								PX = 128;
								if(verbose)printf("[GRAPHICS] Zoom In\n");
								XPOS *= 2;
								YPOS *= 2;
								displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
							}
						} else {				// Scroll DOWN
							if (PX == 128){
								PX = 64;
								if(verbose)printf("[GRAPHICS] Zoom Out\n");
								XPOS /= 2;
								YPOS /= 2;
								displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
							}
						}
					break;
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym)
						{
							case SDLK_KP_PLUS: 	// "+" key
								if (PX == 64){
									PX = 128;
									if(verbose)printf("[GRAPHICS] Zoom In\n");
									XPOS *= 2;
									YPOS *= 2;
									displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
								}
								break;
							case SDLK_KP_MINUS:	// "-" key
								if (PX == 128){
									PX = 64;
									if(verbose)printf("[GRAPHICS] Zoom Out\n");
									XPOS /= 2;
									YPOS /= 2;
									displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
								}
								break;
							case SDLK_z:		// "z" key
								YPOS += (10*(PX/64));
								displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
								break;
							case SDLK_q:		// "q" key
								XPOS += (10*(PX/64));
								displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
								break;
							case SDLK_s:		// "s" key
								YPOS -= (10*(PX/64));
								displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
								break;
							case SDLK_d:		// "d" key
								XPOS -= (10*(PX/64));
								displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
								break;
						}
					break;
					case SDL_MOUSEMOTION:
						hover_ability = -1;
						// Compétences et actions
						tempEntity = getEntity(matrix, getSelectedPos());
						if (tempEntity != NULL)
						{
							if (e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-16)
							{
								if (e.motion.x >= 16 && e.motion.x <= 80)	hover_ability = 0;
								if (e.motion.x >= 96 && e.motion.x <= 160)	hover_ability = tempEntity->cha_class->cla_abilities[0].ab_id;
								if (e.motion.x >= 176 && e.motion.x <= 240)	hover_ability = tempEntity->cha_class->cla_abilities[1].ab_id;
								if (e.motion.x >= 256 && e.motion.x <= 320)	hover_ability = tempEntity->cha_class->cla_abilities[2].ab_id;
								if (e.motion.x >= 336 && e.motion.x <= 400)	hover_ability = tempEntity->cha_class->cla_abilities[3].ab_id;
								displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
							}
						}
						displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);

					break;
				}
			}

			// Déplacement de la caméra grâce aux bords de l'écran
			if (e.motion.x <= xWinSize && e.motion.x >= xWinSize-20){
				XPOS -= (5*(PX/64));
				displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
			}
			if (e.motion.x >= 0 && e.motion.x <= 20){
				XPOS += (5*(PX/64));
				displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
			}
			if (e.motion.y <= yWinSize && e.motion.y >= yWinSize-20){
				YPOS -= (5*(PX/64));
				displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
			}
			if (e.motion.y <= 10){
				YPOS += (5*(PX/64));
				displayMap(renderer, XPOS, YPOS, PX, matrix, _X_SIZE_, _Y_SIZE_, cSprites);
			}

			// Vérification pour ne pas dépasser des "border" avec la caméra
			if (XPOS > 500*(PX/64)) 	XPOS = 500*(PX/64);
			if (XPOS < -1000*(PX/64)) 	XPOS = -1000*(PX/64);
			if (YPOS > 300*(PX/64))		YPOS = 300*(PX/64);
			if (YPOS < -500*(PX/64)) 	YPOS = -500*(PX/64);
			SDL_Delay(16);

		}
		closeWindow(pWindow);
	} else {
		fprintf(stderr,"[GRAPHICS] Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}

	return 1;
}