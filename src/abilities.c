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
    return FALSE;
}

bool Focus_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}

bool Trap_fn(Coord c, Entity * e, StateList * list)
{
    return FALSE;
}

bool Banner_fn(Coord c, Entity * e, StateList * list)
{
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