#include "init.h"
#include "grid.h"

bool same_team(Entity *a, Entity *b)
{
    if(a->cha_id<0&&b->cha_id<0)
    {
        return TRUE;
    }
    else if(a->cha_id>0&&b->cha_id>0)
    {
        return TRUE;
    }else
    {
        return FALSE;
    }
}

err_t apply_to_ent(action a, Ability active_ab, Entity * active_ent, StateList * list, int * deaths, bool (*standard)(Coord, Ability, Entity *,StateList *), bool (*fn)(Coord, Entity *, StateList *) )
{
    int i;
    Coord c;

    for(i=0; i<active_ab.nb_coords; i++)
    {
        //c=add_coords(a.c, active_ab.coord[i])
        if(standard!=NULL)
        {
            if(standard(c,active_ab,active_ent,list))
            {
                (*deaths)++;
            }
        }
        if(fn!=NULL)
        {
            if(fn(c,active_ent,list))
            {
               (*deaths)++; 
            }
        }
    }

    return OK;
}

Coord add_coords(Coord a, Coord b)
{
    Coord c;

    c.x = a.x + b.x;
    c.y = a.y + b.y;

    if(verbose)printf("Coords a(%d,%d) and b(%d,%d) added to give c(%d,%d)\n", a.x, a.y, b.x, b.y, c.x, c.y);

    return c;
}

err_t remove_mod(Status * stat, Entity * e)
{

    if(stat->value==0)
    {
        if(verbose)printf("Modifier to remove is a status effect!\n");
        e->status_effect[stat->stat] = 0;
    }
    else
    {
        if(verbose)printf("Modifier to remove is a stat change of a value of %d\n", stat->value);
        if(verbose)printf("Stat before the change : %d\n", e->stat_mods[stat->stat]);
        e->stat_mods[stat->stat] += stat->value *-1;
        if(verbose)printf("Stat after the change : %d\n", e->stat_mods[stat->stat]);
    }

    return OK;
}

err_t new_death(Entity * e)
{
    int i;
    for(i=0; i<NUM_AB; i++)
    {
        e->ab_cooldown[i] = 0;
    }

    e->active = Dead;
    e->stat_mods[pv]=20;

    Status * v;

    start_list(stSent);
    while((v = list_search(stSent,e))!=NULL)
    {
        remove_mod(v, e);
        list_remove(stSent);
    }

    start_list(stReceived);
    while((v = list_search(stReceived,e))!=NULL)
    {
        remove_mod(v, e);
        list_remove(stReceived);
    }

    //PLAY DEATH ANIMATION

    if(verbose)printf("%s has been killed!\n", e->cha_name);

    return OK;
}

bool apply_damage(Damage * d, Entity * caster, Entity * target)
{
    if(target->cha_id==Goliath)
    {
        int block = target->status_effect[Guarding] == 1 ? 70 : 30;

        if(verbose)printf("Block chance : %d\n", block);

        if(block>=(rand()%100+1))
        {
            if(verbose)printf("Block Successful!\n");
            //PLAY ANIMATION
            return FALSE;
        }
        else
        {
            if(verbose)printf("Block Failed!\n");
        }
        

    }

    float crippled = target->status_effect[Cripple] == 1 ? 1.75 : 1;

    if(crippled==1.75)
        if(verbose)printf("%s is crippled!\n", target->cha_name);

    if(verbose)printf("%s's health before the attack : %d\n", target->cha_name, target->stat_mods[pv]);

    if(caster->status_effect[Piercing])
    {
        if(verbose)printf("%s has piercing!\n", caster->cha_name);
        target->stat_mods[pv] -= (caster->stat_mods[d->type] * d->multiplier * crippled)+0.4;
    }
    else
    {
        target->stat_mods[pv] -= (caster->stat_mods[d->type]/(1+(target->stat_mods[d->type+2]/15)) * d->multiplier * crippled)+0.4;
    }

    if(verbose)printf("%s's health after the attack : %d\n", target->cha_name, target->stat_mods[pv]);

    //UPDATE HEALTH VISUALLY

    if(target->stat_mods[pv]<=0)
    {
        if(verbose)printf("%s was killed!\n", target->cha_name);
        new_death(target);
        return TRUE;
    }
    else
        return FALSE;
}

err_t apply_mod(Modifier m, Entity * target, StateList * list, int caster_id)
{
    if(m.chance*100>=(rand()%100+1))
    {
        if(verbose)printf("Modifier landed!\n");

        if(m.effect.value==0)
        {
            if(verbose)printf("Modifier is a status effect!\n");
            if(m.effect.stat==Provoked)
            {
                target->status_effect[m.effect.stat] = caster_id;
            }
            else
            {
                target->status_effect[m.effect.stat] = 1;
            }
            if(verbose)printf("Status effect : %d", target->status_effect[m.effect.stat]);
        }
        else
        {
            if(verbose)printf("Modifier is a stat change of %d\n", m.effect.value);
            if(verbose)printf("Stat before the change : %d\n", target->stat_mods[m.effect.stat]);
            target->stat_mods[m.effect.stat] += m.effect.value;
            target->stat_mods[m.effect.stat] = target->stat_mods[m.effect.stat]<0 ? 0 : target->stat_mods[m.effect.stat];
            target->stat_mods[m.effect.stat] = target->stat_mods[m.effect.stat]>20 ? 20 : target->stat_mods[m.effect.stat];
            if(verbose)printf("Stat after the change : %d\n", target->stat_mods[m.effect.stat]);
        }

        if(m.effect.duration!=0)
        {
        if(verbose)printf("The mod will last %d turns!\n", m.effect.duration);
        return list_add(list, m.effect, target);
        }
        else
           if(verbose) printf("The mod is permanent!\n");
        
    }
    else
    {
        if(verbose)printf("Modifier not landed!\n");
    }
    
    return OK;
}

bool standard_apply(Coord c, Ability active_ab, Entity * active_ent, StateList * list)
{
    bool death = FALSE;
    Entity * e;
    int j;

    //e=getEntity(c);
    if(e!=NULL&&(e->cha_id!=Trap+1&&e->cha_id!=(Trap*-1)+1))
    {
        if(verbose)printf("%s was found in the zone!\n", e->cha_name);

        if(!same_team(e,active_ent))
        {
            if(verbose)printf("%s is an Ennemy!\n", e->cha_name);
            if(active_ab.damage!=NULL)
                death = apply_damage(active_ab.damage, active_ent, e);

        }
        else
        {
            if(verbose)printf("%s is an Ally!\n", e->cha_name);
        }

        if(active_ab.mods!=NULL&&e->active!=Dead)
        {
            for(j=0; j<active_ab.nb_mods; j++)
            {
                if(!same_team(e,active_ent)&&active_ab.mods[j].t!=ALLIES)
                    apply_mod(active_ab.mods[j],e, list, active_ent->cha_id);

                else if((same_team(e,active_ent)&&active_ab.mods[j].t!=FOES))
                    apply_mod(active_ab.mods[j],e, list, active_ent->cha_id);
            }
        }

    }

    return death;
}