#include "init.h"
#include "print.h"
#include "deplacement.h"
#include "grid.h"
#include "map_editor.h"
#include <stdio.h>

void test()
{
    Entity * e;

    int i;
    for(i=0; i<NUM_CLASS; i++)
    {
        printf("\n");
        printf("Allies[%d] : %s\n", i, Allies[i].cha_class->cla_name);
        printf("x : %d y : %d\n", Allies[i].coords.x, Allies[i].coords.y);
        e = getEntity(Allies[i].coords);
        printf("Tile : %d\n", e == &Allies[i]);

        printf("\n");
        printf("Foes[%d] : %s\n", i, Foes[i].cha_class->cla_name);
        printf("x : %d y : %d\n", Foes[i].coords.x, Foes[i].coords.y);
        e = getEntity(Foes[i].coords);
        printf("Tile : %d\n", e == &Foes[i]);

    }

    printf("\nend of\n");
}