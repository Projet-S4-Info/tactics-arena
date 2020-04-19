
/* =============== DEPENDENCES =============== */


#include <stdio.h>
#include <string.h>
#include <pthread.h>
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
#include "turn.h"
#include "gameplay.h"
#include "chat.h"
#include "print.h"
#include "servFcnt.h"
#include "text.h"


/* =============== CONSTANTES ================ */


#define _NB_MAX_MAPS_ 50					// Max number of maps being listed
#define _X_SIZE_ 30
#define _Y_SIZE_ 30
#define _FPS_ 60							// Define at which frequency the game have to refresh


/* =============== VARIABLES ================ */


SDL_Renderer *renderer = NULL;
SDL_Window* pWindow = NULL;
int XPOS = 50;								// x and y pos where map is displayed
int YPOS = 50;								// |
int selected_ability = -1;					// Selected ability
int hover_ability = -1;						// Hover ability button
bool hover_next_turn = FALSE;				// Hover skip turn button
int hover_tchat = 0;						// Hover tchat button
int hover_passive_help = 0;					// Hover passive help in ID card (with mouse position)
int end_of_turn = 0;						// Fin de tour
int isChatActive = 0;						// Chat button
Direction camMove = -1;
int *exitThread;

int xWinSize, yWinSize;						// x and y sizes of the window
Coord mouse_position;

char pseudoChat[STR_SHORT] = "Chat : ";
int changesChat = 0;


char *compo;

extern Sint32 cursor;
extern Sint32 selection_len;

pthread_t thread_Chat;

static void * fn_chat (void * p_data)
{
    startChat(&chat,sizeof(chat),socketConnected);
    return NULL;
}


/* =============== FONCTIONS ================ */


int createGameWindow(int x, int y)
// Create a window with with x*y size (in px)
{
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

	printf("%s", error_message[setRendererDriver(renderer)]);

	// Launcher icon
    SDL_SetWindowIcon(pWindow, loadImage("../inc/sprites/goliath/sprite_indiv/front/Sprite_frontview_64.png"));

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
			displayText(renderer, 200, yWinSize/2+120, 40, "Chargement des textures du jeu...", "../inc/font/Pixels.ttf", 255, 255, 255, TRUE);
			displayText(renderer, 200, yWinSize/2, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255, TRUE);
			if (load_index == 1)		displayText(renderer, xWinSize/2, yWinSize/3*2, 60, "Ooo", "../inc/font/Aqua.ttf", 255, 255, 255, TRUE);
			else if (load_index == 2)	displayText(renderer, xWinSize/2, yWinSize/3*2, 60, "oOo", "../inc/font/Aqua.ttf", 255, 255, 255, TRUE);
			else if (load_index == 3)	displayText(renderer, xWinSize/2, yWinSize/3*2, 60, "ooO", "../inc/font/Aqua.ttf", 255, 255, 255, TRUE);
			SDL_Delay(100);
			SDL_RenderPresent(renderer);
			SDL_Delay(900);
		}

		/* Le fond de la fenêtre sera blanc */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		loadMap(matrix, "map_ice_hole");

		debugGrid(matrix, _X_SIZE_, _Y_SIZE_);

		ent_init_test(Allies, "Friendly");
        ent_init_test(Foes, "Ennemy");

		SDL_Delay(1);

		if (verbose)
		{
			for (int i=0; i < NUM_CLASS; i++)
			{
				print_Coord(&Allies[i].coords, "");
				print_Coord(&Foes[i].coords, "");
			}
		}

		SDL_RenderPresent(renderer);

		Entity * tempEntity = NULL;

		init_chat(&chat);

		/*--------- to test -----------*/
		char temp[50] = "THILOUROCIEN";
		/*-----------------------------*/
		if(nbPlayer == 0){
			sprintf(pseudoUser, "%s", temp);
		}
		sprintf(pseudoChat, "%s : ", pseudoUser);


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

					/* ********** CLICS SOURIS ************ */
					case SDL_MOUSEBUTTONDOWN:

						if(verbose)printf("X: %d | Y: %d\n", e.motion.x, e.motion.y);		// Debug console pos x & y on term

						// Compétences et actions
						tempEntity = getEntity(getSelectedPos());
						if (tempEntity != NULL && is_ally(tempEntity))
						{
							if (e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-16)
							{
								if (your_turn())
								{
									if (e.motion.x >= 16 && e.motion.x <= 80)
										if (able_ability(tempEntity, Mvt)) selected_ability = Mvt;
										else addLog("Not enough AP or MP to move");
									else if (e.motion.x >= 96 && e.motion.x <= 160)
										if (able_ability(tempEntity, tempEntity->cha_class->cla_abilities[0].ab_id)) selected_ability = tempEntity->cha_class->cla_abilities[0].ab_id;
										else addLog("Not enough AP to cast this spell");
									else if (e.motion.x >= 176 && e.motion.x <= 240)
										if (able_ability(tempEntity, tempEntity->cha_class->cla_abilities[1].ab_id)) selected_ability = tempEntity->cha_class->cla_abilities[1].ab_id;
										else addLog("Not enough AP to cast this spell");
									else if (e.motion.x >= 256 && e.motion.x <= 320)
										if (able_ability(tempEntity, tempEntity->cha_class->cla_abilities[2].ab_id)) selected_ability = tempEntity->cha_class->cla_abilities[2].ab_id;
										else addLog("Not enough AP to cast this spell");
									else if (e.motion.x >= 336 && e.motion.x <= 400)
										if (able_ability(tempEntity, tempEntity->cha_class->cla_abilities[3].ab_id)) selected_ability = tempEntity->cha_class->cla_abilities[3].ab_id;
										else addLog("Not enough AP to cast this spell");
									else if (e.motion.x >= 416 && e.motion.x <= 480) turnRight(tempEntity);
									else if (e.motion.x >= 496 && e.motion.x <= 560) turnLeft(tempEntity);
									else selectTile(XPOS, YPOS, e.motion.x, e.motion.y);

									if(verbose)printf("Selected ability : %d\n", selected_ability);
								}

							} else selectTile(XPOS, YPOS, e.motion.x, e.motion.y);
							
						} 
						else 
						{
							selected_ability = -1;
							selectTile(XPOS, YPOS, e.motion.x, e.motion.y);
						}

						if(e.motion.x >= xWinSize-360 && e.motion.x <= xWinSize-296 && e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-16){
							if (isChatActive == 1){
								isChatActive = 0;

								addLog("Tchat desactive");
								pthread_detach(thread_Chat);
							}
							else
							{
								isChatActive = 1;
								addLog("Tchat active");
								if(nbPlayer > 0){
									pthread_create(&thread_Chat, NULL, fn_chat, NULL);
								}

							}
						}
					break;

					/* ********** SCROLL SOURIS ************ */
					case SDL_MOUSEWHEEL:
						if (e.wheel.y > 0)		// Scroll UP
						{
							if (pxBase == 64){
								pxBase = 128;
								if(verbose)printf("[GRAPHICS] Zoom In\n");
								XPOS *= 2;
								YPOS *= 2;
							}
						} else {				// Scroll DOWN
							if (pxBase == 128){
								pxBase = 64;
								if(verbose)printf("[GRAPHICS] Zoom Out\n");
								XPOS /= 2;
								YPOS /= 2;
							}
						}
					break;

					/* ********** APPUI TOUCHE CLAVIER ************ */
					case SDL_KEYDOWN:
						switch(e.key.keysym.sym)
						{
							case SDLK_KP_PLUS: 	// "+" key
								if (pxBase == 64){
									pxBase = 128;
									addLog("PX 128");
									if(verbose)printf("[GRAPHICS] Zoom In\n");
									XPOS *= 2;
									YPOS *= 2;	
								}
								break;
							case SDLK_KP_MINUS:	// "-" key
								if (pxBase == 128){
									pxBase = 64;
									addLog("PX 64");
									if(verbose)printf("[GRAPHICS] Zoom Out\n");
									XPOS /= 2;
									YPOS /= 2;
								}
								break;
							case SDLK_BACKSPACE:
								if(isChatActive == 1){
									if (strlen(pseudoChat) > strlen(pseudoUser) + 3){
										pseudoChat[strlen(pseudoChat)-1] = '\0';
									}
								}
								break;
							case SDLK_RETURN:
								if(isChatActive == 1){
									
									nouveau_Msg(&chat, pseudoChat);
									sprintf(pseudoChat, "%s : ",pseudoUser);
									changesChat = 1;
								}
						}
					break;

					/* ********** MOUVEMENTS SOURIS ************ */
					case SDL_MOUSEMOTION:
						hover_ability = -1;
						hover_next_turn = FALSE;
						hover_tchat = 0;
						mouse_position.x = e.motion.x;
						mouse_position.y = e.motion.y;

						// Hover skip turn button
						if (e.motion.x >= xWinSize-280 && e.motion.x <= xWinSize-24 && e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-16) hover_next_turn = TRUE;

						// Hover compétences et actions
						tempEntity = getEntity(getSelectedPos());
						if (tempEntity != NULL)
						{
							if (e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-16)
							{
								if (e.motion.x >= 16 && e.motion.x <= 80)			hover_ability = Mvt;
								else if (e.motion.x >= 96 && e.motion.x <= 160)		hover_ability = tempEntity->cha_class->cla_abilities[0].ab_id;
								else if (e.motion.x >= 176 && e.motion.x <= 240)	hover_ability = tempEntity->cha_class->cla_abilities[1].ab_id;
								else if (e.motion.x >= 256 && e.motion.x <= 320)	hover_ability = tempEntity->cha_class->cla_abilities[2].ab_id;
								else if (e.motion.x >= 336 && e.motion.x <= 400)	hover_ability = tempEntity->cha_class->cla_abilities[3].ab_id;
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

						// Hover tchat button
						if(e.motion.x >= xWinSize-360 && e.motion.x <= xWinSize-296 && e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-16){
							if (isChatActive == 1){
								hover_tchat = 1;
							}
							else
							{
								hover_tchat = 2;
							}
						}
					break;
					

					case SDL_TEXTINPUT:
						if(isChatActive == 1){
							strcat(pseudoChat,e.text.text);
						}
					break;


					case SDL_TEXTEDITING:
						if(isChatActive == 1){
							compo = e.edit.text;
							cursor = e.edit.start;
							selection_len = e.edit.length;
						}	
					break;
				}
			}

			/* ********** MOUVEMENTS CAMERA ************ */
			if (SDL_GetMouseFocus() == pWindow)
			{
				camMove = -1;
				// Déplacement de la caméra grâce aux bords de l'écran
				if (mouse_position.x <= xWinSize && mouse_position.x >= xWinSize-20){
					XPOS -= (camSpeed*(pxBase/64));
					camMove = E;
				}
				if (mouse_position.x <= 20 && mouse_position.x >= 0){
					XPOS += (camSpeed*(pxBase/64));
					camMove = W;
				}
				if (mouse_position.y <= yWinSize && mouse_position.y >= yWinSize-20){
					YPOS -= (camSpeed*(pxBase/64));
					camMove = S;
				}
				if (mouse_position.y <= 20 && mouse_position.y >= 0){
					YPOS += (camSpeed*(pxBase/64));
					camMove = N;
				}
				// Vérification pour ne pas dépasser des "border" avec la caméra
				if (XPOS > 500*(pxBase/64)) 	XPOS = 500*(pxBase/64);
				if (XPOS < -1000*(pxBase/64)) 	XPOS = -1000*(pxBase/64);
				if (YPOS > 300*(pxBase/64))		YPOS = 300*(pxBase/64);
				if (YPOS < -500*(pxBase/64)) 	YPOS = -500*(pxBase/64);

				displayMap(renderer, XPOS, YPOS);
			}

			SDL_Delay(1000/_FPS_);
			//clearOldCache();

		}
		closeWindow(pWindow);
	} else {
		fprintf(stderr,"[GRAPHICS] Erreur de création de la fenêtre: %s\n", SDL_GetError());
	}

	return 1;
}