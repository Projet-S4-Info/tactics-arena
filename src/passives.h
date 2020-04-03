#ifndef passives_h
#define passives_h
#include "common.h"

extern int Bloodlust_counter;

err_t activate_bloodlust(Entity *e, StateList * list);

err_t activate_aura(Entity *e);

#endif