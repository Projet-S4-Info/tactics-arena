#include <stdlib.h>
#include <stdio.h>
#include "struct.h"
#include "init_classes.h"
#include "common.h"
#include "print.h"
#include "gameplay.h"
#include "servFcnt.h"
#include "grid.h"

StateList *stSent = NULL;
StateList *stReceived = NULL;
Class classes[NUM_CLASS] = {};
Ability mage_ab[3][NUM_AB] = {};
Entity Allies[NUM_CLASS] = {};
Entity Foes[NUM_CLASS] = {};
Ability Aura_ab;
Ability Move_ab;

err_t init_spawn(Entity * e, Coord c)
{
    e->coords = c;
    if(verbose>=2)printf("Setting %s Spawn at %d %d\n", e->cha_name, c.x, c.y);

    free_spawn(e);
    
    return OK;
}

err_t ent_common_init(Entity *e)
{
    e->active = Alive;
    e->act_points = 3;
    
    int i;
    for(i=0; i<NUM_AB; i++)
    {
        e->ab_cooldown[i] = 0;
    }
    for(i=0; i<NUM_STATUS; i++)
    {
        e->status_effect[i] = 0;
    }

    return OK;
}

err_t init_Foes(Direction d)
{
    init_ent e;
    Tile * t;
    int i,j;
    for(i=0; i<NUM_CLASS; i++)
    {
        rec_id_swap(recep(&e,sizeof(init_ent),socketConnected));
        Foes[e.cha_class].cha_id = e.char_id;
        sprintf(Foes[e.cha_class].cha_name, "Ennemy %s", e.cha_name);
        Foes[e.cha_class].cha_class = &classes[e.cha_class];
        Foes[e.cha_class].direction = d;

        for(j=0; j<NUM_STATS; j++)
        {
            Foes[e.cha_class].base_stats[j] = Foes[e.cha_class].stat_mods[j] = e.base_stats[j];
        }

        Foes[e.cha_class].coords = e.starting_position;
        t = getTile(e.starting_position);
        t->entity = &Foes[e.cha_class];

        ent_common_init(&Foes[e.cha_class]);     
    }

    Foes[Mage].cha_class->cla_abilities = &mage_ab[rand()%3][0];

    return OK;
}

err_t init_Allies(Coord spawn[NUM_CLASS], Direction d)
{
    classId spawn_order[NUM_CLASS] = {Ranger, Mage, Angel, Valkyrie, Goliath, Berserker};
    init_ent ie;

    int s,i,j;
    for(s=0; s<NUM_CLASS; s++)
    {
        ie.cha_class = i = spawn_order[s];

        ie.char_id = Allies[i].cha_id = i+1;

        sprintf(ie.cha_name, "%s", classes[i].cla_name);

        sprintf(Allies[i].cha_name, "Friendly %s", classes[i].cla_name);

        Allies[i].cha_class = &classes[i];
        Allies[i].direction = d;
        
        for(j=0; j<NUM_STATS; j++)
        {
            ie.base_stats[i] = Allies[i].base_stats[j] = Allies[i].stat_mods[j] = classes[i].basic_stats[j];
        }

        ent_common_init(&Allies[i]);
        init_spawn(&Allies[i], spawn[s]);
        ie.starting_position = Allies[i].coords;

        sendStruct(&ie, sizeof(init_ent), socketConnected);
    }

    Allies[Mage].cha_class->cla_abilities = &mage_ab[rand()%3][0];

    return OK;
}

err_t ent_init_test(Entity *e, char title[STR_SHORT])
{
    Coord ally_spawn[NUM_CLASS] = {{29,29},{26,28},{28,26},{22,28},{25,25},{28,22}};
    Coord foe_spawn[NUM_CLASS] = {{0,0},{1,3},{3,1},{1,7},{4,4},{7,1}};
    classId spawn_order[NUM_CLASS] = {Ranger, Mage, Angel, Valkyrie, Goliath, Berserker};

    int s,i,j;
    for(s=0; s<NUM_CLASS; s++)
    {
        i = spawn_order[s];

        sprintf((e+i)->cha_name, "%s %s", title, classes[i].cla_name);
        (e+i)->cha_class = &classes[i];

        if(e==Allies)
        {
            (e+i)->cha_id = i+1;
            (e+i)->direction = W;
            init_spawn(e+i, ally_spawn[s]);
            if(verbose>=2)printf("%s Spawn Set to %d %d\n", e->cha_name, e->coords.x, e->coords.y);

        }
        else
        {
            (e+i)->cha_id = (i+1)*-1;
            (e+i)->direction = S;
            init_spawn(e+i, foe_spawn[s]);
            if(verbose>=2)printf("%s Spawn Set to %d %d\n", e->cha_name, e->coords.x, e->coords.y);
        }

        (e+i)->active = Alive;
        (e+i)->act_points = 3;

        for(j=0;j<NUM_STATS;j++)
        {
            (e+i)->stat_mods[j] = (e+i)->base_stats[j] = classes[i].basic_stats[j];
        }
        for(j=0; j<NUM_STATUS; j++)
        {
            (e+i)->status_effect[j] = 0;
        }
        for(j=0; j<NUM_AB; j++)
        {
            (e+i)->ab_cooldown[j] = 0;
        }
    }

    (e + Mage)->cha_class->cla_abilities = &mage_ab[rand()%3][0];

    return OK;
}

err_t init_classes()
{

    printf("%s",error_message[init_repetitives(&Move_ab)]);

    printf("%s",error_message[init_berserker(&classes[Berserker])]);
    printf("%s",error_message[init_ranger(&classes[Ranger])]);
    printf("%s",error_message[init_goliath(&classes[Goliath])]);
    printf("%s",error_message[init_mage(&classes[Mage], mage_ab)]);
    printf("%s",error_message[init_valkyrie(&classes[Valkyrie])]);
    printf("%s",error_message[init_angel(&classes[Angel], &Aura_ab)]);  
    
    printf("%s",error_message[init_list(&stSent)]);
    printf("%s",error_message[init_list(&stReceived)]);

    if(verbose>=1) printf("End of initialisation!\n");

    return OK;
}

err_t destroy_game()
{
    if(verbose>=1) printf("\nStart of Destruction!\n");

    list_destroy(stSent);
    list_destroy(stReceived);

    int i,j;

    for(i=0; i<NUM_CLASS; i++)
    {
        class_destroy(&classes[i]);
    }

    if(verbose>=1) printf("Starting destruction of Mage!\n");
    for(i=0; i<3; i++)
    {
        for(j=0; j<NUM_AB; j++)
        {
            ability_destroy(&mage_ab[i][j]);
        }
    }
    if(verbose>=1) printf("Mage's abilities destroyed!\n\n");

    ability_destroy(&Aura_ab);
    if(verbose>=1) printf("Angel's Aura passive destroyed!\n");

    ability_destroy(&Move_ab);
    if(verbose>=1) printf("Movement ability destroyed\n");

    return OK;
}