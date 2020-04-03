#include "struct.h"
#include "init.h"
#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "passives.h"
#include "gameplay.h"

err_t apply_action(action a)
{
    Entity * active_ent;
    Ability active_ab;
    int death_count = 0;
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

    if(verbose)printf("\n\n%s has chosen to %s at the following coordinates : %d,%d\n", active_ent->cha_name, active_ab.eng.name, a.c.x, a.c.y);

    if(active_ab.fn_use==AFTER)
    {
        apply_to_ent(a,active_ab,active_ent,list,&death_count,standard_apply,NULL);
    }
    else if(active_ab.fn_use!=ONLY)
    {
        apply_to_ent(a,active_ab,active_ent,list,&death_count,standard_apply,active_ab.function);
    }

    if(active_ab.fn_use>=ONLY)
        if(active_ab.function(a.c, active_ent, list))
            death_count++;

    if(abs(active_ent->cha_id)-1==Berserker && Bloodlust_counter!=-1)
    {
        Bloodlust_counter += death_count;
    }
    
    if(!active_ent->status_effect[Blessed])
    {
        active_ent->ab_cooldown[a.act%NUM_AB] = active_ab.ab_cooldown;
    }
    else
    {
        if(verbose)printf("%s is Blessed!\n", active_ent->cha_name);
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

err_t local_turn()
{
    //check sent statelist
    //decrease cooldowns

    Entity * active_ent=Allies;
    action a;
    Bloodlust_counter = 0;

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

        if(Bloodlust_counter == 2)
        {
            activate_bloodlust(active_ent, stSent);
        }
    }

    //check for local burns
    //check received statelist

    return OK;
}