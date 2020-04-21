#ifndef gameplay_h
#define gameplay_h
#include "struct.h"

err_t rec_id_swap(action * a);
Entity * e_from_id(int Id);
err_t get_team(Entity *e, Entity **all, bool same);
winId game_over();
int get_range(Entity *e, abilityId ab);
char * get_desc(Entity * e, abilityId ab_id);
int get_cooldown(Entity * e, abilityId Id);
char * get_name(Entity * e, abilityId ab_id);
bool able_ability(Entity *e, abilityId ab_id, bool show_logs);
bool is_ally(Entity *e);
bool show(Entity * e);
bool same_team(Entity *a, Entity *b);
err_t end_Detain(Entity *e);
bool tile_type(Coord c, targetType targeting, Entity * e);
bool same_coord(Coord a, Coord b);
bool closer_coords(Coord a, Coord b);
Coord add_coords(Coord a, Coord b);
err_t free_spawn(Entity *e);
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