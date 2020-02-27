#include <stdio.h>
#include <string.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "audio.h"
#include "map.h"
#include "struct.h"
#include "menu.h"

void setRendererDriver(SDL_Renderer *renderer)
// Set the default renderer driver to OpenGL for MacOS compatibility
{
	SDL_RendererInfo *global_renderer_info = malloc(sizeof(SDL_RendererInfo));

	if (SDL_GetRendererInfo(renderer, global_renderer_info) != 0)
    {
        printf("[GRAPHICS] Erreur lors de l'obtention des informations du renderer : %s\n", SDL_GetError());
		return;
    } else {
		printf("Driver utilisé par le renderer : %s\n", global_renderer_info->name);
		printf("Résolution maximale des textures : %dpx / %dpx\n", global_renderer_info->max_texture_width, global_renderer_info->max_texture_height);
	}

	if (!strcmp(global_renderer_info->name, "opengl")){
		if (SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl"))
		{
			printf("[GRAPHICS] Le driver utilisé est maintenant OpenGL\n");
		} else {
			printf("[GRAPHICS] Erreur lors du changement de driver : %s\n", SDL_GetError());
			return;
		}
	}
	
	free(global_renderer_info);
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
		fprintf(stderr, "[GRAPHICS ERROR] loadTexture error while creating texture : %s\n", SDL_GetError());
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

int createGameWindow(int x, int y, Tile * grid, int xSize, int ySize)
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
    if (SDL_Init(SDL_INIT_VIDEO) != 0 ) {
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

		loadMapTextures(renderer);

		/* Le fond de la fenêtre sera blanc */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_Delay(1);

		displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);

		SDL_RenderPresent(renderer);

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

								/* Le fond de la fenêtre sera blanc */
                				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
								SDL_RenderClear(renderer);

								SDL_Delay(1);

								displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);

								SDL_RenderPresent(renderer);

							break;
						}
					break;
					case SDL_MOUSEBUTTONDOWN:

						printf("X: %d | Y: %d\n", e.motion.x, e.motion.y);		// Debug console pos x & y on term
						//if (e.motion.x <= 10*64+XPOS && e.motion.y <= 10*64+YPOS && e.motion.x >= XPOS && e.motion.y >= YPOS){
							selectTile(grid, XPOS, YPOS, e.motion.x, e.motion.y, PX, xSize, ySize);
							displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);
						//}

					break;
					case SDL_MOUSEWHEEL:
						if (e.wheel.y > 0)		// Scroll UP
						{
							if (PX == 64){
								PX = 128;
								printf("[GRAPHICS] Zoom In\n");
								XPOS *= 2;
								YPOS *= 2;
								displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);
							}
						} else {				// Scroll DOWN
							if (PX == 128){
								PX = 64;
								printf("[GRAPHICS] Zoom Out\n");
								XPOS /= 2;
								YPOS /= 2;
								displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);
							}
						}
					break;
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym)
						{
							case SDLK_KP_PLUS: 	// "+" key
								if (PX == 64){
									PX = 128;
									printf("[GRAPHICS] Zoom In\n");
									XPOS *= 2;
									YPOS *= 2;
									displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);
								}
								break;
							case SDLK_KP_MINUS:	// "-" key
								if (PX == 128){
									PX = 64;
									printf("[GRAPHICS] Zoom Out\n");
									XPOS /= 2;
									YPOS /= 2;
									displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);
								}
								break;
							case SDLK_z:		// "z" key
								YPOS += (10*(PX/64));
								displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);
								break;
							case SDLK_q:		// "q" key
								XPOS += (10*(PX/64));
								displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);
								break;
							case SDLK_s:		// "s" key
								YPOS -= (10*(PX/64));
								displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);
								break;
							case SDLK_d:		// "d" key
								XPOS -= (10*(PX/64));
								displayMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize);
								break;
						}
					break;
					case SDL_MOUSEMOTION:

						if (e.motion.x >= 10 && e.motion.y >= 10)
						{

						}

					break;
				}
			}
			SDL_Delay(16);
		}
		closeWindow(pWindow);
	} else {
		fprintf(stderr,"[GRAPHICS] Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}

	return 1;
}