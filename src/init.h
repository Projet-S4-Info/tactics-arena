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
extern Ability Move_ab;

err_t init_spawn(Entity * e, Coord c[NUM_CLASS]);
err_t ent_common_init(Entity *e);
err_t init_Entity(Entity * e,Coord spawn[NUM_CLASS], Direction d, char title[STR_SHORT], int id_mod);
err_t ent_init_test();
err_t init_classes();
err_t destroy_game();

#endif