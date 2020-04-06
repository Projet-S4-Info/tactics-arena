#include "struct.h"
#include "init.h"
#include "gameplay.h"
#include "grid.h"
#include "graphics.h"

int Killing_Blow_fn(Coord c, Entity * e, StateList * list)
{
    Status v = {3,atk,0};
    Entity * t;
    t = getEntity(matrix, c);
    if(t!=NULL)
    {
        if(t==Dead)
        {
            apply_stat_change(v, t, list);
            e->act_points+=1;
        }
    }
    return 0;
}

int Fury_fn(Coord c, Entity * e, StateList * list)
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

    return 0;
}

int Frenzied_Dash_fn(Coord c, Entity * e, StateList * list)
{
    
    return 0;
}

int Focus_fn(Coord c, Entity * e, StateList * list)
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

    return 0;
}

int Trap_fn(Coord c, Entity * e, StateList * list)
{

    Trap_t t = {e->cha_id, same_team(e, Allies)};
    Set_Trap(t, c);
    return 0;
}

int Banner_fn(Coord c, Entity * e, StateList * list)
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

    return 0;
}

int mage_switch(Coord c, Entity * e, StateList * list)
{
    int i = rand()%2;
    if((Ability*)mage_ab+i != e->cha_class->cla_abilities)
    {
        e->cha_class->cla_abilities = (Ability*)mage_ab+i;
    }
    else
    {
        e->cha_class->cla_abilities = (Ability*)mage_ab+2;
    }

    return 0;
}

int FlameCharge_fn(Coord c, Entity * e, StateList * list)
{
    Ability F = e->cha_class->cla_abilities[FlameCharge%NUM_AB];
    //Insert Pathfinding
    //Change F's zone to match pathfinding route
    return apply_to(F,e,list,e->coords);
}

int Flare_fn(Coord c, Entity * e, StateList * list)
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

    Status b_vis = {4,vis,2};
    Status b_mv = {4,mv,2};

    int i;
    for(i=0; i<NUM_CLASS; i++)
    {
        apply_stat_change(b_vis,all+i,list);
        apply_stat_change(b_mv,all+i,list);
    }

    Ability a = e->cha_class->cla_abilities[Flare%NUM_AB];
    Trap_t t;
    Coord c2;

    for(i=0; i<a.nb_coords; i++)
    {
        c2 = add_coords(c,a.coord[i]);
        t = Get_Trap(c2);
        if(t.cha_id<0)
        {
            t.visible = TRUE;
            Set_Trap(t,c2);
        }
    }

    return 0;
}

int Blizzard_fn(Coord c, Entity * e, StateList * list)
{
    freezeWater(c);
    return 0;
}

int Volt_Switch_fn(Coord c, Entity * e, StateList * list)
{
    Entity * t = getEntity(matrix,c);
    switchEntities(c,e->coords);
    t->coords = e->coords;
    e->coords = c;

    return 0;
}

int Lightning_Chain_fn(Coord c, Entity * e, StateList * list)
{
    //Implement algo de lucien
    return 0;
}

int Thrust_fn(Coord c, Entity * e, StateList * list)
{
    Entity * target = getEntity(matrix, add_coords(c,compare_coords(e->coords,c)));

    if(target!=NULL)
    {
        apply_mod(*e->cha_class->cla_abilities[Thrust%NUM_AB].mods,target, list, e->cha_id);
        if(apply_damage(e->cha_class->cla_abilities[Thrust%NUM_AB].damage,e,target))
        {
            return 1;
        }
    }
    return 0;
}

int Life_Transfer_fn(Coord c, Entity * e, StateList * list)
{
    Entity *f;
    Entity *t;

    Entity * all;
    if(e->cha_id<0)
    {
        all = Allies;
    }
    else
    {
        all = Foes;
    }

    int i,j=0;
    int r;
    int tab[NUM_CLASS];

    for(i=0; i<NUM_CLASS; i++)
    {
        if((all+i)->active)
        {
            tab[j++] = (all+i)->cha_class->cla_id;
        }
    }

    t = &all[tab[rand()%j]];

    f=getEntity(matrix, c);

    int h = f->base_stats[pv] - f->stat_mods[pv];
    f->stat_mods[pv] = 20;

    Status s = {h,mv,3};

    apply_stat_change(s,t,list);
    return 0;
}

int Gates_of_Valhalla_fn(Coord c, Entity * e, StateList * list)
{
    return 0;
}

int Last_Sacrfice_fn(Coord c, Entity * e, StateList * list)
{
    return 0;
}