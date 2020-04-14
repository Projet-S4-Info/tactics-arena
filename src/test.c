#include "init.h"
#include "print.h"
#include <stdio.h>

err_t test()
{
    if(stSent!=NULL)
    {
        print_StateList(stSent, "");

        list_add(stSent, (*(classes[Valkyrie].cla_abilities->mods))->effect, &Foes[Berserker]);
        list_add(stSent, (*(mage_ab[1]->mods))->effect, &Foes[Goliath]);

        print_StateList(stSent, "");

        start_list(stSent);
        list_search(stSent, &Foes[Goliath], -1);
        list_remove(stSent);

        print_StateList(stSent, "");
    }
    else
    {
        printf("List is NULL\n");
    }

    return OK;
}