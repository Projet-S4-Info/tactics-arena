/**
 * \file grid.c
 * \brief Contains all the functions relative to the grid of the game
 * \author Thibault DOUCET
 * \version 0.0.1
 * \date 28/01/2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grid.h"
#include "characters.h"
#include "common.h"
#include "graphics.h"

#define _X_SIZE_ 30
#define _Y_SIZE_ 30

/**
 * \fn void createGrid(Entity * grid, int x, int y)
 * \param grid A grid of entity
 * \param x Row's number
 * \param y Column's number
 * \brief Initialize the grid with x*y size in parameter with default entities
 */
void createGrid(Tile * grid, int seed, int x, int y)
// create the grid with x*y size
{
    srand(time(NULL));
    for (int i = 0; i < x*y; i++){
        grid[i].tile_id = rand()%seed;
        int t = rand()%5;
        grid[i].selected = 0;
        grid[i].entity = NULL;
    }
}

/**
 * \fn void debugGrid(Entity * grid, int x, int y)
 * \param grid Pointer to the begining of the entity matrix
 * \param x Row's number
 * \param y Column's number
 * \brief Display the entity ids of the matrix
 */
void debugGrid(Tile * grid, int x, int y)
// Debug by displaying the grid on the cmd
{
    for (int i = 0; i < x; i++){
        for (int j = 0; j < y; j++){
            if(verbose)printf("%d ", (*(grid+i*x+j)).tile_id);
            if ((*(grid+i*x+j)).entity != NULL && verbose == TRUE) printf("X");
        }
        if(verbose)printf("\n");
    }
}

Entity * getEntity(Tile * grid, Coord pos)
// Returns entity at given coord, if not return NULL
{
    if ((*(grid+pos.x*30+pos.y)).entity == NULL) return NULL;
    else return (*(grid+pos.x*30+pos.y)).entity;
}

int Set_Trap(Trap_t trap, Coord pos)
// Set a trap on a tile
{
    (*(matrix+pos.x*30+pos.y)).trap = trap;
    return 0;
}

Tile getSelected()
// Return the selected tile
{
    for (int i=0; i < _X_SIZE_; i++)
    {
        for (int j=0; j < _Y_SIZE_; j++)
        {
            if ((*(matrix+i*_X_SIZE_+j)).selected == 1) return (*(matrix+i*_X_SIZE_+j));
        }
    }
}

Coord getSelectedPos()
// Return the coordinates of the selected tile
{
    for (int i=0; i < _X_SIZE_; i++)
    {
        for (int j=0; j < _Y_SIZE_; j++)
        {
            if ((*(matrix+i*_X_SIZE_+j)).selected == 1)
            {
                Coord res = {i, j};
                return res;
            }
        }
    }
}

void unselect()
// Unselect the selected tile
{
    for (int i=0; i < _X_SIZE_; i++)
    {
        for (int j=0; j < _Y_SIZE_; j++)
        {
            if ((*(matrix+i*_X_SIZE_+j)).selected == 1) (*(matrix+i*_X_SIZE_+j)).selected = 0;
        }
    }
}

void setSelected(Coord pos)
// Set the tile at pos selected
{
    unselect();
    (*(matrix+pos.x*_X_SIZE_+pos.y)).selected = 1;
}