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
#include "characters.h"
#include "deplacement.h"
#include "text.h"
#include "game_window.h"
#include "abilities.h"

bool is_online = FALSE;
bool turn_active = TRUE;
action turn_over = {0,{0,0},0};

err_t online_setup()
{
    is_online = TRUE;
    turn_active = FALSE;
    return OK;
}

err_t endgame_message(winId Id)
{
    if(Id == WIN)
    {
        addLog("YOU HAVE WON!");
    }
    else
    {
        addLog("YOU HAVE LOST!");
    }

    return OK;
}

bool your_turn()
{
    return turn_active;
}

err_t apply_movement(action a)
{
    Entity * e = e_from_id(a.char_id);
    int matrice[_X_SIZE_][_Y_SIZE_];
    Coord tab[_X_SIZE_ * _Y_SIZE_];

    simple_move(e, pathfinding((int(*)[_X_SIZE_])fill_tiles(e -> coords, matrice, e -> stat_mods[mv]), tab, a.c ));

    e->coords = a.c;
    e->act_points--;
    return OK;
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
    
    if(active_ent->cha_class->cla_id == Mage)
    {
        update_mage(active_ent, a.act);
    }

    active_ab = active_ent->cha_class->cla_abilities[a.act%NUM_AB];

    if(verbose>=1)printf("\n\n%s has chosen to %s at the following coordinates : %d,%d\n", active_ent->cha_name, active_ab.eng.name, a.c.x, a.c.y);

    sprintf(log, "%s cast %s", active_ent->cha_name, active_ab.eng.name);
    addLog(log);

    if(active_ab.fn_use==BEFORE)
    {
        if(verbose>=2)printf("Function use : BEFORE\n");
        death_count += active_ab.function(a.c, active_ent, list);
    }

    if(!active_ent->status_effect[Blessed])
    {
        if(verbose>=1)printf("Setting %s's cooldown of %d\n", active_ab.eng.name, active_ab.ab_cost);
        active_ent->ab_cooldown[a.act%NUM_AB] = active_ab.ab_cooldown;
    }
    else
    {
        if(verbose>=1)printf("%s is Blessed!\n", active_ent->cha_name);
    }
    
    if(active_ab.fn_use!=ONLY)
    {
        if(verbose>=2)printf("Function use != ONLY\n");
        death_count += apply_to(active_ab, active_ent, list, a.c);
    }

    if(active_ab.fn_use>=ONLY)
    {
        if(verbose>=2)printf("Function use : AFTER/ONLY\n");
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
    Sentinel_counter = TRUE;

    if(e[Angel].active!=Dead)
    {
        activate_aura(&e[Angel], stReceived);
    }

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

err_t test_turn()
{
    turn_end(Allies, stReceived);
    turn_start(Foes);
    turn_end(Foes, stSent);
    turn_start(Allies);
    return OK;
}

err_t set_endturn()
{
    if(turn_active)
    {
        if(is_online)
        {
            turn_active = FALSE;
        }
        else
        {
            test_turn();
        }
    }
    else
    {
        addLog("It is not your turn");
    }

    return OK;
}

bool play_check(Entity *e)
{
    selected_ability = -1;

    if(e->act_points>0)
    {
        return TRUE;
    }
    else
    {
        unselect();
        Entity * all;
        get_team(e, &all, TRUE);
        int i;
        for(i=0; i<NUM_CLASS; i++)
        {
            if(all[i].act_points>0)
            {
                return TRUE;
            }
        }
    }
    
    set_endturn();

    return FALSE;
}

err_t action_set(action a)
{
    if(verbose>=0)printf("Application de l'action...\n");

    if(is_online)
    {
        sendStruct(&a, sizeof(action), socketConnected);
    }

    if(a.act == Mvt)
    {
        apply_movement(a);
    }
    else
    {
        apply_action(a);
    }

    play_check(&Allies[a.char_id-1]);

    if(verbose>=0)printf("Action appliquee...\n");

    return OK;    
}

err_t local_turn()
{   
    addLog("It's your turn");

    turn_start(Allies);

    turn_active = TRUE;

    while(turn_active);

    turn_end(Allies, stReceived);

    sendStruct(&turn_over, sizeof(action), socketConnected);

    addLog("Your turn is over");

    return OK;
}

err_t opposing_turn()
{

    turn_start(Foes);
    
    action a;

    rec_id_swap(recep(&a, sizeof(action), socketConnected));

    while(a.char_id != 0)
    {
        if(a.act == Mvt)
        {
            apply_movement(a);
        }
        else
        {
            apply_action(a);
        }
        
        rec_id_swap(recep(&a, sizeof(action), socketConnected));
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
    Coord spawn[NUM_CLASS] = {{0,0},{1,3},{3,1},{1,7},{4,4},{7,1}};

    init_Foes(W);
    init_Allies(spawn,S);

    return game_loop(opposing_turn,local_turn);
}

winId init_server()
{
    Coord spawn[NUM_CLASS] = {{29,29},{26,28},{28,26},{22,28},{25,25},{28,22}};

    init_Allies(spawn,W);
    init_Foes(S);
    
    return game_loop(local_turn,opposing_turn);
}
