#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "init_classes.h"
#include "common.h"

StateList *stSent = NULL;
StateList *stReceived = NULL;
Class classes[NUM_CLASS] = {};
Ability mage_ab[3][NUM_AB] = {};
Entity Allies[NUM_CLASS] = {};
Entity Foes[NUM_CLASS] = {};
Ability Aura_ab;

err_t ent_init(Entity *e)
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
        (e+i)->direction = S;

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
        if(verbose) printf("Allies : %s initialized!\n", (e+i)->cha_name);
    }

    Allies[Mage].cha_class->cla_abilities = &mage_ab[rand()%3][0];

    return OK;
}

err_t init_classes()
{

    printf("%s",error_message[init_repetitives()]);

    printf("%s",error_message[init_berserker(&classes[Berserker])]);
    printf("%s",error_message[init_ranger(&classes[Ranger])]);
    printf("%s",error_message[init_goliath(&classes[Goliath])]);
    printf("%s",error_message[init_mage(&classes[Mage], mage_ab)]);
    printf("%s",error_message[init_valkyrie(&classes[Valkyrie])]);
    printf("%s",error_message[init_angel(&classes[Angel], &Aura_ab)]);  
    
    printf("%s",error_message[init_list(stSent)]);
    printf("%s",error_message[init_list(stReceived)]);

    if(verbose) printf("End of initialisation!\n");

    return OK;
}

err_t destroy_game()
{
    list_destroy(stSent);
    list_destroy(stReceived);

    int i,j;

    for(i=0; i<NUM_CLASS; i++)
    {
        class_destroy(&classes[i]);
    }

    for(i=0; i<3; i++)
    {
        for(j=0; j<NUM_AB; j++)
        {
            ability_destroy(&mage_ab[i][j]);
        }
    }

    return OK;
}