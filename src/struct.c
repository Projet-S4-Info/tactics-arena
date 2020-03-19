/**
 * \file struct.c
 * \brief struct related functions
 * \details Contains functions to initialise and save data structures
 * \author Robin BUTEL
 * \version 0.0.1
 * \date 04/02/2020
 */
#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

//err_t get_ability()

err_t get_classes(Class all_classes[NUM_CLASS])
{
    FILE *fp = NULL;
    fp=fopen("classes","r");
    if(fp==NULL) return POINTER_NULL;
    fread(all_classes, sizeof(Class), NUM_CLASS, fp);
    fclose(fp);

    /*int i;
    for(i=0; i<NUM_CLASS; i++)
    {
        Ability * ab = malloc(sizeof(Ability)*NUM_AB);
        get_ability(ab);
    }*/

    return OK;
}
