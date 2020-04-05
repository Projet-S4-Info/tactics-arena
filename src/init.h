#ifndef init_h
#define init_h
#include "struct.h"

extern StateList * stSent;
extern StateList * stReceived;
extern Class classes[NUM_CLASS];
extern Ability mage_ab[3][NUM_AB];
extern Entity Allies[NUM_CLASS];
extern Entity Foes[NUM_CLASS];
extern Ability Aura_ab;

err_t init_game();
err_t destroy_game();

#endif