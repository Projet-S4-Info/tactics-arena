#include "struct.h"
#include "init.h"
#include "grid.h"
#include <stdlib.h>

Coord add_coords(Coord a, Coord b)
{
    Coord c;

    c.x = a.x + b.x;
    c.y = a.y + b.y;

    return c;
}

bool apply_damage(Damage * d, Entity * caster, Entity * target)
{
    float crippled = target->status_effect[Cripple] == 1 ? 1.75 : 1;

    if(caster->status_effect[Piercing])
    {
        target->stat_mods[pv] -= (caster->stat_mods[d->type] * d->multiplier * crippled)+0.4;
    }
    else
    {
        target->stat_mods[pv] -= (caster->stat_mods[d->type]/(1+(target->stat_mods[d->type+2]/15)) * d->multiplier * crippled)+0.4;
    }

    //UPDATE HEALTH VISUALLY

    if(target->stat_mods[pv]<=0)
    {
        target->active = Dead;
        return TRUE;
    }
    else
        return FALSE;
}

err_t apply_mod(Modifier m, Entity * target, StateList * list, int caster_id)
{
    if(m.chance*100>=(rand()%100+1))
    {
        if(m.effect.value==0)
        {
            if(m.effect.stat==Provoked)
            {
                target->status_effect[m.effect.stat] = caster_id;
            }
            else
            {
                target->status_effect[m.effect.stat] = 1;
            }
        }
        else
        {
            target->stat_mods[m.effect.stat] += m.effect.value;
            target->stat_mods[m.effect.stat] = target->stat_mods[m.effect.stat]<0 ? 0 : target->stat_mods[m.effect.stat];
        }
        
    }
    if(m.effect.duration!=0)
    {
    return list_add(list, &m.effect, target->cha_id);
    }
    else return OK;
}

err_t remove_mod(Status * stat, int cha_id)
{
    Entity * e = cha_id<0 ? &Foes[cha_id*-1-1] : &Allies[cha_id-1];

    if(stat->value==0)
    {
        e->status_effect[stat->stat] = 0;
    }
    else
    {
        e->stat_mods[stat->stat] += stat->value *-1;
    }

    return OK;
}

err_t new_death(Entity * e)
{
    StateList * list = e->cha_id<0 ? stFoe : stAlly;

    int i;
    for(i=0; i<NUM_AB; i++)
    {
        e->ab_cooldown[i] = 0;
    }

    start_list(list);
    while(list_search(list,e->cha_id))
    {
        list_remove(list);
    }

    //PLAY DEATH ANIMATION

    return OK;
}

err_t apply_action(action a)
{
    Entity * active_ent;
    Ability active_ab;
    Entity * e;
    int death_count=0;
    Entity * morts[6];
    if(a.char_id<0)
        active_ent = &Foes[a.char_id*-1-1];
    else
        active_ent = &Allies[a.char_id-1];

    active_ab = active_ent->cha_class->cla_abilities[a.act%NUM_AB];

    //ANIMATE THE ACTION

    int i,j;
    for(i=0; i<active_ab.nb_coords; i++)
    {
        //e=getEntity(add_coords(a.c, active_ab.coord[i]));
        if(e!=NULL&&(e->cha_id!=Trap+1&&e->cha_id!=Trap*-1+1))
        {
            if(e->cha_id<0)
                if(active_ab.damage!=NULL)
                    if(apply_damage(active_ab.damage, active_ent, e))
                    {
                        morts[death_count++] = e;
                    }

            if(active_ab.mods!=NULL&&e->active!=Dead)
                for(j=0; j<active_ab.nb_mods; j++)
                {
                    if(e->cha_id<0&&active_ab.mods[j].t!=ALLIES)
                        apply_mod(active_ab.mods[j],e, stFoe, a.char_id);

                    else if((e->cha_id>0&&active_ab.mods[j].t!=FOES))
                        apply_mod(active_ab.mods[j],e, stAlly, a.char_id);
                }
        }
    }
    
    
    if(active_ab.function!=NULL)
        active_ab.function(a);
    
    if(!active_ent->status_effect[Blessed])
        active_ent->ab_cooldown[a.act%NUM_AB] = active_ab.ab_cooldown;

    for(i=0; i<death_count; i++)
        new_death(morts[i]);

    return OK;
}

Entity * play_check(Entity *E)
{
    int current = E->cha_id-1;
    Entity * F = E - current;
    int i = current;
    do
    {
        if((F[i].active)&&(F[i].act_points>0))
            return F + i;
        else
            i = i==NUM_CLASS-1 ? 0 : i+1;    
        
    } while(i!=current);
    
    return NULL;
}

err_t turn()
{
    //check ennemy statelist

    Entity * active_ent=Allies;
    action a;
    while((active_ent=play_check(active_ent))!=NULL)
    {
        //while action selected is change character
            //Select entity pointed to by active_ent
            //Wait for action selection
            //Wait for coordinate selection

        //relay action information
        //wait for confirmation
        if(a.act<0);
            //call mvt function (dont forget to remove semicolon after if)
        else
            apply_action(a);
        
    }

    //check local statelist

    return OK;
}

