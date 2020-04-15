#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

err_t init_chat(chat_t * c)
{
    c->index = -1;
    return OK;
}

bool chat_vide(chat_t *c)
{
    return c->index == -1;
}

bool increment(chat_t * c)
{
    c->index ++;
    if(c->index>=_NB_MAX_CHAT_)
    {
        c->index = _NB_MAX_CHAT_ -1;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

err_t decaler(chat_t *c)
{
    int i;

    for(i=0; i < _NB_MAX_CHAT_ - 1; i++)
    {
        strcpy(c->chatTab[i], c->chatTab[i+1]);
    }

    return OK;
}

err_t nouveau_Msg(chat_t *c, char msg[STR_LONG])
{
    
    if(increment(c))
    {
        decaler(c);
    }

    strcpy(c->chatTab[c->index], msg);

    return OK;
}