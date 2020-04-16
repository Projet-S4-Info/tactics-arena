#ifndef border_h
#define border_h
#include "struct.h"
#include "grid.h"

bool isInRange(Coord coorTab[], Coord target);
Coord normalize(Coord c);
Coord * setActionBorder(Coord start, int range, Coord tab[]);
Coord * setActionZone(int range, Coord Border[], Coord Zone[]);
Coord * setMovementBorder(int matrice[_X_SIZE_][_Y_SIZE_], Coord tab[]);
Coord * setMovementZone(int matrice[_X_SIZE_][_Y_SIZE_], Coord tab[]);
Coord * get_border(int cha_id, abilityId Id, Coord coorTab[], Coord zone[]);
bool Cast_check(action a, Coord coorTab[]);



#endif