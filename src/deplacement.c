#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "file_coord.h"
#include "gameplay.h"
#include "grid.h"


Coord closes_free_tile(Coord c)
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

