#include "struct.h"
#include "init.h"
#include "grid.h"

// LOCAL TURN

//Go Through Opponent Statelist

//While characters can still play

    //Select character
    //Show Character Hud

    //Select action
    //Show action range

    //Select action coord
    //Relay information

    //Show animation
    //Apply stat changes
    //Apply State changes
    //Apply ab cooldown

//Go Through Your Statelist
Coord add_coords(Coord a, Coord b)
{
    Coord c;

    c.x = a.x + b.x;
    c.y = a.y + b.y;

    return c;
}

err_t apply_damage(Damage * d, Entity * target)
{
    return OK;
}

err_t apply_mod(Modifier m, Entity * target, StateList * list)
{
    return OK;
}

err_t apply_action(action a)
{
    Entity * active_ent;
    Ability active_ab;
    Entity * e;
    if(a.char_id<0)
        active_ent = &Foes[a.char_id*-1-1];
    else
        active_ent = &Allies[a.char_id-1];

    active_ab = active_ent->cha_class->cla_abilities[a.act%NUM_AB];

    int i,j;
    for(i=0; i<active_ab.nb_coords; i++)
    {
        //e=getEntity(add_coords(a.c, active_ab.coord[i]));
        if(e!=NULL)
            if(e->cha_id<0)
                if(active_ab.damage!=NULL)
                    apply_damage(active_ab.damage, e);

            if(active_ab.mods!=NULL)
                for(j=0; j<active_ab.nb_mods; j++)
                {
                    if(e->cha_id<0&&active_ab.mods[j].t!=Allies)
                        apply_mod(active_ab.mods[j],e, &stFoe);
                    else if((e->cha_id>0&&active_ab.mods[j].t!=Foes))
                        apply_mod(active_ab.mods[j],e, &stAlly);
                }
    }
    
    if(active_ab.function!=NULL)
        active_ab.function(a);
    
    active_ent->ab_cooldown[a.act%NUM_AB] = active_ab.ab_cooldown;

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

err_t turn(Class * class, Entity * allies, Entity * foes, StateList * local, StateList * opponent)
{
    Entity * active_ent=allies;
    action a;
    while((active_ent=play_check(active_ent))!=NULL)
    {
        //while action selected is change character
            //Select entity pointed to by active_ent
            //Wait for action selection
            //Wait for coordinate selection

        //relay action information
        //wait for confirmation
        //animate the action
        if(a.act<0);
            //call mvt function (dont forget to remove semicolon after if)
        else
            apply_action(a);
        
    }
    return OK;
}

