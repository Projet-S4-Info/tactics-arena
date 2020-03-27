#ifndef init_h
#define init_h
#include "struct.h"

extern StateList *stAlly;
extern StateList *stFoe;
extern Class classes[NUM_CLASS];
extern Ability mage_ab[3][NUM_AB];
extern Entity Allies[NUM_CLASS];
extern Entity Foes[NUM_CLASS];

err_t init_game();

#endif