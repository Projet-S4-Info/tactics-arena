#include <stdlib.h>
#include <time.h>
#include "struct.h"
#include "init_classes.h"

StateList *stAlly = NULL;
StateList *stFoe = NULL;
Class classes[NUM_CLASS] = {};
Ability mage_ab[3][NUM_AB] = {};
Entity Allies[NUM_CLASS] = {};
Entity Foes[NUM_CLASS] = {};

err_t init_game()
{
    srand(time(NULL));
    return OK;
}