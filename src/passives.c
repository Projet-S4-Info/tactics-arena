#include "gameplay.h"
#include "init_classes.h"
#include "init.h"
#include "display.h"
#include "border.h"
#include "text.h"

int Bloodlust_counter;
bool Sentinel_counter;

err_t activate_bloodlust(Entity *e, StateList * list)
{
    if(verbose>=1)printf("Bloodlust was triggered!\n");

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
    if(verbose>=1)printf("%s\n",log);

    apply_to(Aura_ab, e, list, e->coords);

    return OK;
}

bool sentinel_check(Entity *e)
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

        int sight = get_range(r, Bolt);

        Coord t[MAXRANGE];

        setActionBorder(r->coords,sight,t);
        
        if(isInRange(t, e->coords))
        {
            char log[STR_LONG];
            sprintf(log, "%s's Sentinel was triggered", r->cha_name);
            if(verbose>=1)printf("%s\n",log);
            addLog(log);

            Sentinel_counter = FALSE;

            if(apply_to(r->cha_class->cla_abilities[Bolt%NUM_AB], r, list, e->coords) != 0)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}