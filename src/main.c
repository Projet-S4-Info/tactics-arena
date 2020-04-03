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
#include "common.h"
#include "init.h"
#include "errt.h"

#define X 30
#define Y 30

/**
 * \fn int main(void)
 * \return void
 * \brief Main function
 */

bool verbose = FALSE;
int main(int argc, char * argv[])
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

    int selection = 0;

    if(argc < 3){
        if(argc == 2){
            if(strcmp(argv[1], "-c") == 0){
                verbose = TRUE;
            }else{
                printf("Argument invalide... \n");
            }
        }else if(argc == 1){
            verbose = FALSE;
        }
    }else{
        printf("Nombre de paramètres incorrectes ...");
        printf("Usage : ./TacticsArena <arg1> : -c \n");
    }


    playMenuMusic(1);
    selection = displayMenu(1280, 720);
    
    error_message[init_game()];

    // Lancement du jeu
    if (selection == 2)
    {
        stopMenuMusic(1);
        createGameWindow(1920, 1080);
    }

    // Editeur de map
    else if (selection == 3)
    {
        stopMenuMusic(1);
        createMapEditorWindow(1920, 1080);
    }
                
    // Multiplayer
    else if (selection == 4){
        stopMenuMusic(1);
        int res = 0;
        playMenuMusic(2);
        res = displayMenuMulti(1280,720);
        if(res == 1){
            createGameWindow(1920,1080);
        } else if(res == 2) {
            createGameWindow(1920,1080);
        }
    }

    //getchar();  // Wait for the user to press a key to end the program

    error_message[destroy_game()];

    return 0;
}
