#include <stdio.h>
#include <math.h>
#include <dirent.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "graphics.h"
#include "struct.h"
#include "audio.h"
#include "map.h"

#define _NB_MAX_TEXTURES_ 50
#define _NB_MAX_MAPS_ 20

int isInSaveMenu = 0;
int isInLoadMenu = 0;

// Textures table
TabTexture textures[_NB_MAX_TEXTURES_];

// Map list
char *mapList[_NB_MAX_MAPS_];
int mapListIndex = 0;

// Contains the name of the map the user wants to save
char mapName[50] = "map_";
char *composition;
int mapNameIndex;
Sint32 cursor;
Sint32 selection_len;

int loadEditorTextures(SDL_Renderer * renderer, TabTexture * textures)
// Load all the map related textures
{
	int index;

	printf("[GRAPHICS] Effacement des textures pré-existantes...\n");

	freeTextures(textures);

	printf("[GRAPHICS] Chargement des textures du jeu...\n");

    // Loading blank pattern textures
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/blank_64.png")),
						loadTexture(renderer, loadImage("../inc/img/blank_128.png")),
						"blank");

	// Loading non-selected pattern textures
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/block_64.png")),
						loadTexture(renderer, loadImage("../inc/img/block_128.png")),
						"block");

	// Loading blue selected pattern textures
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/block_blue_64.png")),
						loadTexture(renderer, loadImage("../inc/img/block_blue_128.png")),
						"blue_selected");

	// Loading red selected pattern textures
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/block_red_64.png")),
						loadTexture(renderer, loadImage("../inc/img/block_red_128.png")),
						"red_selected");

	// Loading water block textures
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/water_64.png")),
						loadTexture(renderer, loadImage("../inc/img/water_128.png")),
						"water");

	// Loading sand block textures
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/sand_64.png")),
						loadTexture(renderer, loadImage("../inc/img/sand_128.png")),
						"sand");

	// Loading interface texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/block_interface.png")),
						NULL,
						"interface");

	// Loading eraser texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/eraser_64.png")),
						NULL,
						"eraser");

	// Loading selection hover texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/editor_selection.png")),
						NULL,
						"selection");

	// Loading green button texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/ok_button.png")),
						NULL,
						"ok_button");

	// Loading orange button texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/load_button.png")),
						NULL,
						"load_button");

	// Loading red button texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/cancel_button.png")),
						NULL,
						"cancel_button");

	// Loading blue button texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/fill_button.png")),
						NULL,
						"fill_button");

	// Loading blur effect texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/blur.png")),
						NULL,
						"blur");

	// Loading save menu texture
	index = addTextureToTable(	textures,
								loadTexture(renderer, loadImage("../inc/img/saveMenu.png")),
								NULL,
								"save_menu");

	printf("[GRAPHICS] %d texture(s) chargée(s) !\n", index+1);

	return index+1;
}

int listMaps(char *mapList[])
// List all the maps and save them in mapList
{
	int index = 0;
	DIR * d;
	struct dirent *dir;

	d = opendir("../maps");
	if (!d){
		printf("Aucune map sauvegardée\n");
		return -1;
	}
	else
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
			{
				mapList[index] = dir->d_name;
				index++;
			}
		}
		closedir(d);
	}

	return index;
}

int displayLoadMenu(SDL_Renderer * renderer, char *mapList[], int xWinSize, int yWinSize, int index)
// Display a load map menu and load the selected one
{
	int xPos = (xWinSize/2)-300;
	int yPos = (yWinSize/2)-60;
	int xOpPos = xPos + 600;
	int yOpPos = yPos + 120;
	int nbMaps = listMaps(mapList);

	if (index <= 0) 		index = 0, 			mapListIndex = 0;
	if (index > nbMaps-1)	index = nbMaps-1,	mapListIndex = nbMaps-1;

	char mapName[50];
	sprintf(mapName, "%s", mapList[index]);

	displaySprite(renderer, getTexture(textures, "blur"), 0, 0);
	displaySprite(renderer, getTexture(textures, "save_menu"), xPos, yPos);
	displayText(renderer, xPos+10, yPos+5, 20, "Charger la map...", "../inc/font/Pixels.ttf", 255, 255, 255);
	displayText(renderer, xPos+10, yOpPos-25, 15, "[ENTER] pour valider, [ESC] pour annuler, [ARROWS] pour naviguer", "../inc/font/Pixels.ttf", 255, 255, 255);
	if (mapList[index] != NULL) displayText(renderer, xPos+40, yPos+55, 20, mapName, "../inc/font/Pixels.ttf", 0, 0, 0);
	if (index > 0) 		displayText(renderer, xPos+10, yPos+50, 30, "<", "../inc/font/Pixels.ttf", 255, 255, 255);
	if (index < nbMaps-1) 	displayText(renderer, xOpPos-20, yPos+50, 30, ">", "../inc/font/Pixels.ttf", 255, 255, 255);

	return 1;
}

int displaySaveMenu(SDL_Renderer * renderer, Tile * grid, int xWinSize, int yWinSize, char * mapName, int save)
// Display a save map menu and save the map
{
	int xPos = (xWinSize/2)-300;
	int yPos = (yWinSize/2)-60;
	int yOpPos = yPos + 120;

	displaySprite(renderer, getTexture(textures, "blur"), 0, 0);
	displaySprite(renderer, getTexture(textures, "save_menu"), xPos, yPos);
	displayText(renderer, xPos+10, yPos+5, 20, "Enregistrer la map...", "../inc/font/Pixels.ttf", 255, 255, 255);
	displayText(renderer, xPos+10, yOpPos-25, 20, "[ENTER] pour valider, [ESC] pour annuler", "../inc/font/Pixels.ttf", 255, 255, 255);
	if (mapName != NULL) displayText(renderer, xPos+40, yPos+55, 20, mapName, "../inc/font/Pixels.ttf", 0, 0, 0);

	return 1;
}

int saveMap(Tile * grid, const char * name)
// Save a map
{
	char mapName[20];

	sprintf(mapName, "../maps/%s", name);

	FILE * map;
	map = fopen(mapName, "wb");
	fwrite(grid, sizeof(Tile)*10*10, 1, map);

	fclose(map);

	return 1;
}

int loadMap(Tile * grid, const char * name)
// Load a map
{
	char mapName[20];

	sprintf(mapName, "../maps/%s", name);
	printf("[EDITOR] Chargement de la map %s...\n", mapName);

	FILE * map;
	map = fopen(mapName, "rb");
	fread(grid, sizeof(Tile)*10*10, 1, map);

	fclose(map);

	return 1;
}

int changeTile(Tile * grid, int xpos, int ypos, int mx, int my, int pxBase, int xSize, int ySize, int toTile)
// Set the tile selected according to 2D iso from 2D coordinates
{
	int xIndex, yIndex, xIsoOrigin, yIsoOrigin, xTile, yTile;
	float cpAB, cpBC, cpDC, cpAD;

	// On déselectionne toutes les cases
	for (int i=0; i<xSize; i++){
		for (int j=0; j<ySize; j++){
			(*(grid+i*xSize+j)).selected = 0;
		}
	}

	// Position de l'origine de la map en 2D isométrique
	xIsoOrigin = xpos;
	yIsoOrigin = ypos+ySize*(pxBase/4);

	// Coordonnées 2D -> 2D iso
	xIndex = floor(((my-yIsoOrigin)/(pxBase/2) + ((mx-xIsoOrigin)/pxBase)))-1;
	yIndex = ceil((((mx-xIsoOrigin)/pxBase) - (my-yIsoOrigin)/(pxBase/2)))-1;

	if (my < yIsoOrigin){
		xIndex--;
		yIndex++;
	}

	xTile = xpos+((((xIndex+yIndex)/2)+1)*pxBase);
	yTile = ypos+((ySize-(yIndex-xIndex))*(pxBase/4)+(pxBase/4));

	// Calcul des coordonnées des 4 coins de la tile
	Coord A = { xTile, yTile };
	Coord B = { xTile + (pxBase / 2), yTile - (pxBase / 2) };
	Coord C = { xTile + pxBase, yTile };
	Coord D = { xTile + (pxBase / 2), yTile + (pxBase / 4) };

	// Calcul des coordonnées des vecteurs de la tile
	Vector AB = { B.x - A.x, B.y - A.y };
	Vector AM = { mx - A.x, my - A.y };
	Vector BC = { C.x - B.x, C.y - B.y };
	Vector BM = { mx - B.x, my - B.y };
	Vector DC = { C.x - D.x, C.y - D.y };
	Vector DM = { mx - D.x, my - D.y };
	Vector AD = { D.x - A.x, D.y - A.y };
	Vector A2M = { mx - A.x, my - A.y };
	cpAB = crossProduct(AB, AM);
	cpBC = crossProduct(BC, BM);
	cpDC = crossProduct(DC, DM);
	cpAD = crossProduct(AD, A2M);

	// Sélection de la case sélectionnée en fonction de la position relative du clic et des vecteurs
	if (cpAB > 0){
		xIndex--;
	} else if (cpBC > 0){
		yIndex++;
	} else if (cpDC < 0){
		xIndex++;
	} else if (cpAD < 0){
		yIndex--;
	}

	if (xIndex > xSize-1 || yIndex > ySize-1 || xIndex < 0 || yIndex < 0) return 0;

	// DEBUG printf("[GRAPHICS] Case sélectionnée : %d, %d\n", xIndex, yIndex);
	(*(grid+xIndex*xSize+yIndex)).tile_id = toTile;

	return 1;
}

void fillMap(Tile * grid, int block_id, int xSize, int ySize)
// Fill the map with the id_block block
{
	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
		{
			(*(grid+i*xSize+j)).tile_id = block_id;
		}
	}
	printf("[EDITOR] Map remplie avec le bloc [%s] id %d\n", textures[block_id].texture_name, block_id);
}

int displayEditorMap(SDL_Renderer *renderer, int x, int y, int pxBase, Tile * grid, int xSize, int ySize, int select, int xWinSize, int yWinSize)
// Display the map
{
	Coord selectionPos, blockPos, buttonPos;

	/* Le fond de la fenêtre sera blanc */
    SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
	SDL_RenderClear(renderer);

    for (int i=0; i < xSize; i++){
        for (int j=(ySize-1); j >= 0; j--){

			blockPos.x = x+(j+1)*(pxBase/2)+(i+1)*(pxBase/2);
			blockPos.y = y+i*(pxBase/4)+(ySize-j)*(pxBase/4);

			if ((*(grid+i*xSize+j)).tile_id == 1)
			{
				if ((*(grid+i*xSize+j)).selected)
				{
					if (pxBase == 64)	displaySprite(renderer, getTexture(textures, "blue_selected"), blockPos.x, blockPos.y);
					else				displaySprite(renderer, getBigTexture(textures, "blue_selected"), blockPos.x, blockPos.y);
				} 
				else
				{
					if (pxBase == 64)	displaySprite(renderer, getTexture(textures, "block"), blockPos.x, blockPos.y);
					else				displaySprite(renderer, getBigTexture(textures, "block"), blockPos.x, blockPos.y);
				}
			}
			else if ((*(grid+i*xSize+j)).tile_id <= 5)
			{
				if (pxBase == 64)	displaySprite(renderer, textures[(*(grid+i*xSize+j)).tile_id].texture, blockPos.x, blockPos.y);
				else 				displaySprite(renderer, textures[(*(grid+i*xSize+j)).tile_id].big_texture, blockPos.x, blockPos.y);
			}

			/*/ -- DEBUG Affichage des indices des tuiles --
			char pos[6];
			sprintf(pos, "%d,%d", i, j);
			displayText(renderer, imgDestRect.x+(pxBase/2)-10, imgDestRect.y+(pxBase/4), (pxBase/64)*10, pos, "../inc/font/Pixels.ttf", 255, 50, 50);
			// -- DEBUG --*/
        }
    }

	displaySprite(renderer, getTexture(textures, "interface"), 0, 0);
	displaySprite(renderer, getTexture(textures, "eraser"), 10, 50);

	for (int i = 1; i <= 5; i++)
	{
		if (i%2 == 0)		buttonPos.x = 10;
		else				buttonPos.x = 126;
		buttonPos.y = ceil(i/2)*74+50;
		displaySprite(renderer, textures[i].texture, buttonPos.x, buttonPos.y);
	}

	if (select%2 == 0)		selectionPos.x = 10;
	else					selectionPos.x = 126;
	selectionPos.y = ceil(select/2)*74+50;
	displaySprite(renderer, getTexture(textures, "selection"), selectionPos.x, selectionPos.y);

	// Boutons
	displaySprite(renderer, getTexture(textures, "fill_button"), 0, yWinSize-160);
	displayText(renderer, 10, yWinSize-150, 20, "REMPLIR", "../inc/font/Pixels.ttf", 255, 255, 255);
	displaySprite(renderer, getTexture(textures, "cancel_button"), 0, yWinSize-120);
	displayText(renderer, 10, yWinSize-110, 20, "QUITTER", "../inc/font/Pixels.ttf", 255, 255, 255);
	displaySprite(renderer, getTexture(textures, "load_button"), 0, yWinSize-80);
	displayText(renderer, 10, yWinSize-70, 20, "OUVRIR UNE MAP", "../inc/font/Pixels.ttf", 255, 255, 255);
	displaySprite(renderer, getTexture(textures, "ok_button"), 0, yWinSize-40);
	displayText(renderer, 10, yWinSize-30, 20, "ENREGISTRER", "../inc/font/Pixels.ttf", 255, 255, 255);

	displayText(renderer, 20, 20, 20, "----- BLOCS -----", "../inc/font/Pixels.ttf", 255, 255, 255);

	// -- DEBUG Affichage des coordonnées d'affichage de la map
	char str[12];
	sprintf(str, "%d, %d", x, y);
	displayText(renderer, 220, 20, 20, str, "../inc/font/Pixels.ttf", 255, 255, 255);
	// -- DEBUG --

	if (isInSaveMenu == 1) displaySaveMenu(renderer, grid, xWinSize, yWinSize, mapName, 1);
	if (isInLoadMenu == 1) displayLoadMenu(renderer, mapList, xWinSize, yWinSize, mapListIndex);

	SDL_RenderPresent(renderer);

	return 1;
}

int createMapEditorWindow(int x, int y, Tile * grid, int xSize, int ySize)
// Create a window with with x*y size (in px)
{
    // Le pointeur vers la fenetre
	SDL_Window* pWindow = NULL;
	SDL_Renderer *renderer=NULL;

	// Resolution of a bloc texture (can be 64 or 128)
	int PX = 64;

	// x and y pos where map is displayed
	int XPOS = 400;
	int YPOS = 170;

	// x and y sizes of the window
	int xWinSize;
	int yWinSize;

	// Block selected
	int SELECT = 1;

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
	pWindow = SDL_CreateWindow("Tactics Arena - Editeur de map", 	SDL_WINDOWPOS_UNDEFINED,
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

		if (loadEditorTextures(renderer, textures) == 0){
			printf("Erreur lors du chargement des textures (0 chargée)\n");
			return 0;
		}

		SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);

		/* Le fond de la fenêtre sera blanc */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_Delay(1);

		displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);

		SDL_RenderPresent(renderer);

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

								SDL_GetWindowSize(pWindow, &xWinSize, &yWinSize);

								/* Le fond de la fenêtre sera blanc */
                				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
								SDL_RenderClear(renderer);

								SDL_Delay(1);

								displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);

								SDL_RenderPresent(renderer);

							break;
						}
					break;
					case SDL_MOUSEBUTTONDOWN:

						//printf("X: %d | Y: %d\n", e.motion.x, e.motion.y);		// Debug console pos x & y on term
						if (e.motion.x <= 200 && isInSaveMenu == 0 && isInLoadMenu == 0)
						{
							if (e.motion.x >= 10 && e.motion.x <= 74 && e.motion.y >= 50 && e.motion.y <= 114)							SELECT = 0;
							else if (e.motion.x >= 126 && e.motion.x <= 190 && e.motion.y >= 50 && e.motion.y <= 114)					SELECT = 1;
							else if (e.motion.x >= 10 && e.motion.x <= 74 && e.motion.y >= 124 && e.motion.y <= 188)					SELECT = 2;
							else if (e.motion.x >= 126 && e.motion.x <= 190 && e.motion.y >= 124 && e.motion.y <= 188)					SELECT = 3;
							else if (e.motion.y >= 10 && e.motion.x <= 74 && e.motion.y >= 198 && e.motion.y <= 262)					SELECT = 4;
							else if (e.motion.y >= 126 && e.motion.x <= 190 && e.motion.y >= 198 && e.motion.y <= 262)					SELECT = 5;
							// Sauvegarder
							else if (e.motion.y >= yWinSize-40 && e.motion.y <= yWinSize) isInSaveMenu = 1;
							// Charger une map
							else if (e.motion.y >= yWinSize-80 && e.motion.y <= yWinSize-40) isInLoadMenu = 1;
							// Quitter
							else if (e.motion.y >= yWinSize-120 && e.motion.y <= yWinSize-80)
							{
								closeWindow(pWindow);
								freeTextures(textures);
							}
							// Remplir
							else if (e.motion.y >= yWinSize-160 && e.motion.y <= yWinSize-120)
							{
								fillMap(grid, SELECT, xSize, ySize);
							}
						}
						else
						{
							if (!isInSaveMenu) changeTile(grid, XPOS, YPOS, e.motion.x, e.motion.y, PX, xSize, ySize, SELECT);
						}

						displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);

					break;
					case SDL_MOUSEWHEEL:
						if (e.wheel.y > 0)		// Scroll UP
						{
							if (PX == 64){
								PX = 128;
								printf("[GRAPHICS] Zoom In\n");
								XPOS *= 2;
								YPOS *= 2;
								displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
							}
						} else {				// Scroll DOWN
							if (PX == 128){
								PX = 64;
								printf("[GRAPHICS] Zoom Out\n");
								XPOS /= 2;
								YPOS /= 2;
								displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
							}
						}
					break;
					case SDL_KEYDOWN:
						if (isInSaveMenu == 0 && isInLoadMenu == 0){
							switch(e.key.keysym.sym)
							{
								case SDLK_KP_PLUS: 	// "+" key
									if (PX == 64){
										PX = 128;
										printf("[GRAPHICS] Zoom In\n");
										XPOS *= 2;
										YPOS *= 2;
										displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
									}
									break;
								case SDLK_KP_MINUS:	// "-" key
									if (PX == 128){
										PX = 64;
										printf("[GRAPHICS] Zoom Out\n");
										XPOS /= 2;
										YPOS /= 2;
										displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
									}
									break;
								case SDLK_z:		// "z" key
									YPOS += (10*(PX/64));
									displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
									break;
								case SDLK_q:		// "q" key
									XPOS += (10*(PX/64));
									displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
									break;
								case SDLK_s:		// "s" key
									YPOS -= (10*(PX/64));
									displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
									break;
								case SDLK_d:		// "d" key
									XPOS -= (10*(PX/64));
									displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
									break;
							}
						}
						else
						{
							if (e.key.keysym.sym == SDLK_BACKSPACE)
							{
								if (strlen(mapName) > 4){
									mapName[strlen(mapName)-1] = '\0';
									displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
								}
							}
							else if (e.key.keysym.sym == SDLK_RETURN)
							{
								if (isInSaveMenu == 1) saveMap(grid, mapName);
								if (isInLoadMenu == 1)
								{
									listMaps(mapList);
									char mapSelected[50];
									sprintf(mapSelected, "%s", mapList[mapListIndex]);
									loadMap(grid, mapSelected);
								}
								isInSaveMenu = 0;
								isInLoadMenu = 0;
								displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
							}
							else if (e.key.keysym.sym == SDLK_ESCAPE)
							{
								isInSaveMenu = 0;
								isInLoadMenu = 0;
								displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
							}
							else if (e.key.keysym.sym == SDLK_LEFT)
							{
								if (isInLoadMenu == 1)
								{
									mapListIndex--;
									displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
								}
							}
							else if (e.key.keysym.sym == SDLK_RIGHT)
							{
								if (isInLoadMenu == 1)
								{
									mapListIndex++;
									displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
								}
							}
						}
						
					break;
					case SDL_TEXTINPUT:
						if (isInSaveMenu == 1)
						{
							strcat(mapName, e.text.text);
							displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
						}
					break;
					case SDL_TEXTEDITING:
						if (isInSaveMenu == 1)
						{
							composition = e.edit.text;
							cursor = e.edit.start;
							selection_len = e.edit.length;
							displayEditorMap(renderer, XPOS, YPOS, PX, grid, xSize, ySize, SELECT,  xWinSize, yWinSize);
						}
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
		freeTextures(textures);
	} else {
		fprintf(stderr,"[GRAPHICS] Erreur de création de la fenêtre: %s\n",SDL_GetError());
	}

	return 1;
}