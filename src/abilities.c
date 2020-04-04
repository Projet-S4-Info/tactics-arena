#include "struct.h"
#include "init.h"
#include "gameplay.h"
#include "grid.h"

bool Killing_Blow_fn(Coord c, Entity * e, StateList * list)
{
    Status v = {3,atk,0};
    Entity * t;
    //t = getEntity(c);
    if(t!=NULL)
    {
        if(t==Dead)
        {
            apply_stat_change(v, t, list);
            e->act_points+=1;
        }
    }
    return FALSE;
}

bool Fury_fn(Coord c, Entity * e, StateList * list)
{
    int turns = 0;
    Status * v;

    start_list(stSent);
    while((v = list_search(stSent, e, -1)) !=NULL)
    {
        if(!list_check(stSent))
        {
            turns += v->duration;
            remove_mod(v,e);
            list_remove(stSent);
        }
        list_next(stSent);
    }

    start_list(stReceived);
    while((v = list_search(stReceived, e, -1)) !=NULL)
    {
        if(!list_check(stReceived))
        {
            turns += v->duration;
            remove_mod(v,e);
            list_remove(stReceived);
        }
        list_next(stReceived);
    }

    Status d = {turns, atk, 0};

    apply_stat_change(d, e, list);

    return FALSE;
}

bool Focus_fn(Coord c, Entity * e, StateList * list)
{
    Status * v;

    start_list(stSent);
    while((v = list_search(stSent, e, -1)) !=NULL)
    {
        if(list_check(stSent))
        {
            list_change(stSent, 2);
        }
        else
        {
            if(list_change(stSent, -1)!=NULL)
            {
                remove_mod(v,e);
                list_remove(stSent);
            }
        }
        list_next(stSent);
    }

    start_list(stReceived);
    while((v = list_search(stReceived, e, -1)) !=NULL)
    {
        if(list_check(stReceived))
        {
            list_change(stReceived, 2);
        }
        else
        {
            if(list_change(stReceived, -1)!=NULL)
            {
                remove_mod(v,e);
                list_remove(stReceived);
            }
        }
        list_next(stReceived);
    }

    return FALSE;
}

bool Trap_fn(Coord c, Entity * e, StateList * list)
{
    //VOIR AVEC THIBAULT
    return FALSE;
}

bool Banner_fn(Coord c, Entity * e, StateList * list)
{
    Entity * all;
    if(e->cha_id<0)
    {
        all = Foes;
    }
    else
    {
        all = Allies;
    }

    int i;
    for(i=0; i<NUM_CLASS; i++)
    {
        reset_cooldowns(all+i);
    }

    return FALSE;
}

bool mage_switch(Coord c, Entity * e, StateList * list)
{
    
    return FALSE;
}

bool FlameCharge_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}

bool Flare_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}

bool Blizzard_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}

bool Volt_Switch_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}

bool Lightning_Chain_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}

bool Thrust_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}

bool Life_Transfer_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}

bool Gates_of_Valhalla_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}

bool Last_Sacrfice_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}