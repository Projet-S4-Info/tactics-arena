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

err_t apply_action(action act)
{
    Entity active_ent;
    Ability active_ab;
    Entity * e;
    if(act.char_id<0)
        active_ent = Foes[act.char_id*-1-1];
    else
        active_ent = Allies[act.char_id-1];

    active_ab = *(active_ent.cha_class->cla_abilities);

    int i;
    for(i=0; i<active_ab.nb_coords; i++)
    {
        
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

err_t turn(Class * class, Entity * allies, Entity * foes, StateList * local, StateList * opponent)
{
    Entity * active_ent=allies;
    while((active_ent=play_check(active_ent))!=NULL)
    {
        //while action selected is change character
            //Select entity pointed to by active_ent
            //Wait for action selection
            //Wait for coordinate selection

        //relay action information
        //wait for confirmation
        //animate the action

    }
    return OK;
}

