#ifndef test_h
#define test_h
#include "struct.h"

err_t print_class_name(classId class, char tab[STR_SHORT]);
err_t print_int(int * i, char tab[STR_SHORT]);
err_t print_abilityId(abilityId id, char tab[STR_SHORT]);
err_t print_targetType(targetType target, char tab[STR_SHORT]);
err_t print_statId(statId id, char tab[STR_SHORT]);
err_t print_statusId(statusId id, char tab[STR_SHORT]);
err_t print_fnid(fnid id,  char tab[STR_SHORT]);
err_t print_Damage(Damage **d, char tab[STR_SHORT]);
err_t print_Coord(Coord * c, char tab[STR_SHORT]);
err_t print_Coord_list(Coord ** c, int nb, char tab[STR_SHORT]);
err_t print_action(action *a, char tab[STR_SHORT]);
err_t print_Status(Status s, char tab[STR_SHORT]);
err_t print_Modifier(Modifier *m, char tab[STR_SHORT]);
err_t print_Modifier_list(Modifier **m, int nb, char tab[STR_SHORT]);
err_t print_init_ent(init_ent *e, char tab[STR_SHORT]);
err_t print_Ability(Ability *a, char tab[STR_SHORT]);
err_t print_StateList(StateList * list, char tab[STR_SHORT]);
err_t print_grid(Coord tab[]);

#endif