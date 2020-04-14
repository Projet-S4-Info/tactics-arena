#include "init.h"
#include "print.h"
#include "deplacement.h"
#include "grid.h"
#include <stdio.h>

err_t test()
{
    Coord c = {0,0};
    createGrid(matrix, 5, _X_SIZE_, _Y_SIZE_);
    int matrice[_X_SIZE_][_Y_SIZE_];
    fill_tiles(c, matrice);

    int i,j;

    printf("\n\nWALKABILITY\n");
    for(i=0; i<_X_SIZE_; i++)
    {
        for(j=0; j<_Y_SIZE_; j++)
        {
            printf("%d ", (matrix + j + (i*_X_SIZE_))->walkable);
        }
        printf("\n");
    }

    printf("\n\nFill Tiles\n");
    for(i=0; i<_X_SIZE_; i++)
    {
        for(j=0; j<_Y_SIZE_; j++)
        {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
    return OK;
}