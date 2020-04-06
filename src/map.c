#include <stdio.h>
#include <math.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "audio.h"
#include "graphics.h"
#include "characters.h"
#include "common.h"
#include "grid.h"

#define _NB_MAX_TEXTURES_ 50

// Textures table
TabTexture textures[_NB_MAX_TEXTURES_];

int loadMapTextures(SDL_Renderer * renderer)
// Load all the map related textures
{
	int index;

	if(verbose)printf("[GRAPHICS] Effacement des textures pré-existantes...\n");

	freeTextures(textures);

	if(verbose)printf("[GRAPHICS] Chargement des textures du jeu...\n");

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

	// Loading ice block textures
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/ice_64.png")),
						loadTexture(renderer, loadImage("../inc/img/ice_128.png")),
								"ice");

	// Loading snow block textures
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/block_snow_64.png")),
						loadTexture(renderer, loadImage("../inc/img/block_snow_128.png")),
						"snow");

	// Loading sand block textures
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/selection_64.png")),
						loadTexture(renderer, loadImage("../inc/img/selection_128.png")),
						"selection");

	// Loading attack logo
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/attack_logo_64.png")),
						NULL,
						"attack");

	// Loading sand block textures
	index = addTextureToTable(	textures,
								loadTexture(renderer, loadImage("../inc/img/turn_end_grey.png")),
								NULL,
								"end_turn");

	if(verbose)printf("[GRAPHICS] %d texture(s) chargée(s) !\n", index+1);

	return index+1;
}

float crossProduct(Vector AB, Vector AC)
// Renvoie le produit vectoriel
{
	return (( AB.y * AC.x ) - ( AB.x * AC.y ));
}

int selectTile(Tile * grid, int xpos, int ypos, int mx, int my, int pxBase, int xSize, int ySize)
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
	if(verbose)printf("xTile : %d yTile : %d\n", xTile, yTile);

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

	if(verbose)printf("[GRAPHICS] Case sélectionnée : %d, %d\n", xIndex, yIndex);
	(*(grid+xIndex*xSize+yIndex)).selected = 1;
	Coord selectedTile = {xIndex, yIndex};
	Entity *selectedEntity = getEntity(grid, selectedTile);
	if (selectedEntity != NULL)
	{
		action act = {selectedEntity->cha_id, selectedTile, selected_ability};
		if (selected_ability != -1)
		{
			if (selected_ability == 0) apply_action(act);
		}
	}

	return 1;
}

int displayAbilities(SDL_Renderer *renderer)
// Display the abilities menu
{
	// Abilities icons
	displaySprite(renderer, getTexture(textures, "attack"), 16, yWinSize-80);
	displaySprite(renderer, getTexture(textures, "attack"), 16+1*80, yWinSize-80);
	displaySprite(renderer, getTexture(textures, "attack"), 16+2*80, yWinSize-80);
	displaySprite(renderer, getTexture(textures, "attack"), 16+3*80, yWinSize-80);
	displaySprite(renderer, getTexture(textures, "attack"), 16+4*80, yWinSize-80);

	return 0;
}

int displayInterface(SDL_Renderer *renderer)
// Display the UI
{
	Entity * tempEntity = getEntity(matrix, getSelectedPos());
	if (tempEntity != NULL)
	{
		displayAbilities(renderer);
		if (selected_ability != 0){
			displayText(renderer, 16, yWinSize-110, 20, description, "../inc/font/Pixels.ttf", 255, 255, 255);
		} else {
			if (hover_ability == 10) displayText(renderer, xWinSize-280, yWinSize-110, 20, "Passer son tour", "../inc/font/Pixels.ttf", 255, 255, 255);
			else if (hover_ability > 0) displayText(renderer, 16, yWinSize-110, 20, "Description capacite " + (char)hover_ability, "../inc/font/Pixels.ttf", 255, 255, 255);
		}
	}

	// Turn end icon
	displaySprite(renderer, getTexture(textures, "end_turn"), xWinSize-280, yWinSize-80);

	return 0;
}

int displayMap(SDL_Renderer *renderer, int x, int y, int pxBase, Tile * grid, int xSize, int ySize, TabTexture * cSprites)
// Display the map
{
	/* Le fond de la fenêtre sera blanc */
    SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
	SDL_RenderClear(renderer);

    Coord blockPos;

	/* Le fond de la fenêtre sera blanc */
    SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
	SDL_RenderClear(renderer);

    for (int i=0; i < xSize; i++){
        for (int j=(ySize-1); j >= 0; j--){

			blockPos.x = x+(j+1)*(pxBase/2)+(i+1)*(pxBase/2);
			blockPos.y = y+i*(pxBase/4)+(ySize-j)*(pxBase/4);

			if ((*(grid+i*xSize+j)).tile_id == 1)
			{
				if (pxBase == 64)	displaySprite(renderer, getTexture(textures, "blue_selected"), blockPos.x, blockPos.y);
				else				displaySprite(renderer, getBigTexture(textures, "blue_selected"), blockPos.x, blockPos.y);
				if (pxBase == 64)	displaySprite(renderer, getTexture(textures, "block"), blockPos.x, blockPos.y);
				else				displaySprite(renderer, getBigTexture(textures, "block"), blockPos.x, blockPos.y);
			}
			else if ((*(grid+i*xSize+j)).tile_id <= 7)
			{
				if (pxBase == 64)	displaySprite(renderer, textures[(*(grid+i*xSize+j)).tile_id].texture, blockPos.x, blockPos.y);
				else 				displaySprite(renderer, textures[(*(grid+i*xSize+j)).tile_id].big_texture, blockPos.x, blockPos.y);
			}

			// Affichage tuile de sélection
			if ((*(grid+i*xSize+j)).selected == 1)
			{
				if (pxBase == 64)	displaySprite(renderer, getTexture(textures, "selection"), blockPos.x, blockPos.y);
				else				displaySprite(renderer, getBigTexture(textures, "selection"), blockPos.x, blockPos.y);
			}

			if ((*(grid+i*xSize+j)).entity != NULL)	displayCharacters(renderer, cSprites, (*(grid+i*xSize+j)).entity, blockPos.x, blockPos.y-pxBase/1.6, pxBase);

			/*/ -- DEBUG Affichage des indices des tuiles --
			char pos[6];
			sprintf(pos, "%d,%d", i, j);
			displayText(renderer, imgDestRect.x+(pxBase/2)-10, imgDestRect.y+(pxBase/4), (pxBase/64)*10, pos, "../inc/font/Pixels.ttf", 255, 50, 50);
			// -- DEBUG --*/
        }
    }

	displayInterface(renderer);

	// -- DEBUG Affichage des coordonnées d'affichage de la map
	char str[12];
	sprintf(str, "%d, %d", x, y);
	displayText(renderer, 20, 20, 20, str, "../inc/font/Pixels.ttf", 255, 255, 255);
	// -- DEBUG --

	SDL_RenderPresent(renderer);

	return 1;
}
