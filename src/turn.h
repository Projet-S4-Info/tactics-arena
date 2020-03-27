#ifndef turn_h
#define turn_h
#include "struct.h"

Coord add_coords(Coord a, Coord b);
bool apply_damage(Damage * d, Entity * caster, Entity * target);
err_t apply_mod(Modifier m, Entity * target, StateList * list, int caster_id);
err_t remove_mod(Status * stat, int cha_id);
err_t new_death(Entity * e);
err_t apply_action(action a);
Entity * play_check(Entity *E);
err_t turn();

#endif