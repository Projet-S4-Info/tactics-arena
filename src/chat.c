#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "struct.h"
#include "servFcnt.h"


err_t init_chat(chat_t *c)
{
    c->index = -1;
    return OK;
}

bool chat_vide(chat_t *c)
{
    return c->index == -1;
}

bool increment(chat_t *c)
{
    c->index++;
    if (c->index >= _NB_MAX_CHAT_)
    {
        c->index = _NB_MAX_CHAT_ - 1;
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

    for (i = 0; i < _NB_MAX_CHAT_ - 1; i++)
    {
        strcpy(c->chatTab[i], c->chatTab[i + 1]);
    }

    return OK;
}

err_t nouveau_Msg(chat_t *c, char msg[STR_LONG])
{

    if (increment(c))
    {
        decaler(c);
    }

    strcpy(c->chatTab[c->index], msg);

    return OK;
}

void startChat(chat_t chat, int size, int socket)
{
    if (verbose >= 0)
        printf("Début de start Chat \n");

    while (1)
    {
        if(verbose >= 0)printf("Dans le while threadchat \n");
        if (changesChat == 1)
        {
            if (sendChat((void *)&chat, size, socket) == 1)
            {
                if(verbose >= 1)printf("Chat envoyé en vrai \n");
            }
            else
            {
                printf("Aucune structure envoyée depuis thread Chat \n");
            }
            changesChat = 0;
        }
        else
        {
            if (recepChat((void *)&chat,size , socket) != 1)
            {
                printf("Aucune structure recue depuis threadChat \n");
            }
            else
            {
                if (verbose >= 0)
                    printf("Structure recue dans thread chat ! \n");
            }
        }
        sleep(2);
    }
}