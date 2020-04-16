#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "file_coord.h"
#include "gameplay.h"
#include "grid.h"


Coord closest_free_tile(Coord c)
{
    Coord active = {-1,-1};
    File *maFile = initialiser();
    enfiler(maFile, c);
    Tile * t;
    int i;
    Coord add[4] = {{1,0},{-1,0},{0,1},{0,-1}};

    while (!file_vide(maFile))
    {
        active = defiler(maFile);
        t = getTile(active);
        printf("x : %d    y : %d\n", active.x, active.y);
        printf("Null : %d    Walk : %d\n", t->entity == NULL, t->walkable);

        if(t->entity == NULL && t->walkable)
        {
            detruire_file(maFile);
            break;
        }
        else
        {
            for(i=0; i<4; i++)
            {
                Coord a = add_coords(active, add[i]);
                if(isInGrid(a))
                {
                    enfiler(maFile, a);
                }
            }
        }
    }

    return active;
}


int * fill_tiles(Coord c, int matrice[_X_SIZE_][_Y_SIZE_])
{
    //On initialise la matrice à -1
    int i, j;
    for(i = 0; i < _X_SIZE_; i ++)
    {
        for(j = 0; j < _Y_SIZE_; j ++)
        {
            matrice[i][j] = -1;
        }
    }

    File *maFile = initialiser();
    enfiler(maFile, c);
    matrice[c.x][c.y] = 0;
    Coord active;
    Coord active_2;
    Coord add[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    Tile * t;

    printf("\n");
    while(!file_vide(maFile))
    {
        active = defiler(maFile);

        printf("Active : %d %d\n", active.x, active.y);

        for(i=0; i<4; i++)
        {
            active_2 = add_coords(active, add[i]);
            printf("    Active_2 : %d %d\n", active_2.x, active_2.y);
            if(isInGrid(active_2))
            {
                printf("        In Grid\n");
                t = getTile(active_2);

                if(matrice[active_2.x][active_2.y]==-1 && t->walkable && t->entity == NULL)
                {
                    printf("        Adding\n");
                    matrice[active_2.x][active_2.y] = matrice[active.x][active.y] + 1;
                    enfiler(maFile, active_2);
                }
                else
                {
                    printf("        Not Adding\n");
                }
            }
        }
    }
    return (int *)matrice;
}

Coord * pathfinding()
{
    //je me place à la coord de fin sur la matrice et je la met dans le tableau bis 
    //je regarde les chiffres aux alentours 
    //je prends le plus petit (sauf -1)
    //ensuite je suis le chemin du plus petit au plus petit jusqu'a 0 
    //je met toute les coordonnées passé dans le tableau bis sauf la derniere qui est 0 
    //je remet les coord dans l'ordre dans le tableau donné en paramètre 
    //j'ajoute -99,-99 à la fin
    //je renvoie ce tableau
}