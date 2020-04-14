#include "gameplay.h"
#include "init_classes.h"
#include "init.h"
#include "display.h"

int Bloodlust_counter;
bool Sentinel_counter;

err_t activate_bloodlust(Entity *e, StateList * list)
{
    if(verbose)printf("Bloodlust was triggered!\n");

    char log[STR_LONG];
    sprintf(log, "%s's Bloodlust was triggered", e->cha_name);
    addLog(log);

    e->stat_mods[atk] *= 2;

    Status v = {e->stat_mods[atk]/2,atk,1};
    
    Bloodlust_counter = -1;

    return list_add(list, v, e);
}

err_t activate_aura(Entity *e, StateList *list)
{   
    char log[STR_LONG];
    sprintf(log, "%s's Aura was triggered", e->cha_name);
    addLog(log);

    apply_to(Aura_ab, e, list, e->coords);

    return OK;
}

err_t sentinel_check(Entity *e)
{
    if(Sentinel_counter)
    {
        Entity * r;
        StateList * list;

        if(same_team(e,Allies))
        {
            r = &Foes[Ranger];
            list = stReceived;
        }
        else
        {
            r = &Allies[Ranger];
            list = stSent;
        }

        int sight = get_range(r->stat_mods[vis], r->cha_class->cla_abilities->range);

        Coord t[MAXRANGE];

        setActionZone(r->coords,sight,t);
        
        if(isInRange(t, e->coords))
        {
            char log[STR_LONG];
            sprintf(log, "%s's Sentinel was triggered", r->cha_name);
            addLog(log);

            apply_to(r->cha_class->cla_abilities[Bolt%NUM_AB], r, list, e->coords);
            Sentinel_counter = FALSE;
        }
    }
    return OK;
}