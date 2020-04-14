#ifndef passives_h
#define passives_h
#include "common.h"

extern int Bloodlust_counter;
extern bool Sentinel_counter;

err_t activate_bloodlust(Entity *e, StateList * list);

err_t activate_aura(Entity *e, StateList * list);

err_t sentinel_check(Entity *e);

#endif