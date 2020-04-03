#include "gameplay.h"
#include "init_classes.h"

int Bloodlust_counter;

err_t activate_bloodlust(Entity *e, StateList * list)
{
    if(verbose)printf("Bloodlust was triggered!\n");
    e->stat_mods[atk] *= 2;

    Status v = {e->stat_mods[atk]/2,atk,1};
    
    Bloodlust_counter = -1;

    return list_add(list, v, e);
}

err_t activate_aura(Entity *e)
{
    int i;
    Entity * active;

    for(i=0; i<103; i++)
    {
        //active=getEntity(add_coords(e->coords, *(aoe103+i)));

        if(active!=NULL&&same_team(e,active))
        {
            active->stat_mods[pv] = active->stat_mods[pv]==20 ? 20 : active->stat_mods[pv] + 5;
            printf("%s was healed by %s's Aura!\n", active->cha_name, e->cha_name);
        }

    return OK;
}