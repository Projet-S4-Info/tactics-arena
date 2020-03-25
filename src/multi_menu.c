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
			*host_button = NULL,
			*ok_button_Multi = NULL;

int isHostMenu = 0;
int isJoinMenu = 0;
int inputPseudoBtn = 0;
int inputIpBtn = 0;
int isPseudoValid = 0;
int isIPValid = 0;
int isInfoJoinSet = 0;
int music_Multi_playing = 1;

char pseudoSrv[50] = "Pseudo : ";
char pseudoJoin[50] = "Pseudo : ";
char pseudoUser[50];
char ipSrv[85];
char ipJoin[90] = "IP : ";
char *compo;

extern Sint32 cursor;
extern Sint32 selection_len;


char *tabLog[] = {
		"Création du serveur",
		"Le serveur est initialisé !",
		"En attente du client ...",
		"Une connexion a été établie !",
		" ",
};

// Initialisation du thread 
typedef struct                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
{
   pthread_t thread_server;
   pthread_t thread_client;
}
multiThread_t;

multiThread_t threadServ;
multiThread_t threadCli;

 
/*-- Fonction pour le thread du serveur. --*/
static void * fn_server (void * p_data)
{
    startTCPSocketServ();
    return NULL;
}

/*-- Fonction pour le thread du client --*/
static void * fn_client (void * p_data)
{
    startTCPSocketCli();
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


	// == ok switch ==
	ok_button_Multi = loadTexture(renderer, loadImage("../inc/img/Okey_button.png"));

}

void freeMultiMenuTextures()
// Free all the textures loaded
{
	SDL_DestroyTexture(background_Multi);
	SDL_DestroyTexture(quit_button_Multi);
	SDL_DestroyTexture(music_on_Multi);
	SDL_DestroyTexture(music_off_Multi);
	SDL_DestroyTexture(host_button);
	SDL_DestroyTexture(join_button);
	SDL_DestroyTexture(ok_button_Multi);
}

void dispLog(SDL_Renderer *renderer, int consoleX, int consoleY){

	
	for(int i = 0; i < logFlag; i++ ){
		displayText(renderer, consoleX + 20, consoleY + (25 * i) , 22, tabLog[i], "../inc/font/PixelOperator.ttf", 255, 255, 255);
		if(i == 4){
			displayText(renderer, consoleX + 20, consoleY + (25 * i) , 22, pseudoCli, "../inc/font/PixelOperator.ttf", 255, 255, 255);
		}
	}
}


void dispHostMenu(SDL_Renderer *renderer, int x, int y){

	SDL_Rect console;
			console.x = x-(40+450);
			console.y = 350;
			console.w = 450;
			console.h = 250;
	
	SDL_Rect infoHost;
			infoHost.x = 500;
			infoHost.y = 350;
			infoHost.w = 250;
			infoHost.h = 75;
	
	SDL_Rect inputSrv;
			inputSrv.x = 40;
			inputSrv.y = 350;
			inputSrv.w = 300;
			inputSrv.h = 150;
	
	SDL_Rect pseudoHostBox;
			pseudoHostBox.x = 150;
			pseudoHostBox.y = 400;
			pseudoHostBox.w = 150;
			pseudoHostBox.h = 28;
	

	
    /* Background image */
	displaySprite(renderer, background_Multi, 0, 0);
    /* Affiche en gros Tactics Arena */
	displayText(renderer, 300, 100, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255);
    /* Quit button */
    displaySprite(renderer, quit_button_Multi, x-300, y-190);
    /* initialisation de l'opacitée */
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    /* initialise la couleur sur noir */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    /*remplis le rectange*/
    SDL_RenderFillRect(renderer, &console);
	/* ré atribution des valeurs par défault */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    // displayText(renderer, 50, 400, 15, "Host est séléctioné", "../inc/font/PixelOperator.ttf", 255, 255, 255);
    
	/*-------------------input box Host menu -------------------------*/
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 85, 34, 0, 185);
	SDL_RenderFillRect(renderer, &inputSrv);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 185);
	SDL_RenderFillRect(renderer, &pseudoHostBox);
	displayText(renderer, inputSrv.x + 10, inputSrv.y + 15 , 22, "Saisir votre Pseudo : ", "../inc/font/Pixels.ttf", 255, 255, 255);
	displayText(renderer, inputSrv.x + 15, inputSrv.y + 50, 22, pseudoSrv, "../inc/font/PixelOperator.ttf", 255, 255, 255);
	displaySprite(renderer, ok_button_Multi, 50, 340);
	displayText(renderer, 150, 430, 55, "OK", "../inc/font/PixelOperator.ttf", 255, 255, 255);



	/*-------------------input box Host menu -------------------------*/
	

	

	if(logFlag >= 3) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 85, 34, 0, 185);
		SDL_RenderFillRect(renderer, &infoHost);
		displayText(renderer, infoHost.x + 10, infoHost.y + 15 , 22, "L'ip du serveur est :", "../inc/font/Pixels.ttf", 255, 255, 255);
		displayText(renderer, infoHost.x + 15, infoHost.y + 50, 22, monIP, "../inc/font/PixelOperator.ttf", 255, 255, 255);
	}
	
	dispLog(renderer, console.x, console.y);
	

}


void dispJoinMenu(SDL_Renderer *renderer, int x, int y)
{
	SDL_Rect infoJoin;
			infoJoin.x = 450;
			infoJoin.y = 350;
			infoJoin.w = 475;
			infoJoin.h = 170;
	
	SDL_Rect ipJoinBox;
			ipJoinBox.x = 515;
			ipJoinBox.y = 397;
			ipJoinBox.w = 350;
			ipJoinBox.h = 28;

	SDL_Rect inputJoin;
			inputJoin.x = 40;
			inputJoin.y = 350;
			inputJoin.w = 300;
			inputJoin.h = 150;
	
	SDL_Rect pseudoJoinBox;
			pseudoJoinBox.x = 150;
			pseudoJoinBox.y = 400;
			pseudoJoinBox.w = 150;
			pseudoJoinBox.h = 28;

	 /* Background image */
	displaySprite(renderer, background_Multi, 0, 0);
    /* Affiche en gros Tactics Arena */
	displayText(renderer, 300, 100, 100, "Tactics Arena", "../inc/font/Blox2.ttf", 255, 255, 255);
    /* Quit button */
    displaySprite(renderer, quit_button_Multi, x-300, y-190);

	/*-----------input box for JoinMenu--------*/
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 85, 34, 0, 185);
	SDL_RenderFillRect(renderer, &inputJoin);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 185);
	SDL_RenderFillRect(renderer, &pseudoJoinBox);
	displayText(renderer, inputJoin.x + 10, inputJoin.y + 15 , 22, "Saisir votre Pseudo : ", "../inc/font/Pixels.ttf", 255, 255, 255);
	displayText(renderer, inputJoin.x + 15, inputJoin.y + 50, 22, pseudoJoin, "../inc/font/PixelOperator.ttf", 255, 255, 255);
	displaySprite(renderer, ok_button_Multi, 50, 340);
	displayText(renderer, 140, 430, 55, "OK", "../inc/font/PixelOperator.ttf", 255, 255, 255);
	/*-----------------------------------------*/

	/*-----------info box for JoinMenu---------*/
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 85, 34, 0, 185);
	SDL_RenderFillRect(renderer, &infoJoin);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 185);
	SDL_RenderFillRect(renderer, &ipJoinBox);
	displayText(renderer, infoJoin.x + 10, infoJoin.y + 15 , 22, "Saisir l'ip du serveur : ", "../inc/font/Pixels.ttf", 255, 255, 255);
	displayText(renderer, infoJoin.x + 15, infoJoin.y + 50, 22, ipJoin, "../inc/font/PixelOperator.ttf", 255, 255, 255);
	displaySprite(renderer, ok_button_Multi, 480, 360);
	displayText(renderer, 590, 450, 55, "OK", "../inc/font/PixelOperator.ttf", 255, 255, 255);
	if((isPseudoValid == 1) && (isIPValid == 1)){
		displaySprite(renderer, ok_button_Multi, 530, 450);
		displayText(renderer, 605, 540, 55, "JOIN", "../inc/font/PixelOperator.ttf", 255, 255, 255);
	}
	/*-----------------------------------------*/

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

						printf("\nX: %d | Y: %d\n", u.motion.x, u.motion.y);	// Debug console pos x & y on term

						// Bouton "Host"
						if (u.motion.x >= 576 && u.motion.x <= 723 && u.motion.y >= 449 && u.motion.y <= 488 && isHostMenu == 0 && isJoinMenu == 0)
						{
							isHostMenu = 1;
                            printf("Host cliqué :) \n");
                            pthread_create (& threadServ.thread_server, NULL, fn_server, NULL);
                            dispHostMenu(renderer, x, y);
						}

						// Bouton "Join"
						else if (u.motion.x >= 575 && u.motion.x <= 724 && u.motion.y >= 513 && u.motion.y <= 554 && isHostMenu == 0 && isJoinMenu == 0)
						{
							isJoinMenu = 1;
                            printf("Join cliqué :) \n");
							
							dispJoinMenu(renderer, x, y);
						}

						// PseudoBox
						else if (u.motion.x >= 150 && u.motion.x <= 305 && u.motion.y >= 399 && u.motion.y <= 432 && (isJoinMenu == 1 || isHostMenu == 1))
						{	
							inputPseudoBtn = 1;
							inputIpBtn = 0;
						}
						else if (u.motion.x >= 127 && u.motion.x <= 241 && u.motion.y >= 441 && u.motion.y <= 479 && (isJoinMenu == 1 || isHostMenu == 1) && isPseudoValid == 0){
							
							isPseudoValid = 1;
							
							char pseuTemp[50];
							if(isHostMenu == 1){
								strcpy(pseuTemp,pseudoSrv);
							}else if(isJoinMenu == 1){
								strcpy(pseuTemp,pseudoJoin);
							}	
							//printf("\npseudo temp : %s\n",pseuTemp);
							for(int i = 0; i < (strlen(pseuTemp)); i++){
								pseudoUser[i-9] = pseuTemp[i];
								//printf("\nps | i : %d | char %c \n", i, pseuTemp[i]);
							}
							printf("\nTest User pseudo : %s\n",pseudoUser);
						}
						
						// IPBox
						else if (u.motion.x >= 520 && u.motion.x <= 868 && u.motion.y >= 399 && u.motion.y <= 429 && isJoinMenu == 1)
						{
							inputIpBtn = 1;
							inputPseudoBtn = 0;
						}
						else if (u.motion.x >= 556 && u.motion.x <= 668 && u.motion.y >= 461 && u.motion.y <= 500 && isJoinMenu == 1 && isIPValid == 0){
							
							isIPValid = 1;
							char ipTemp[85];
							
							strcpy(ipTemp,ipJoin);
							
							//printf("\nip temp : %s\n",ipTemp);
							for(int i = 5; i < (strlen(ipTemp)); i++){
								ipSrv[i-5] = ipTemp[i];
								//printf("\nipTemp[%d] | char %c \n", i, ipTemp[i]);
							}
							printf("\nTest IP Join : %s\n",ipSrv);
						}
						else if (u.motion.x >= 608 && u.motion.x <= 721 && u.motion.y >= 550 && u.motion.y <= 590 && isJoinMenu == 1 && isIPValid == 1 && isPseudoValid == 1){
							printf("JOIN cliqué ! \n");
							pthread_create(&threadCli.thread_client, NULL, fn_client, NULL);
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
						
						//
						else{
							inputIpBtn = 0;
							inputPseudoBtn = 0;

						}	
					break;
					case SDL_KEYDOWN:
						if (u.key.keysym.sym == SDLK_BACKSPACE)
							{
								if(inputPseudoBtn == 1 && isHostMenu == 1){
									if (strlen(pseudoSrv) > 6){
										pseudoSrv[strlen(pseudoSrv)-1] = '\0';
										dispHostMenu(renderer, x, y);
									}
								}else if(inputPseudoBtn == 1 && isJoinMenu == 1){
									if (strlen(pseudoJoin) > 6){
										pseudoJoin[strlen(pseudoJoin)-1] = '\0';
										dispJoinMenu(renderer, x, y);
									}
								}else if(inputIpBtn == 1 && isJoinMenu == 1){
									if(strlen(ipJoin) > 5){
										ipJoin[strlen(ipJoin) - 1] = '\0';
										dispJoinMenu(renderer, x, y);
									}
								}
								
							}
                    break;
					case SDL_TEXTINPUT:
						if ((isHostMenu == 1) && (inputPseudoBtn == 1))
						{
							strcat(pseudoSrv,u.text.text);
							// printf("%s\n",u.text.text);
                            // printf("hostmenu = %d", isHostMenu);
							// printf("Host : %d \nMulti : %d \n",isHostButton,isMultiMenu);
							dispHostMenu(renderer, x, y);
							// SDL_RenderPresent(renderer);
						}else if((isJoinMenu == 1) && (inputPseudoBtn == 1)){
							strcat(pseudoJoin, u.text.text);
							dispJoinMenu(renderer, x, y);
						}else if ((isJoinMenu == 1) && (inputIpBtn == 1)){
							strcat(ipJoin,u.text.text);
							dispJoinMenu(renderer, x, y);
						}
					break;
					case SDL_TEXTEDITING:
						if ( isHostMenu == 1){
							compo = u.edit.text;
							cursor = u.edit.start;
							selection_len = u.edit.length;
							dispHostMenu(renderer, x, y);
							// SDL_RenderPresent(renderer);
					    }else if ( isJoinMenu == 1){
							compo = u.edit.text;
							cursor = u.edit.start;
							selection_len = u.edit.length;
							dispJoinMenu(renderer, x, y);
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
