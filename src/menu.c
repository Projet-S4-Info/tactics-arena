#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "audio.h"
#include "map.h"
#include "struct.h"
#include "graphics.h"
#include "Socket_Server.h"
#include "map_editor.h"

#ifdef _WIN32
#  include <windows.h>
#  define psleep(sec) Sleep ((sec) * 1000)
#else
#  include <unistd.h>
#  define psleep(sec) sleep ((sec))
#endif

#define XPOS 500			// x position of the grid
#define YPOS 100			// y position of the grid
#define _NB_MAX_MAPS_ 20



SDL_Texture *background = NULL,
			*start_button = NULL,
			*quit_button = NULL,
			*music_on = NULL,
			*music_off = NULL,
			*map_editor_button = NULL,
			*multi_button = NULL,
			*join_button = NULL,
			*host_button = NULL;

// La musique est activée de base
int music_playing = 1;
int isMultiMenu = 0;
int isHostButton = 0;
int isJoinButton = 0;



// Map list
char *mapList[_NB_MAX_MAPS_];
int mapIndex = 0;

// multi

char consoleLog[50] = "log_:";
char *compo;
Sint32 cursor;
Sint32 selection_len;

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

static void * fn_client (void * p_data)
{
    startTCPSocketCli();
    return NULL;
}



void loadMenuTextures(SDL_Renderer *renderer)
// Load all the textures needed for the menu
{
	printf("Chargement des textures du menu...\n");

	// == Loading background ==
	background = loadTexture(renderer, loadImage("../inc/img/menu2_720p.png"));

	// == Loading start button ==
	start_button = loadTexture(renderer, loadImage("../inc/img/start_button_256.png"));

	// == Loading quit button ==
	quit_button = loadTexture(renderer, loadImage("../inc/img/quit_button_256.png"));

	// == Loading music ON switch ==
	music_on = loadTexture(renderer, loadImage("../inc/img/music_on.png"));

	// == Loading music OFF switch ==
	music_off = loadTexture(renderer, loadImage("../inc/img/music_off.png"));

	// == Loading map editor button ==
	map_editor_button = loadTexture(renderer, loadImage("../inc/img/map_editor_button_256.png"));
	
	// == Loading Multi switch ==
	multi_button = loadTexture(renderer, loadImage("../inc/img/start_button_256.png"));

	// == Loading Host switch ==
	host_button = loadTexture(renderer, loadImage("../inc/img/start_button_256.png"));

	// == Loading Join switch ==
	join_button = loadTexture(renderer, loadImage("../inc/img/start_button_256.png"));

}

void freeMenuTextures()
// Free all the textures loaded
{
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(start_button);
	SDL_DestroyTexture(quit_button);
	SDL_DestroyTexture(music_on);
	SDL_DestroyTexture(music_off);
	SDL_DestroyTexture(map_editor_button);
}

void dispHostMenu(SDL_Renderer *renderer, int x, int y){

	SDL_Rect console;
			console.x = 40;
			console.y = 350;
			console.w = 600;
			console.h = 300;
		/* initialise la couleur sur noir */
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 );
		/*remplis le rectange*/
		SDL_RenderFillRect(renderer, &console);
		/* Petit text de confirmation */
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
		displayText(renderer, 50, 400, 15, "Host est séléctioné", "../inc/font/PixelOperator.ttf", 255, 255, 255);
		displayText(renderer, 50, 422, 15, consoleLog, "../inc/font/PixelOperator.ttf", 255, 255, 255);

		displaySprite(renderer, quit_button, x-300, y-190);
		
}

void dispMultiMenu(SDL_Renderer *renderer, int x, int y){
	displaySprite(renderer, background, 0, 0);
	/* Host button */
	displaySprite(renderer, host_button, 75, 300);

	/* Join bouton */
	displaySprite(renderer, join_button, 75, 375);

	/* Quit button */
	displaySprite(renderer, quit_button, 515, 525);

		/* Affiche en gros Tactics Arena */
	displayText(renderer, 300, 200, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255);

	/* Mentions de bas de menu */
	displayText(renderer, 5, y-20, 15, "Projet L2 Informatique - BUTEL CHAUVIN DOUCET LAFAY", "../inc/font/Pixels.ttf", 255, 255, 255);
}

void dispJoinMenu(SDL_Renderer *renderer, int x, int y)
{
	/* Petit text de confirmation */
	displayText(renderer, 80, 450, 15, "Join est séléctioné", "../inc/font/PixelOperator.ttf", 255, 255, 255);
	
	/* Quit button */
	displaySprite(renderer, quit_button, 515, 525);

}

void updateMenu(SDL_Renderer *renderer, int x, int y)
// Update the menu display
{
	printf("Updte Menu : \n ");
	printf("multi : %d |", isMultiMenu);
	printf("host : %d |", isHostButton);
	printf("join : %d |", isJoinButton);
	
	/* Background image */
	displaySprite(renderer, background, 0, 0);

	/* Start button */
	displaySprite(renderer, start_button, 500, 300);

	/* Editor button */
	displaySprite(renderer, map_editor_button, 515, 375);

	/* Multi button */
	displaySprite(renderer, multi_button, 500, 450);

	/* Quit button */
	displaySprite(renderer, quit_button, 515, 525);

	/* Bouton musique ON/OFF */
	if (music_playing){
		displaySprite(renderer, music_on, x-175, y-200);
	}else {
		displaySprite(renderer, music_off, x-175, y-200);
	}
	/* Affiche en gros Tactics Arena */
	displayText(renderer, 300, 200, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255);

	/* Mentions de bas de menu */
	displayText(renderer, 5, y-20, 15, "Projet L2 Informatique - BUTEL CHAUVIN DOUCET LAFAY", "../inc/font/Pixels.ttf", 255, 255, 255);
}

int displayMenu(int x, int y)
// Create a window with with x*y size (in px)
{
    //Le pointeur vers la fenetre
	SDL_Window* pWindow = NULL;
	SDL_Renderer *renderer=NULL;

	// x and y sizes of the window
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
		SDL_StartTextInput();
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

								loadMenuTextures(renderer);
								if(isMultiMenu == 0 ){
									updateMenu(renderer, x, y);
								}
								
								// SDL_RenderPresent(renderer);
								
							break;
						}
					break;
					case SDL_MOUSEBUTTONDOWN:

						printf("X: %d | Y: %d\n", e.motion.x, e.motion.y);		// Debug console pos x & y on term

						// Bouton "Start"
						if (e.motion.x >= 569 && e.motion.x <= 730 && e.motion.y >= 394 && e.motion.y <= 443 && isMultiMenu == 0)
						{
							closeWindow(pWindow);
							freeMenuTextures();
							return 2;
						}

						// Bouton "Map editor"
						else if ((e.motion.x >= 585 && e.motion.x <= 710 && e.motion.y >= 467 && e.motion.y <= 518) && isMultiMenu == 0)
						{
							closeWindow(pWindow);
							freeMenuTextures();
							return 3;
						}

						// Bouton "Multiplayer"
						else if ((e.motion.x >= 585 && e.motion.x <= 710 && e.motion.y >= 540 && e.motion.y <= 600) && (isHostButton == 0 || isJoinButton == 0))
						{
							isMultiMenu = 1;
							printf("Multi button touch : \n");
							printf("multi : %d |", isMultiMenu);
							printf("host : %d |", isHostButton);
							printf("join : %d |", isJoinButton);
							dispMultiMenu(renderer, x, y);
							// SDL_RenderPresent(renderer);
						}

						// Bouton "Host"
						else if ((e.motion.x >= 160 && e.motion.x <= 305 && e.motion.y >= 390 && e.motion.y <= 450) && isMultiMenu == 1)
						{	
							isHostButton = 1;
							dispHostMenu(renderer, x, y);
							printf("Host button touch : \n");
							printf("multi : %d |", isMultiMenu);
							printf("host : %d |", isHostButton);
							printf("join : %d |", isJoinButton);
							//SDL_RenderPresent(renderer);
							// pthread_create (& otherThread.thread_server, NULL, fn_server, NULL);
							
							
						}
						
						// Nouveau boutton "QUIT" 
						else if (e.motion.x >= 1000 && e.motion.x <= 1190 && e.motion.y >= 627 && e.motion.y <= 680 && isHostButton == 1){
							closeWindow(pWindow);
							freeMenuTextures();
						}

						// Bouton "Join"
						else if ((e.motion.x >= 160 && e.motion.x <= 305 && e.motion.y >= 465 && e.motion.y <= 525) && isMultiMenu == 1) 
						{
							isJoinButton = 1;
							dispJoinMenu(renderer, x, y);
							printf("multi : %d", isMultiMenu);
							printf("host : %d", isHostButton);
							printf("join : %d", isJoinButton);
							// SDL_RenderPresent(renderer);
							pthread_create (& otherThread.thread_client, NULL, fn_client, NULL);
						}

						// Switch musique ON/OFF
						else if (e.motion.x >= 1202 && e.motion.x <= 1250 && e.motion.y >= 627 && e.motion.y <= 680)
						{
							if (music_playing)
							{
								music_playing = 0;
								pauseMenuMusic();
								displaySprite(renderer, music_off, x-175, y-200);
								// SDL_RenderPresent(renderer);
							}
							else
							{
								music_playing = 1;
								resumeMenuMusic();
								displaySprite(renderer, music_on, x-175, y-200);
								// SDL_RenderPresent(renderer);
							}
						}
						
						// Bouton "Quit"
						else if (e.motion.x >= 569 && e.motion.x <= 730 && e.motion.y >= 613 && e.motion.y <= 673 && isHostButton == 0)
						{
							closeWindow(pWindow);
							freeMenuTextures();
						}
					break;
					case SDL_KEYDOWN:
						if (e.key.keysym.sym == SDLK_BACKSPACE)
							{
								if (strlen(consoleLog) > 9){
									consoleLog[strlen(consoleLog)-1] = '\0';
									dispHostMenu(renderer, x, y);
								}
							}
					case SDL_TEXTINPUT:
						if (isHostButton == 1)
						{
							strcat(consoleLog, e.text.text);
							printf("%s\n",e.text.text);
							// printf("Host : %d \nMulti : %d \n",isHostButton,isMultiMenu);
							dispHostMenu(renderer, x, y);
							// SDL_RenderPresent(renderer);

						}
					break;
					case SDL_TEXTEDITING:
						if ( isHostButton == 1)
						{
							compo = e.edit.text;
							cursor = e.edit.start;
							selection_len = e.edit.length;
							dispHostMenu(renderer, x, y);
							// SDL_RenderPresent(renderer);
						}
				}
			}
			SDL_Delay(16);
			SDL_RenderPresent(renderer);
		}
		closeWindow(pWindow);
		freeMenuTextures();
	} else {
		fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}

	return 1;
}
