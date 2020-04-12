
/* =============== DEPENDENCES =============== */


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
#include "display.h"
#include "textures.h"	


/* =============== CONSTANTES ================ */


#define _NB_MAX_MAPS_ 50
#define _X_SIZE_ 30
#define _Y_SIZE_ 30
#define _FPS_ 60							// Define at which frequency the game have to refresh


/* =============== VARIABLES ================ */


int selected_ability = -1;					// Selected ability
int hover_ability = -1;						// Hover ability
int hover_passive_help = 0;					// Hover passive help in ID card (with mouse position)
int end_of_turn = 0;						// Fin de tour
int isChatActive = 0;						// Chat button

int xWinSize, yWinSize;						// x and y sizes of the window
Coord mouse_position;


/* =============== FONCTIONS ================ */


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

		Coord testCoord = {rand()%30,rand()%30};
		setEntityToTile(&Allies[0], testCoord);
		Coord testCoord2 = {rand()%30,rand()%30};
		setEntityToTile(&Allies[1], testCoord2);
		Coord testCoord3 = {rand()%30,rand()%30};
		setEntityToTile(&Allies[2], testCoord3);
		Coord testCoord4 = {rand()%30,rand()%30};
		setEntityToTile(&Allies[3], testCoord4);
		Coord testCoord5 = {rand()%30,rand()%30};
		setEntityToTile(&Allies[4], testCoord5);
		Coord testCoord6 = {rand()%30,rand()%30};
		setEntityToTile(&Allies[5], testCoord6);

		displayMap(renderer, XPOS, YPOS);

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

							break;
						}
					break;
					case SDL_MOUSEBUTTONDOWN:

						if(verbose)printf("X: %d | Y: %d\n", e.motion.x, e.motion.y);		// Debug console pos x & y on term
						selectTile(XPOS, YPOS, e.motion.x, e.motion.y);

						// Compétences et actions
						tempEntity = getEntity(getSelectedPos());
						if (tempEntity != NULL)
						{
							if (e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-16)
							{
								if (e.motion.x >= 16 && e.motion.x <= 80)	selected_ability = Mvt;
								if (e.motion.x >= 96 && e.motion.x <= 160)	selected_ability = tempEntity->cha_class->cla_abilities[0].ab_id;
								if (e.motion.x >= 176 && e.motion.x <= 240)	selected_ability = tempEntity->cha_class->cla_abilities[1].ab_id;
								if (e.motion.x >= 256 && e.motion.x <= 320)	selected_ability = tempEntity->cha_class->cla_abilities[2].ab_id;
								if (e.motion.x >= 336 && e.motion.x <= 400)	selected_ability = tempEntity->cha_class->cla_abilities[3].ab_id;
								if(verbose)printf("Selected ability : %d\n", selected_ability);
							}
						}

						if(e.motion.x >= 1653 && e.motion.x <= 1889 && e.motion.y >= 873 && e.motion.y <= 925){
							isChatActive = 1;
						}
						else if(e.motion.x >= 1863 && e.motion.x <= 1894 && e.motion.y >= 527 && e.motion.y <= 556 && isChatActive == 1){
							isChatActive = 0;
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
							}
						} else {				// Scroll DOWN
							if (PX == 128){
								PX = 64;
								if(verbose)printf("[GRAPHICS] Zoom Out\n");
								XPOS /= 2;
								YPOS /= 2;
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
								}
								break;
							case SDLK_KP_MINUS:	// "-" key
								if (PX == 128){
									PX = 64;
									if(verbose)printf("[GRAPHICS] Zoom Out\n");
									XPOS /= 2;
									YPOS /= 2;
								}
								break;
							case SDLK_z:		// "z" key
								YPOS += (camSpeed*(PX/64));
								break;
							case SDLK_q:		// "q" key
								XPOS += (camSpeed*(PX/64));
								break;
							case SDLK_s:		// "s" key
								YPOS -= (camSpeed*(PX/64));
								break;
							case SDLK_d:		// "d" key
								XPOS -= (camSpeed*(PX/64));
								break;
						}
					break;
					case SDL_MOUSEMOTION:
						hover_ability = -1;
						// Compétences et actions
						tempEntity = getEntity(getSelectedPos());
						if (tempEntity != NULL)
						{
							if (e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-16)
							{
								if (e.motion.x >= 16 && e.motion.x <= 80)	hover_ability = Mvt;
								if (e.motion.x >= 96 && e.motion.x <= 160)	hover_ability = tempEntity->cha_class->cla_abilities[0].ab_id;
								if (e.motion.x >= 176 && e.motion.x <= 240)	hover_ability = tempEntity->cha_class->cla_abilities[1].ab_id;
								if (e.motion.x >= 256 && e.motion.x <= 320)	hover_ability = tempEntity->cha_class->cla_abilities[2].ab_id;
								if (e.motion.x >= 336 && e.motion.x <= 400)	hover_ability = tempEntity->cha_class->cla_abilities[3].ab_id;
								
							}
							else if (e.motion.x >= 377 && e.motion.x <= 396 && e.motion.y >= 128 && e.motion.y <= 146)
							{
								hover_passive_help = 1;
								mouse_position.x = e.motion.x;
								mouse_position.y = e.motion.y;
							} else {
								hover_passive_help = 0;
							}
						}
					break;
				}
			}

			if (SDL_GetMouseFocus() == pWindow)
			{
				// Déplacement de la caméra grâce aux bords de l'écran
				if (e.motion.x <= xWinSize && e.motion.x >= xWinSize-20){
					XPOS -= (camSpeed*(PX/64));
				}
				if (e.motion.x >= 0 && e.motion.x <= 20){
					XPOS += (camSpeed*(PX/64));
				}
				if (e.motion.y <= yWinSize && e.motion.y >= yWinSize-20){
					YPOS -= (camSpeed*(PX/64));
				}
				if (e.motion.y <= 20){
					YPOS += (camSpeed*(PX/64));
				}
				// Vérification pour ne pas dépasser des "border" avec la caméra
				if (XPOS > 500*(PX/64)) 	XPOS = 500*(PX/64);
				if (XPOS < -1000*(PX/64)) 	XPOS = -1000*(PX/64);
				if (YPOS > 300*(PX/64))		YPOS = 300*(PX/64);
				if (YPOS < -500*(PX/64)) 	YPOS = -500*(PX/64);

				displayMap(renderer, XPOS, YPOS);
			}

			SDL_Delay(1000/_FPS_);

		}
		closeWindow(pWindow);
	} else {
		fprintf(stderr,"[GRAPHICS] Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}

	return 1;
}