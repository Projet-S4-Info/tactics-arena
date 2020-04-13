#ifndef gameplay_h
#define gameplay_h
#include "struct.h"

err_t get_team(Entity *e, Entity **all, bool same);
bool game_over();
int get_range(int vision, int range_mod);
char * get_desc(Entity * e, abilityId ab_id);
bool able_ability(Entity *e, abilityId ab_id);
bool same_team(Entity *a, Entity *b);
bool closer_coords(Coord a, Coord b); //Utile : return true si a est plus proche de 0,0 que de la coordonnées b
Coord add_coords(Coord a, Coord b);  //Utile : Additionne les coordonneés
Coord compare_coords(Coord a, Coord b); //Utile : Soustrait les coordonées 
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

int isInRange(Coord coorTab[], Coord attack);
int setActionZone(Coord perso, int actionRange, Coord coorTab[]);

#endif