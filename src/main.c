/**
 * \file main.c
 * \brief Main program for TacticsArena
 * \details Contains main function
 * \author Thibault DOUCET
 * \version 0.0.1
 * \date 28/01/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "Socket_Server.h"
#include "terrain.h"
#include "graphics.h"
#include "audio.h"
#include "menu.h"
#include "menu_multi.h"
#include "map_editor.h"
#include "characters.h"

#define X 30
#define Y 30

/**
 * \fn int main(void)
 * \return void
 * \brief Main function
 */
int main()
{   
    #ifdef _WIN32
    /*
    * Change the cmd codepage on Windows
    */
    system("chcp 65001");
    system("cls");
    #else
    system("clear");
    #endif
    
    Tile grid[X][Y];                                  /**< Contains the pointer to the start of the matrix */
    Tile *matrix = &grid[0][0];

    Tile blankGrid[X][Y];                             /**< Contains the pointer to the start of the blank matrix */
    Tile *blankMatrix = &blankGrid[0][0];

    int choix = 0;
    int selection = 0;
    printf("Bienvenue dans le serveur :) \n");


    printf("Lancer le serveur (1) \n");
    printf("Lancer le client (2) \n");
    printf("Lancer le jeux (3) \n");
    printf("\nFaites votre choix : \n");
    scanf("%d", &choix);

    switch (choix) {
        case 1: startTCPSocketServ();
        break;
        case 2: startTCPSocketCli();
        break;
        case 3:
                createGrid((Tile *)matrix, 7, X, Y);
                createGrid((Tile *)blankGrid, 1, X, Y);
                Entity * entity = malloc(sizeof(Entity));
                entity->base_stats[0] = 40;
                Coord test = {23,10};
                setEntityToTile(matrix, entity, test, X, Y);
                playMenuMusic();
                selection = displayMenu(1280, 720);

                // Lancement du jeu
                if (selection == 2)
                {
                    stopMenuMusic();
                    createGameWindow(1920,1080, matrix, X, Y);
                }

                // Editeur de map
                else if (selection == 3)
                {
                    stopMenuMusic();
                    createMapEditorWindow(1920, 1080, blankMatrix, X, Y);
                }
                
                // Multiplayer
                else if (selection == 4){
                    int res = 0;
                    res = displayMenuMulti(1280,720);
                    if(res == 1){
                        createGameWindow(1920,1080,matrix, X, Y);
                    }else if(res == 2){
                        createGameWindow(1920,1080,matrix, X, Y);
                    }
                }

                debugGrid(matrix, X, Y);
                printf("Entity at pos %d:%d : %d\n", 5, 7, getTileId(getTile(matrix, X, 5, 7)));
                free(matrix);
        break;
  }

    

    

    getchar();  // Wait for the user to press a key to end the program

    return 0;
}
