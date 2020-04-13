#include "struct.h"
#include "init.h"
#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "passives.h"
#include "gameplay.h"
#include "grid.h"
#include "servFcnt.h"
#include "display.h"

bool turn_active = TRUE;
const action turn_over = {0,{0,0},0};

bool your_turn()
{
    return turn_active;
}

err_t apply_action(action a)
{
    Entity * active_ent;
    Ability active_ab;
    int death_count = 0;
    StateList * list;

    char log[STR_LONG];

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

    sprintf(log, "%s cast %s", active_ent->cha_name, active_ab.eng.name);
    addLog(log);

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

    if(e[Angel].active!=Dead)
    {
        activate_aura(&e[Angel], stReceived);
    }

    return OK;
}

err_t turn_end(Entity *e, StateList * list)
{
    //sendStruct(&turn_over, sizeof(action), );

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
    
    turn_active = FALSE;

    return NULL;
}

err_t action_set(action a)
{
    //sendStruct(&a, sizeof(action), );

    if(a.act == Mvt)
    {
        //Mvt function
    }
    else
    {
        apply_action(a);
    }

    play_check(&Allies[a.char_id-1]);

    return OK;    
}

err_t local_turn()
{   
    addLog("It's your turn");

    turn_start(Allies);

    turn_active = TRUE;

    while(turn_active);

    turn_end(Allies, stReceived);

    addLog("Your turn is over");

    return OK;
}

err_t opposing_turn()
{

    turn_start(Foes);
    
    action a;

    //recep(&a, sizeof(action), );

    while(a.char_id != 0)
    {
        if(a.act == Mvt)
        {
            //Mvt function
        }
        else
        {
            apply_action(a);
        }
        
        //recep(&a, sizeof(action), );
    }

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