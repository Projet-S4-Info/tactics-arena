#ifndef gameplay_h
#define gameplay_h
#include "struct.h"

int get_range(int vision, int range_mod);
char * get_desc(Entity * e, abilityId ab_id);
bool able_ability(Entity *e, abilityId ab_id);
bool same_team(Entity *a, Entity *b);
Coord add_coords(Coord a, Coord b);
Coord compare_coords(Coord a, Coord b);
bool death_check(Entity * e);
err_t reset_cooldowns(Entity * e);

err_t new_death(Entity * e);
bool apply_damage(Damage * d, Entity * caster, Entity * target);

err_t remove_mod(Status * stat, Entity * e);
Status * renew_mod(Entity * e, statusId status);

err_t apply_stat_change(Status s, Entity * target, StateList * list);
err_t apply_status(Status s, Entity *target, StateList *list, int caster_id);
err_t apply_mod(Modifier m, Entity * target, StateList * list, int caster_id);

int apply_to(Ability active_ab, Entity * active_ent, StateList * list, Coord starting_point);

#endif