#ifndef gameplay_h
#define gameplay_h
#include "struct.h"

bool same_team(Entity *a, Entity *b);
Coord add_coords(Coord a, Coord b);
bool apply_damage(Damage * d, Entity * caster, Entity * target);
err_t apply_mod(Modifier m, Entity * target, StateList * list, int caster_id);
err_t remove_mod(Status * stat, int cha_id);
err_t new_death(Entity * e);
err_t apply_to_ent(action a, Ability active_ab, Entity * active_ent, StateList * list, int * deaths, bool (*standard)(Coord, Ability, Entity *,StateList *), bool (*fn)(Coord, Entity *, StateList *));
bool standard_apply(Coord c, Ability active_ab, Entity * active_ent, StateList * list);


#endif