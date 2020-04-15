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
Ability Move_ab;

err_t init_spawn(Entity * e, Coord c[NUM_CLASS], int i);
err_t ent_common_init(Entity *e);
err_t init_Foes();
err_t init_Allies();
err_t ent_init_test(Entity *e, char title[STR_SHORT]);
err_t init_classes();
err_t destroy_game();

#endif