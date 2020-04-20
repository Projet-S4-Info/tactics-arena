#ifndef deplacement_h
#define deplacement_h
#include "struct.h"
#include "grid.h"

Coord closest_free_tile(Coord c);
int * fill_tiles(Coord c, int matrice[_X_SIZE_][_Y_SIZE_], int max);
Coord * pathfinding(int matrice[_X_SIZE_][_Y_SIZE_], Coord tabcoord[], Coord goal);
err_t simple_move(Entity * e, Coord tabcoord[]);
err_t total_move(Entity * e, Coord tabcoord[]);

#endif