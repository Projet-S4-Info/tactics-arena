#include "init.h"
#include "print.h"
#include "deplacement.h"
#include "grid.h"
#include "map_editor.h"
#include "border.h"
#include "gameplay.h"
#include <stdio.h>
#include <stdlib.h>

void test()
{
    printf("\n");
    int i;
    for(i=0; i<NUM_CLASS; i++)
    {
        print_class_name(i,"");
    }
}