#include "struct.h"
#include "init.h"
#include "grid.h"
#include <stdlib.h>
#include <stdio.h>

Coord add_coords(Coord a, Coord b)
{
    Coord c;

    c.x = a.x + b.x;
    c.y = a.y + b.y;

    printf("Coords a(%d,%d) and b(%d,%d) added to give c(%d,%d)\n", a.x, a.y, b.x, b.y, c.x, c.y);

    return c;
}

bool apply_damage(Damage * d, Entity * caster, Entity * target)
{
    if(target->cha_id==Goliath)
    {
        int block = target->status_effect[Guarding] == 1 ? 70 : 30;

        printf("Block chance : %d\n", block);

        if(block>=(rand()%100+1))
        {
            printf("Block Successful!\n");
            //PLAY ANIMATION
            return FALSE;
        }
        else
        {
            printf("Block Failed!\n");
        }
        

    }

    float crippled = target->status_effect[Cripple] == 1 ? 1.75 : 1;

    if(crippled==1.75)
        printf("%s is crippled!\n", target->cha_name);

    printf("%s's health before the attack : %d\n", target->cha_name, target->stat_mods[pv]);

    if(caster->status_effect[Piercing])
    {
        printf("%s has piercing!\n", caster->cha_name);
        target->stat_mods[pv] -= (caster->stat_mods[d->type] * d->multiplier * crippled)+0.4;
    }
    else
    {
        target->stat_mods[pv] -= (caster->stat_mods[d->type]/(1+(target->stat_mods[d->type+2]/15)) * d->multiplier * crippled)+0.4;
    }

    printf("%s's health after the attack : %d\n", target->cha_name, target->stat_mods[pv]);

    //UPDATE HEALTH VISUALLY

    if(target->stat_mods[pv]<=0)
    {
        printf("%s was killed!\n", target->cha_name);
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
        printf("Modifier landed!\n");

        if(m.effect.value==0)
        {
            printf("Modifier is a status effect!\n");
            if(m.effect.stat==Provoked)
            {
                target->status_effect[m.effect.stat] = caster_id;
            }
            else
            {
                target->status_effect[m.effect.stat] = 1;
            }
            printf("Status effect : %d", target->status_effect[m.effect.stat]);
        }
        else
        {
            printf("Modifier is a stat change of %d\n", m.effect.value);
            printf("Stat before the change : %d\n", target->stat_mods[m.effect.stat]);
            target->stat_mods[m.effect.stat] += m.effect.value;
            target->stat_mods[m.effect.stat] = target->stat_mods[m.effect.stat]<0 ? 0 : target->stat_mods[m.effect.stat];
            printf("Stat after the change : %d\n", target->stat_mods[m.effect.stat]);
        }

        if(m.effect.duration!=0)
        {
        printf("The mod will last %d turns!\n", m.effect.duration);
        return list_add(list, &m.effect, target->cha_id);
        }
        else
            printf("The mod is permanent!\n");
        
    }
    else
    {
        printf("Modifier not landed!\n");
    }
    
    return OK;
}

err_t remove_mod(Status * stat, int cha_id)
{
    Entity * e = cha_id<0 ? &Foes[cha_id*-1-1] : &Allies[cha_id-1];

    if(stat->value==0)
    {
        printf("Modifier to remove is a status effect!\n");
        e->status_effect[stat->stat] = 0;
    }
    else
    {
        printf("Modifier to remove is a stat change of a value of %d\n", stat->value);
        printf("Stat before the change : %d\n", e->stat_mods[stat->stat]);
        e->stat_mods[stat->stat] += stat->value *-1;
        printf("Stat after the change : %d\n", e->stat_mods[stat->stat]);
    }

    return OK;
}

err_t new_death(Entity * e)
{
    int i;
    for(i=0; i<NUM_AB; i++)
    {
        e->ab_cooldown[i] = 0;
        e->stat_mods[pv]=20;
    }

    start_list(stSent);
    while(list_search(stSent,e->cha_id))
    {
        list_remove(stSent);
    }

    start_list(stReceived);
    while(list_search(stReceived,e->cha_id))
    {
        list_remove(stReceived);
    }

    //PLAY DEATH ANIMATION

    printf("%s has been killed!\n", e->cha_name);

    return OK;
}

err_t apply_action(action a)
{
    Entity * active_ent;
    Ability active_ab;
    Entity * e;
    int death_count=0;
    Entity * morts[6];
    StateList * list;

    if(a.char_id<0)
    {
        active_ent = &Foes[a.char_id*-1-1];
        list = stReceived;
    }
    else
    {
        active_ent = &Allies[a.char_id-1];
        list = stSent;
    }
    active_ab = active_ent->cha_class->cla_abilities[a.act%NUM_AB];

    //ANIMATE THE ACTION

    printf("\n\n%s has chosen to %s at the following coordinates : %d,%d\n", active_ent->cha_name, active_ab.eng.name, a.c.x, a.c.y);

    if(active_ab.only_fn!=TRUE)
    {
        printf("only_fn = TRUE\n");
        int i,j;

        for(i=0; i<active_ab.nb_coords; i++)
        {
            //e=getEntity(add_coords(a.c, active_ab.coord[i]));
            if(e!=NULL&&(e->cha_id!=Trap+1&&e->cha_id!=(Trap*-1)+1))
            {
                printf("%s was found in the zone!\n", e->cha_name);

                if(e->cha_id<0)
                {
                    printf("%s is an Ennemy!\n", e->cha_name);
                    if(active_ab.damage!=NULL)
                        if(apply_damage(active_ab.damage, active_ent, e))
                        {
                            morts[death_count++] = e;
                        }
                }
                else
                {
                    printf("%s is an Ally!\n", e->cha_name);
                }

                if(active_ab.mods!=NULL&&e->active!=Dead)
                    for(j=0; j<active_ab.nb_mods; j++)
                    {
                        if(e->cha_id<0&&active_ab.mods[j].t!=ALLIES)
                            apply_mod(active_ab.mods[j],e, list, a.char_id);

                        else if((e->cha_id>0&&active_ab.mods[j].t!=FOES))
                            apply_mod(active_ab.mods[j],e, list, a.char_id);
                    }
            }
        }

        for(i=0; i<death_count; i++)
            new_death(morts[i]);
    }
    else
    {
        printf("only_fn = FALSE\n");
    }
    

    if(active_ab.function!=NULL)
        active_ab.function(a);
    
    if(!active_ent->status_effect[Blessed])
    {
        active_ent->ab_cooldown[a.act%NUM_AB] = active_ab.ab_cooldown;
    }
    else
    {
        printf("%s is Blessed!\n", active_ent->cha_name);
    }

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
    //check sent statelist
    //decrease cooldowns
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

    //check for local burns
    //check received statelist

    return OK;
}

