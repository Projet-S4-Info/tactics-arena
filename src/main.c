/**
 * \file main.c
 * \brief Main program for TacticsArena
 * \details Contains main function
 * \author Thibault DOUCET
 * \version 0.0.1
 * \date 28/01/2020
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "game_window.h"
#include "audio.h"
#include "menu.h"
#include "multi_menu.h"
#include "map_editor.h"
#include "characters.h"
#include "stringdb.h"
#include "init.h"
#include "print.h"

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
            }
            else{
                printf("Argument invalide... \n");
            }
        }else if(argc == 1){
            verbose = FALSE;
        }
    }else{
        printf("Nombre de paramètres incorrectes ...");
        printf("Usage : ./TacticsArena <arg1> : -c \n");
    }

    srand(time(NULL));

    printf("%s", error_message[ init_classes() ] );
    
    if(verbose)
    {
        printf("Initiation Check!\n\n");
        int i,j;

        for(i=0; i<NUM_CLASS; i++)
        {
            printf("\n%s\n", classes[i].cla_name);
            if(classes[i].cla_abilities!=NULL)
            {
                for(j=0; j<NUM_AB; j++)
                {
                    print_Ability(classes[i].cla_abilities+j, "");
                }
            }
            else
            {
                printf("Abilities : NULL\n");
            }
            
        }

        for(i=0; i<3; i++)
        {
            printf("\nMage Abilities");

            for(j=0; j<NUM_AB; j++)
            {
                switch (j)
                {
                    case 0:
                        printf("\nFire\n");
                        break;
                    case 1:
                        printf("\nIce\n");
                        break;
                    case 2:
                        printf("\nLighting\n");
                        break;
                }

                print_Ability(&mage_ab[i][j], "");

            }
        }

    }

    playMenuMusic(1);
    selection = displayMenu(1280, 720);
    

    // Lancement du jeu
    if (selection == 2)
    {
        ent_init_test(Allies, "Friendly");
        ent_init_test(Foes, "Ennemy");
        if(verbose) printf("GAME START!\n");
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

    printf("%s",error_message[destroy_game()]);

    //getchar();  // Wait for the user to press a key to end the program

    return 0;
}
