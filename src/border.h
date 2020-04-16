#ifndef border_h
#define border_h
#include "struct.h"
#include "grid.h"

int actionZone(int posX, int posY, int actionRange, Coord coorTab[]);
bool isInRange(Coord coorTab[], Coord attack);
int setActionZone(Coord perso, int actionRange, Coord coorTab[]);
Coord * setMovementZone(int matrice[_X_SIZE_][_Y_SIZE_], Coord tab[]);
err_t get_border(int cha_id, abilityId Id, Coord coorTab[]);
bool Cast_check(action a, Coord coorTab[]);

#endif