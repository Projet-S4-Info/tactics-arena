#ifndef deplacement_h
#define deplacement_h
#include "struct.h"
#include "grid.h"

Coord closest_free_tile(Coord c);
err_t fill_tiles(Coord c, int matrice[_X_SIZE_][_Y_SIZE_]);

#endif