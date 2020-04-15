
/* =============== DEPENDENCES =============== */


#include <stdio.h>
#include <math.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "audio.h"
#include "game_window.h"
#include "characters.h"
#include "common.h"
#include "grid.h"
#include "gameplay.h"
#include "turn.h"
#include "display.h"
#include "textures.h"


/* =============== CONSTANTES =============== */


#define _NB_MAX_TEXTURES_ 50



/* =============== VARIABLES =============== */


char chatTab[STR_LONG][_NB_MAX_ChAT_];


/* =============== FONCTIONS =============== */

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

	// Loading arrow right texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/arrow_right_64.png")),
						NULL,
						"arrow_right");

	// Loading arrow left texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/arrow_left_64.png")),
						NULL,
						"arrow_left");

	// Loading arrow up texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/arrow_up_64.png")),
						NULL,
						"arrow_up");
					
	// Loading arrow down texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/arrow_down_64.png")),
						NULL,
						"arrow_down");

	// Red team texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/red_team_64.png")),
						loadTexture(renderer, loadImage("../inc/img/red_team_128.png")),
						"red_team");

	// Blue team texture
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/blue_team_64.png")),
						loadTexture(renderer, loadImage("../inc/img/blue_team_128.png")),
						"blue_team");

	// Loading attack logo
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/attack_logo_64.png")),
						NULL,
						"attack");

	// Clockwise turn icon
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/clockwise_icon_64.png")),
						NULL,
						"turn_right");
					
	// Anti-clockwise turn icon
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/anti_clockwise_icon_64.png")),
						NULL,
						"turn_left");

	// Loading end of turn button
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/turn_end_grey.png")),
						NULL,
						"end_turn");

	// Loading end of turn button (hover)
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/turn_end_hover.png")),
						NULL,
						"end_turn_hover");
	
	// Loading tchat button
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/tchat_icon_64.png")),
						NULL,
						"tchat_button");

	// Loading tchat button (hover)
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/tchat_icon_hover_64.png")),
						NULL,
						"tchat_button_hover");

	// Loading tchat  (selected)
	addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/tchat_icon_selected_64.png")),
						NULL,
						"tchat_button_selected");

	// Loading ID card texture
	index = addTextureToTable(	textures,
						loadTexture(renderer, loadImage("../inc/img/id_card.png")),
						NULL,
						"id_card");

	if(verbose)printf("[GRAPHICS] %d texture(s) chargée(s) !\n", index+1);

	return index+1;
}

float crossProduct(Vector AB, Vector AC)
// Renvoie le produit vectoriel
{
	return (( AB.y * AC.x ) - ( AB.x * AC.y ));
}

int selectTile(int xpos, int ypos, int mx, int my)
// Set the tile selected according to 2D iso from 2D coordinates
{
	int xIndex, yIndex, xIsoOrigin, yIsoOrigin, xTile, yTile;
	float cpAB, cpBC, cpDC, cpAD;

	// On déselectionne toutes les cases
	for (int i=0; i<_X_SIZE_; i++){
		for (int j=0; j<_Y_SIZE_; j++){
			(*(matrix+i*_X_SIZE_+j)).selected = 0;
		}
	}

	// Position de l'origine de la map en 2D isométrique
	xIsoOrigin = xpos;
	yIsoOrigin = ypos+_Y_SIZE_*(pxBase/4);

	// Coordonnées 2D -> 2D iso
	xIndex = floor(((my-yIsoOrigin)/(pxBase/2) + ((mx-xIsoOrigin)/pxBase)))-1;
	yIndex = ceil((((mx-xIsoOrigin)/pxBase) - (my-yIsoOrigin)/(pxBase/2)))-1;

	if (my < yIsoOrigin){
		xIndex--;
		yIndex++;
	}

	xTile = xpos+((((xIndex+yIndex)/2)+1)*pxBase);
	yTile = ypos+((_Y_SIZE_-(yIndex-xIndex))*(pxBase/4)+(pxBase/4));
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

	if (xIndex > _X_SIZE_-1 || yIndex > _Y_SIZE_-1 || xIndex < 0 || yIndex < 0)
	{
		selected_ability = -1;
		return 0;
	}

	if(verbose)printf("[GRAPHICS] Case sélectionnée : %d, %d\n", xIndex, yIndex);
	(*(matrix+xIndex*_X_SIZE_+yIndex)).selected = 1;
	Coord selectedTile = {xIndex, yIndex};
	Entity *selectedEntity = getEntity(selectedTile);
	if (selectedEntity != NULL)
	{
		action act = {selectedEntity->cha_id, selectedTile, selected_ability};
		if (selected_ability != -1)
		{
			apply_action(act);
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
	displaySprite(renderer, getTexture(textures, "turn_right"), 16+5*80, yWinSize-80);
	displaySprite(renderer, getTexture(textures, "turn_left"), 16+6*80, yWinSize-80);

	return 0;
}

err_t setChatTab(char chatTab[STR_LONG][_NB_MAX_ChAT_])
// Set-up the chat array
{
	return OK;
}

int displayInterface(SDL_Renderer *renderer)
// Display the UI
{
	Entity * tempEntity = getEntity(getSelectedPos());
	char passive[100];
	char pv_text[2];
	char pa_text[2];
	char pm_text[2];

	SDL_Rect chatScreen;
	chatScreen.x = xWinSize-300;
	chatScreen.y = yWinSize-460;
	chatScreen.w = 275;
	chatScreen.h = 310;

	SDL_Rect chatBox;
	chatBox.x = chatScreen.x - 5;
	chatBox.y = chatScreen.y - 35;
	chatBox.w = chatScreen.w + 10;
	chatBox.h = 375;

	SDL_Rect chatMsg;
	chatMsg.x = chatScreen.x;
	chatMsg.y = chatScreen.y + chatScreen.h + 2;
	chatMsg.w = chatScreen.w;
	chatMsg.h = 20; 


	if (tempEntity != NULL)
	{
		displayAbilities(renderer);
		if (selected_ability != -1){
			displayText(renderer, 16, yWinSize-110, 20, get_desc(tempEntity, selected_ability), "../inc/font/Pixels.ttf", 255, 255, 255);
		} else {
			if (hover_ability >= 0) 
			{
				char abilityDesc[STR_LONG];
				sprintf(abilityDesc, "%s : %s", strToUpper(get_name(tempEntity, hover_ability)), get_desc(tempEntity, hover_ability));
				displayText(renderer, 16, yWinSize-110, 20, abilityDesc, "../inc/font/Pixels.ttf", 255, 255, 255);
			}
		}

		// Display the ID card of the selected entity
		displaySprite(renderer, getTexture(textures, "id_card"), 10, 10);
		displayText(renderer, 382, 128, 18, "?", "../inc/font/Pixels.ttf", 255, 255, 255);
		displaySprite(renderer, getCharFrontTexture(tempEntity->cha_class->cla_name), 51, 52);
		displayText(renderer, 170, 45, 20, tempEntity->cha_name, "../inc/font/Pixels.ttf", 255, 255, 255);

		// -- entity health
		displaySprite(renderer, getBigTexture(cSprites, "heart_icon"), 170, 70);
		sprintf(pv_text, "%d", tempEntity->stat_mods[pv]);
		displayText(renderer, 200, 70, 30, pv_text, "../inc/font/Pixels.ttf", 255, 0, 0);

		// -- entity action points
		displaySprite(renderer, getBigTexture(cSprites, "star_icon"), 165, 102);
		sprintf(pa_text, "%d", tempEntity->act_points);
		displayText(renderer, 202, 106, 30, pa_text, "../inc/font/Pixels.ttf", 49, 174, 196);

		// -- entity mouvement points
		displaySprite(renderer, getBigTexture(cSprites, "mv_icon"), 250, 102);
		sprintf(pm_text, "%d", tempEntity->stat_mods[mv]);
		displayText(renderer, 287, 106, 30, pm_text, "../inc/font/Pixels.ttf", 52, 169, 43);

		// -- passive description if hovering info icon
		if (hover_passive_help == 1)
		{
			sprintf(passive, "Passive : %s", tempEntity->cha_class->Passive.name);
			displayText(renderer, mouse_position.x+20, mouse_position.y+20, 20, passive, "../inc/font/Pixels.ttf", 238, 165, 53);
			displayText(renderer, mouse_position.x+20, mouse_position.y+40, 20, tempEntity->cha_class->Passive.desc, "../inc/font/Pixels.ttf", 238, 165, 53);
		}
	}

	// Logs
	Coord logPos;
	logPos.x = 20;
	if (tempEntity != NULL) logPos.y = 170;
	else logPos.y = 20;
	removeOldLogs(SDL_GetTicks());
	displayLog(renderer, logPos);

	// Next turn button
	if (hover_next_turn == TRUE)
	{
		displayText(renderer, xWinSize-280, yWinSize-110, 20, "Skip turn", "../inc/font/Pixels.ttf", 255, 255, 255);
		displaySprite(renderer, getTexture(textures, "end_turn_hover"), xWinSize-280, yWinSize-80);
	}
	else
	{
		displaySprite(renderer, getTexture(textures, "end_turn"), xWinSize-280, yWinSize-80);
	}


	// Tchat window
	if (isChatActive) displaySprite(renderer, getTexture(textures, "tchat_button_selected"), xWinSize-360, yWinSize-80);
	else displaySprite(renderer, getTexture(textures, "tchat_button"), xWinSize-360, yWinSize-80);
	if (hover_tchat == 1 || hover_tchat == 2) displaySprite(renderer, getTexture(textures, "tchat_button_hover"), xWinSize-360, yWinSize-80);
	if (hover_tchat == 1)displayText(renderer, xWinSize-360, yWinSize-110, 20, "Hide tchat", "../inc/font/Pixels.ttf", 255, 255, 255);
	else if (hover_tchat == 2) displayText(renderer, xWinSize-360, yWinSize-110, 20, "Display tchat", "../inc/font/Pixels.ttf", 255, 255, 255);
	if(isChatActive){
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 153, 153, 153, 185);
		SDL_RenderFillRect(renderer, &chatBox);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
		SDL_RenderFillRect(renderer, &chatScreen);
		displayText(renderer, chatBox.x + (chatBox.w /2) - 10, chatBox.y + 5, 25, "Chat", "../inc/font/Pixels.ttf", 255, 255, 255);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
		SDL_SetRenderDrawColor(renderer, 85, 34, 0, 255);
		SDL_RenderFillRect(renderer, &chatMsg);

		displayText(renderer, chatMsg.x+2, chatMsg.y + 2, 15 , "Chat :", "../inc/font/PixelOperator.ttf", 255, 255, 255);
	}
	
	

	return 0;
}

int displayMap(SDL_Renderer *renderer, int x, int y)
// Display the map
{
	/* Le fond de la fenêtre sera blanc */
    SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
	SDL_RenderClear(renderer);

    Coord blockPos;

	/* Le fond de la fenêtre sera blanc */
    SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
	SDL_RenderClear(renderer);

    for (int i=0; i < _X_SIZE_; i++){
        for (int j=(_Y_SIZE_-1); j >= 0; j--){

			blockPos.x = x+(j+1)*(pxBase/2)+(i+1)*(pxBase/2);
			blockPos.y = y+i*(pxBase/4)+(_Y_SIZE_-j)*(pxBase/4);

			if (blockPos.x >= -pxBase && blockPos.x <= xWinSize && blockPos.y >= -pxBase && blockPos.y <= yWinSize)
			{

				// Affichage block
				if ((*(matrix+i*_X_SIZE_+j)).tile_id == 1)
				{
					if (pxBase == 64)	displaySprite(renderer, getTexture(textures, "blue_selected"), blockPos.x, blockPos.y);
					else				displaySprite(renderer, getBigTexture(textures, "blue_selected"), blockPos.x, blockPos.y);
					if (pxBase == 64)	displaySprite(renderer, getTexture(textures, "block"), blockPos.x, blockPos.y);
					else				displaySprite(renderer, getBigTexture(textures, "block"), blockPos.x, blockPos.y);
				}
				else if ((*(matrix+i*_X_SIZE_+j)).tile_id <= 7)
				{
					if (pxBase == 64)	displaySprite(renderer, textures[(*(matrix+i*_X_SIZE_+j)).tile_id].texture, blockPos.x, blockPos.y);
					else 				displaySprite(renderer, textures[(*(matrix+i*_X_SIZE_+j)).tile_id].big_texture, blockPos.x, blockPos.y);
				}

				// Affichage équipe
				if ((*(matrix+i*_X_SIZE_+j)).entity != NULL)
				{
					if (pxBase == 64)
					{
						if ((*(matrix+i*_X_SIZE_+j)).entity->cha_id > 0) displaySprite(renderer, getTexture(textures, "blue_team"), blockPos.x, blockPos.y);
						else displaySprite(renderer, getTexture(textures, "red_team"), blockPos.x, blockPos.y);
					}
					else
					{
						if ((*(matrix+i*_X_SIZE_+j)).entity->cha_id > 0) displaySprite(renderer, getBigTexture(textures, "blue_team"), blockPos.x, blockPos.y);
						else displaySprite(renderer, getBigTexture(textures, "red_team"), blockPos.x, blockPos.y);
					}
				}

				// Affichage tuile de sélection
				if ((*(matrix+i*_X_SIZE_+j)).selected == 1)
				{
					if (pxBase == 64)	displaySprite(renderer, getTexture(textures, "selection"), blockPos.x, blockPos.y);
					else				displaySprite(renderer, getBigTexture(textures, "selection"), blockPos.x, blockPos.y);
				}

				if ((*(matrix+i*_X_SIZE_+j)).entity != NULL)
				{
					displayCharacters(renderer, cSprites, (*(matrix+i*_X_SIZE_+j)).entity, blockPos.x, blockPos.y-pxBase/1.6, pxBase);
				}

				if (camMove != -1)
				{
					if (mouse_position.x <= xWinSize && mouse_position.x >= xWinSize-20){
						displaySprite(renderer, getTexture(textures, "arrow_right"), xWinSize-64, mouse_position.y-32);
					}
					if (mouse_position.x >= 0 && mouse_position.x <= 20){
						displaySprite(renderer, getTexture(textures, "arrow_left"), 0, mouse_position.y-32);
					}
					if (mouse_position.y <= yWinSize && mouse_position.y >= yWinSize-20){
						displaySprite(renderer, getTexture(textures, "arrow_down"), mouse_position.x-32, yWinSize-64);
					}
					if (mouse_position.y <= 20 && mouse_position.y >= 0){
						displaySprite(renderer, getTexture(textures, "arrow_up"), mouse_position.x-32, 0);
					}
				}

				/*/ -- DEBUG Affichage des indices des tuiles --
				char pos[6];
				sprintf(pos, "%d,%d", i, j);
				displayText(renderer, imgDestRect.x+(pxBase/2)-10, imgDestRect.y+(pxBase/4), (pxBase/64)*10, pos, "../inc/font/Pixels.ttf", 255, 50, 50);
				// -- DEBUG --*/

			}
        }
    }

	displayInterface(renderer);

	/* -- DEBUG Affichage des coordonnées d'affichage de la map
	char str[12];
	sprintf(str, "%d, %d", x, y);
	displayText(renderer, 20, 20, 20, str, "../inc/font/Pixels.ttf", 255, 255, 255);
	// -- DEBUG --*/

	SDL_RenderPresent(renderer);

	return 1;
}
