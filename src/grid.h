#ifndef grid_h
#define grid_h

#include "struct.h"

void createGrid(Tile * grid, int seed, int x, int y);
void debugGrid(Tile * grid, int x, int y);
Entity * getEntity(Tile * grid, Coord pos);
int Set_Trap(Trap_t trap, Coord pos);
Trap_t Get_Trap(Coord pos);
Tile getSelected();
Coord getSelectedPos();
void unselect();
void setSelected(Coord pos);
void freezeWater(Coord pos);
Tile * getTile(Coord pos);

#endif