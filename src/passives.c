#include "gameplay.h"
#include "init_classes.h"
#include "init.h"
#include "display.h"

int Bloodlust_counter;

err_t activate_bloodlust(Entity *e, StateList * list)
{
    if(verbose)printf("Bloodlust was triggered!\n");
    e->stat_mods[atk] *= 2;

    Status v = {e->stat_mods[atk]/2,atk,1};
    
    Bloodlust_counter = -1;

    return list_add(list, v, e);
}

err_t activate_aura(Entity *e, StateList *list)
{
    apply_to(Aura_ab, e, list, e->coords);
    return OK;
}

err_t sentinel_check(Entity *e)
{
    Entity * r;
    StateList * list;

    if(e->cha_id<0)
    {
        r = &Allies[Ranger];
        list = stSent;
    }
    else
    {
        r = &Foes[Ranger];
        list = stReceived;
    }

    int sight = get_range(r->stat_mods[vis], r->cha_class->cla_abilities->range);

    Coord t[MAXRANGE];

    setActionZone(r->coords,sight,t);
    
    if(isInRange(t, e->coords))
    {
        apply_to(r->cha_class->cla_abilities[Bolt%NUM_AB], r, list, e->coords);
    }

    return OK;
}