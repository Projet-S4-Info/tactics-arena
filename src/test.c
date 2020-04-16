#include "init.h"
#include "print.h"
#include "deplacement.h"
#include "grid.h"
#include "map_editor.h"
#include "border.h"
#include <stdio.h>

void test()
{
    loadMap(matrix, "map_sandy_pass");

	debugGrid(matrix, _X_SIZE_, _Y_SIZE_);

    Coord coorTab[MAXRANGE];
    Coord zone[_X_SIZE_*_Y_SIZE_];
    Coord c ={0,0};

    printf("\n");
    setActionBorder(c, 8, coorTab);
    setActionZone(8,coorTab,zone);

    print_grid(coorTab);
    printf("\n");
    print_grid(zone);

}