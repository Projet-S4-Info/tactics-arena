#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "struct.h"
#include "file_coord.h"
#include "gameplay.h"
#include "grid.h"
#include "characters.h"
#include "passives.h"
#include "map.h"
#include "game_window.h"
#include "print.h"


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
        if(verbose)printf("x : %d    y : %d\n", active.x, active.y);
        if(verbose)printf("Null : %d    Walk : %d\n", t->entity == NULL, t->walkable);

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


int * fill_tiles(Coord c, int matrice[_X_SIZE_][_Y_SIZE_], int max)
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

    if(verbose)printf("\n");
    while(!file_vide(maFile))
    {
        active = defiler(maFile);

        if(verbose)printf("Active : %d %d\n", active.x, active.y);

        for(i=0; i<4; i++)
        {
            active_2 = add_coords(active, add[i]);
            if(verbose)printf("    Active_2 : %d %d\n", active_2.x, active_2.y);
            if(isInGrid(active_2))
            {
                if(verbose)printf("        In Grid\n");
                t = getTile(active_2);

                if(matrice[active_2.x][active_2.y]==-1 && t->walkable && t->entity == NULL)
                {
                    if(verbose)printf("        Adding\n");
                    matrice[active_2.x][active_2.y] = matrice[active.x][active.y] + 1;
                    if(matrice[active_2.x][active_2.y] < max)
                    {
                        enfiler(maFile, active_2);
                    }
                }
                else
                {
                    if(verbose)printf("        Not Adding\n");
                }
            }
        }
    }
    if(verbose)printf("Fill Tiles Done\n");
    return (int *)matrice;
}

Coord * pathfinding(int matrice[_X_SIZE_][_Y_SIZE_], Coord tabcoord[], Coord goal)
{
    Coord tab_bis[MAXRANGE];
    Coord add[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    Coord active;
    Coord is_lowest; 
    int i, lowest = 1000, t = 0;
    tab_bis[t++] = goal;
    for(i = 0; i < 4; i++)
    {
        active = add_coords(goal, add[i]);
        if(matrice[active.x][active.y] < lowest && matrice[active.x][active.y] != -1)
        {
            is_lowest.x = active.x;
            is_lowest.y = active.y;
            lowest = matrice[active.x][active.y];
        }
    }
    while(matrice[is_lowest.x][is_lowest.y] > 0)
    {
        tab_bis[t++] = is_lowest;
        for(i = 0; i < 4; i ++)
        {
            active = add_coords(is_lowest , add[i]);
            if(matrice[active.x][active.y] == lowest - 1)
            {
                is_lowest.x = active.x;
                is_lowest.y = active.y;
                lowest -= 1; 
                break;
            }
        }   
    }

    if(verbose)
    {
        printf("Goal : ");
        print_Coord(&goal, "");
        printf("Origin : ");
        print_Coord(&is_lowest, "");
    }

    for(t -= 1, i = 0; t >= 0; t --, i ++)
    {
        tabcoord[i] = compare_coords(is_lowest,tab_bis[t]);
    }

    tabcoord[i].x = -99;
    tabcoord[i].y = -99;
    if(verbose)printf("Path Found\n");
    return tabcoord;
}

err_t simple_move(Entity * e, Coord tabcoord[])
{
    int i;
    Coord c, start = e->coords;

    selected_ability = -1;

    if(verbose)
    {
        for(i=0; tabcoord[i].x!=-99; i++)
        {
            print_Coord(&tabcoord[i], "");
        }
    }

    unselect();

    for(i = 0; tabcoord[i].x != -99; i ++)
    {
        c = add_coords(start, tabcoord[i]);
        if(TRUE)printf("Moving from (%d,%d) to (%d,%d)...", e->coords.x, e->coords.y, c.x, c.y);
        moveEntity(e->coords, c);
        Coord target = {e->coords.x, e->coords.y};
        setSelected(target);
        if(TRUE)printf("Completed\n");
        sentinel_check(e);
        displayMap(SDL_GetRenderer(pWindow), XPOS, YPOS);
        SDL_Delay(250);
    }

    return OK;
}