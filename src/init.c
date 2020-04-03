#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "struct.h"
#include "init_classes.h"

StateList *stSent = NULL;
StateList *stReceived = NULL;
Class classes[NUM_CLASS] = {};
Ability mage_ab[3][NUM_AB] = {};
Entity Allies[NUM_CLASS] = {};
Entity Foes[NUM_CLASS] = {};

err_t temp_ent_init(Entity *e)
{

    int i,j;
    for(i=0; i<NUM_CLASS; i++)
    {
        (e+i)->cha_id = i+1;
        strcpy((e+i)->cha_name, classes[i].cla_name);
        (e+i)->cha_class = &classes[i];
        (e+i)->active = Alive;
        (e+i)->coords.x = 0;
        (e+i)->coords.y = 0;
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

    return OK;
}

err_t init_game()
{
    srand(time(NULL));

    init_berserker(&classes[Berserker]);
    init_ranger(&classes[Ranger]);
    init_goliath(&classes[Goliath]);
    init_mage(&classes[Ranger], mage_ab);
    init_valkyrie(&classes[Valkyrie]);
    init_angel(&classes[Angel]);

    init_list(stSent);
    init_list(stReceived);

    temp_ent_init(Allies);

    return OK;
}