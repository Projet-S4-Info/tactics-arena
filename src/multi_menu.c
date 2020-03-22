#include <stdio.h>
#include <stdlib.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "audio.h"
#include "map.h"
#include "struct.h"
#include "graphics.h"
#include "Socket_Server.h"
#include "menu_multi.h"
#include <unistd.h>
#include <string.h>
#include <pthread.h>




SDL_Texture *background_Multi = NULL,
			*quit_button_Multi = NULL,
			*music_on_Multi = NULL,
			*music_off_Multi = NULL,
			*join_button = NULL,
			*host_button = NULL;

int isHostMenu = 0;
int isJoinMenu = 0;
int music_Multi_playing = 1;

char consoleLog[50] = "Host : ";

char *compo;

extern Sint32 cursor;
extern Sint32 selection_len;

// Initialisation du thread 
typedef struct
{
   int stock;
 
   pthread_t thread_server;
   pthread_t thread_client;
}
multiThread_t;

static multiThread_t otherThread =
{
   .stock = 0,
};
 
/* Fonction pour le thread du magasin. */
static void * fn_server (void * p_data)
{
    startTCPSocketServ();
    return NULL;
}

void loadMultiMenuTextures(SDL_Renderer *renderer)
// Load all the textures needed for the menu
{
	printf("Chargement des textures du menu...\n");

	// == Loading background ==
	background_Multi = loadTexture(renderer, loadImage("../inc/img/menu2_720p.png"));

	// == Loading quit button ==
	quit_button_Multi = loadTexture(renderer, loadImage("../inc/img/quit_button_256.png"));

	// == Loading music ON switch ==
	music_on_Multi = loadTexture(renderer, loadImage("../inc/img/music_on.png"));

	// == Loading music OFF switch ==
	music_off_Multi = loadTexture(renderer, loadImage("../inc/img/music_off.png"));

	// == Loading Host switch ==
	host_button = loadTexture(renderer, loadImage("../inc/img/start_button_256.png"));

	// == Loading Join switch ==
	join_button = loadTexture(renderer, loadImage("../inc/img/start_button_256.png"));

}

void freeMultiMenuTextures()
// Free all the textures loaded
{
	SDL_DestroyTexture(background_Multi);
	SDL_DestroyTexture(quit_button_Multi);
	SDL_DestroyTexture(music_on_Multi);
	SDL_DestroyTexture(music_off_Multi);
}

void dispHostLog(SDL_Renderer *renderer){
	switch (logFlag)
    {
		case 1 : displayText(renderer, 50, 400, 15, "Création du serveur", "../inc/font/PixelOperator.ttf", 255, 255, 255);
		break;
    }
}

void dispHostMenu(SDL_Renderer *renderer, int x, int y){

	SDL_Rect console;
			console.x = 40;
			console.y = 350;
			console.w = 600;
			console.h = 300;
	
    /* Background image */
	displaySprite(renderer, background_Multi, 0, 0);
    /* Affiche en gros Tactics Arena */
	displayText(renderer, 300, 200, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255);
    /* Quit button */
    displaySprite(renderer, quit_button_Multi, x-300, y-190);
    
    /* initialise la couleur sur noir */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 );
    /*remplis le rectange*/
    SDL_RenderFillRect(renderer, &console);
    /* Petit text de confirmation */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
    // displayText(renderer, 50, 400, 15, "Host est séléctioné", "../inc/font/PixelOperator.ttf", 255, 255, 255);
    
    if (consoleLog != NULL) displayText(renderer, 50, 422, 15, consoleLog, "../inc/font/PixelOperator.ttf", 255, 255, 255);
	dispHostLog(renderer);
}





void dispJoinMenu(SDL_Renderer *renderer, int x, int y)
{
	/* Petit text de confirmation */
	displayText(renderer, 80, 450, 15, "Join est séléctioné", "../inc/font/PixelOperator.ttf", 255, 255, 255);
	
	/* Quit button */
	displaySprite(renderer, quit_button_Multi, 515, 525);

}


int displayMenuMulti(int x, int y)
// Create a window with with x*y size (in px)
{

    SDL_Window* pWindow = NULL;
	SDL_Renderer *renderer=NULL;

    int xWinSize;
	int yWinSize;

     /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0 ) {
        fprintf(stdout,"[MENU] Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

	/* Initialisation TTF */
	if(TTF_Init() == -1) {
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

    /* Création de la fenêtre */
	pWindow = SDL_CreateWindow("Tactics Arena - Menu", 	SDL_WINDOWPOS_UNDEFINED,
												  		SDL_WINDOWPOS_UNDEFINED,
												  		x,
												  		y,
												  		SDL_WINDOW_SHOWN);

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
		int running = 1;
		while(running) {
			SDL_Event u;
			while(SDL_PollEvent(&u)) {
                if(isHostMenu == 1 || isJoinMenu == 1){
                    if(isJoinMenu == 1){
                        dispJoinMenu(renderer, x, y);
                    }else{
                        dispHostMenu(renderer, x, y);
                    }
                }
				switch(u.type) {
					case SDL_QUIT: running = 0;
					break;
					case SDL_WINDOWEVENT:
						switch(u.window.event){
							case SDL_WINDOWEVENT_EXPOSED:
							case SDL_WINDOWEVENT_SIZE_CHANGED:
							case SDL_WINDOWEVENT_RESIZED:
							case SDL_WINDOWEVENT_HIDDEN:
							case SDL_WINDOWEVENT_SHOWN:

								loadMultiMenuTextures(renderer);
                                if(isHostMenu == 0 && isJoinMenu == 0){

                                    /* Background image */
	                                displaySprite(renderer, background_Multi, 0, 0);
                                    /* Host button */
	                                displaySprite(renderer, host_button, 500, 350);
                                    /* Join button */
	                                displaySprite(renderer, join_button, 500, 415);
                                    /* Quit button */
                                    displaySprite(renderer, quit_button_Multi, x-300, y-190);
                                    /* Affiche en gros Tactics Arena */
	                                displayText(renderer, 300, 200, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255);
	                                /* Mentions de bas de menu */
	                                displayText(renderer, 5, y-20, 15, "Projet L2 Informatique - BUTEL CHAUVIN DOUCET LAFAY", "../inc/font/Pixels.ttf", 255, 255, 255);
                                }
								
								// SDL_RenderPresent(renderer);
								
							break;
						}
					break;
					case SDL_MOUSEBUTTONDOWN:

						printf("X: %d | Y: %d\n", u.motion.x, u.motion.y);	// Debug console pos x & y on term

						// Bouton "Host"
						if (u.motion.x >= 576 && u.motion.x <= 723 && u.motion.y >= 449 && u.motion.y <= 488 && isJoinMenu == 0)
						{
							isHostMenu = 1;
                            printf("Host cliqué :) \n");
                            pthread_create (& otherThread.thread_server, NULL, fn_server, NULL);
                            dispHostMenu(renderer, x, y);
							
                            
                            
						}

						// Bouton "Join"
						else if (u.motion.x >= 575 && u.motion.x <= 724 && u.motion.y >= 513 && u.motion.y <= 554 && isHostMenu == 0)
						{
							isJoinMenu = 1;
                            printf("Join cliqué :) \n");
						}

						// Bouton "Host"
						else if (u.motion.x >= 160 && u.motion.x <= 305 && u.motion.y >= 390 && u.motion.y <= 450)
						{	

							// dispHostMenu(renderer, x, y);
							// printf("Host button touch : \n");
							// printf("multi : %d |", isMultiMenu);
							// printf("host : %d |", isHostButton);
							// printf("join : %d |", isJoinButton);
							//SDL_RenderPresent(renderer);
							// pthread_create (& otherThread.thread_server, NULL, fn_server, NULL);
							
							
						}
						
						// Nouveau boutton "QUIT" 
						else if (u.motion.x >= 1000 && u.motion.x <= 1190 && u.motion.y >= 627 && u.motion.y <= 680 ){
							closeWindow(pWindow);
							freeMultiMenuTextures();
						}

						// // Bouton "Join"
						// else if (u.motion.x >= 160 && u.motion.x <= 305 && u.motion.y >= 465 && u.motion.y <= 525)
						// {

						// 	// dispJoinMenu(renderer, x, y);
						// 	// SDL_RenderPresent(renderer);
						// 	// pthread_create (& otherThread.thread_client, NULL, fn_client, NULL);
						// }

						// Switch musique ON/OFF
						else if (u.motion.x >= 1202 && u.motion.x <= 1250 && u.motion.y >= 627 && u.motion.y <= 680)
						{
							if (music_Multi_playing)
							{
								music_Multi_playing = 0;
								pauseMenuMusic();
								displaySprite(renderer, music_off_Multi, x-175, y-200);
							}
							else
							{
								music_Multi_playing = 1;
								resumeMenuMusic();
								displaySprite(renderer, music_on_Multi, x-175, y-200);
							}
						}
						
						// Bouton "Quit"
						else if (u.motion.x >= 569 && u.motion.x <= 730 && u.motion.y >= 613 && u.motion.y <= 673)
						{
							closeWindow(pWindow);
							freeMultiMenuTextures();
						}
					break;
					case SDL_KEYDOWN:
						if (u.key.keysym.sym == SDLK_BACKSPACE)
							{
								if (strlen(consoleLog) > 6){
									consoleLog[strlen(consoleLog)-1] = '\0';
									dispHostMenu(renderer, x, y);
								}
							}
                    break;
					case SDL_TEXTINPUT:
						if (isHostMenu == 1)
						{
							strcat(consoleLog,u.text.text);
							printf("%s\n",u.text.text);
                            printf("hostmenu = %d", isHostMenu);
							// printf("Host : %d \nMulti : %d \n",isHostButton,isMultiMenu);
							dispHostMenu(renderer, x, y);
							// SDL_RenderPresent(renderer);

						}
					break;
					case SDL_TEXTEDITING:
						if ( isHostMenu == 1){
							compo = u.edit.text;
							cursor = u.edit.start;
							selection_len = u.edit.length;
							dispHostMenu(renderer, x, y);
							// SDL_RenderPresent(renderer);
					    }
                    break;
				}
			}
			SDL_Delay(16);
			SDL_RenderPresent(renderer);
		}
		closeWindow(pWindow);
		freeMultiMenuTextures();
        SDL_StopTextInput();
	} else {
		fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}

	return 1;
}
