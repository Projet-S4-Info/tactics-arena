#include "struct.h"
#include "init.h"
#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "passives.h"
#include "gameplay.h"
#include "grid.h"

err_t apply_action(action a)
{
    Entity * active_ent;
    Ability active_ab;
    int death_count = 0;
    StateList * list;

    if(a.char_id<0)
    {
        active_ent = &Foes[(a.char_id*-1)-1];
        list = stReceived;
    }
    else
    {
        active_ent = &Allies[a.char_id-1];
        list = stSent;
    }
    active_ab = active_ent->cha_class->cla_abilities[a.act%NUM_AB];

    if(!active_ent->status_effect[Blessed])
    {
        active_ent->ab_cooldown[a.act%NUM_AB] = active_ab.ab_cooldown;
    }
    else
    {
        if(verbose)printf("%s is Blessed!\n", active_ent->cha_name);
    }

    //ANIMATE THE ACTION

    if(verbose)printf("\n\n%s has chosen to %s at the following coordinates : %d,%d\n", active_ent->cha_name, active_ab.eng.name, a.c.x, a.c.y);

    if(active_ab.fn_use==BEFORE)
    {
        death_count += active_ab.function(a.c, active_ent, list);
    }
    
    if(active_ab.fn_use!=ONLY)
    {
        death_count += apply_to(active_ab, active_ent, list, a.c);
    }

    if(active_ab.fn_use>=ONLY)
    {
        death_count += active_ab.function(a.c, active_ent, list);
    }

    if(abs(active_ent->cha_id)-1==Berserker && Bloodlust_counter!=-1)
    {
        Bloodlust_counter += death_count;

        if(Bloodlust_counter>=2)
        {
        activate_bloodlust(active_ent, list);
        }
    }

    active_ent->act_points -= active_ab.ab_cost;

    return OK;
}

err_t turn_start(Entity *e)
{
    int i;
    for(i=0; i<NUM_CLASS; i++)
    {
        if((e+i)->status_effect[Paralyzed])
        {
            (e+i)->act_points = 1;
        }
        else
        {
            (e+i)->act_points = 3;
        }

    }

    Bloodlust_counter = 0;

    return OK;
}

err_t turn_end(Entity *e, StateList * list)
{
    int i,j;
    for(i=0; i<NUM_CLASS; i++)
    {
        if((e+i)->status_effect[Burning])
        {
            e->stat_mods[pv]-=3;
            death_check(e);
        }

        for(j=0;j<NUM_AB; j++)
        {
            (e+i)->ab_cooldown[j] -= (e+i)->ab_cooldown[j] != 0 ? 1 : 0;
        }
    }

    List_Elem * elem;

    start_list(list);
    while(!(out_of_list(list)))
    {
        elem = list_change(list, -1);

        if(elem!=NULL)
        {
            list_remove(list);
            remove_mod(elem->value, elem->entity);
        }
        list_next(list);
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
        if((F[i].active) && (F[i].act_points>0) && !(F[i].status_effect[Freezing]))
        {
            setSelected((F+i)->coords);
            return F + i;
        }
        else
        {
            i = i==NUM_CLASS-1 ? 0 : i+1;  
        }  
        
    } while(i!=current);
    
    return NULL;
}

err_t local_turn()
{   

    Entity * active_ent = Allies;
    //action a;

    turn_start(Allies);
    
    if(Allies[Angel].active!=Dead)
    {
        activate_aura(&Allies[Angel], stSent);
    }

    while((active_ent=play_check(active_ent))!=NULL)
    {
            /*do
            {
                //Select entity pointed to by active_ent
                //wait for action selection
                //wait for coordinate selection
                a = fonction(Entity * e)
            }while(a.act<0);*/

        //relay action information & wait for confirmation

        /*if(a.act==0)
        {
            call mvt function
        }
        else
        {
            apply_action(a);
        }*/
    }

    turn_end(Allies, stReceived);

    return OK;
}

err_t opposing_turn()
{

    turn_start(Foes);

    if(Foes[Angel].active!=Dead)
    {
        activate_aura(&Foes[Angel], stReceived);
    }

    /*
    while(Wait for action & relay confirmation!=End Turn)
    {

    apply_action(a);

    }*/

    turn_end(Foes, stSent);

    return OK;
}

winId game_loop(err_t (*turn1)(void), err_t (*turn2)(void))
{
    winId game_end;

    while(TRUE)
    {
        turn1();

        if((game_end = game_over()))
            break;

        turn2();

        if((game_end = game_over()))
            break;
    }

    return game_end;
}

winId init_client()
{
    return game_loop(opposing_turn,local_turn);
}

winId init_server()
{
    return game_loop(local_turn,opposing_turn);
}