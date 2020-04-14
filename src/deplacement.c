#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "file_coord.h"
#include "gameplay.h"
#include "grid.h"


Coord closest_free_tile(Coord c)
{
    Coord active;
    File *maFile = initialiser();
    enfiler(maFile, c);
    Tile * t;
    int i;
    Coord add[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    while (1)
    {
        active = defiler(maFile);
        t = getTile(active);
        if(t->entity == NULL && t->walkable)
        {
            detruire_file(maFile);
            return active;
        }
        else
        {
            for(i=0; i<4; i++)
            {
                enfiler(maFile, add_coords(active, add[i]));
            }
        }
        

    }
}


err_t fill_tiles(Coord c, int matrix[_X_SIZE_][_Y_SIZE_])
{
    //On initialise la matrice à -1
    int i, j;
    for(i = 0; i < _X_SIZE_; i ++)
    {
        for(j = 0; j < _Y_SIZE_; j ++)
        {
            matrix[i][j] = -1;
        }
    }
    File *maFile = initialiser();
    enfiler(maFile, c);
    matrix[c.x][c.y] = 0;
    Coord active;
    Coord active_2;
    Coord add[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    Tile * t;
    while(!file_vide(maFile))
    {

        active = defiler(maFile);
        
        for(i=0; i<4; i++)
        {
            active_2 = add_coords(active, add[i]);

            t = getTile(active_2);

            if(matrix[active_2.x][active_2.y]==-1 && !(t->walkable) && t->entity == NULL)
            {
                matrix[active_2.x][active_2.y] = matrix[active.x][active.y] + 1;
                enfiler(maFile, active_2);
            }
        }
    }

    return OK;
}

