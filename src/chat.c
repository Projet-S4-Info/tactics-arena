#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "servFcnt.h"

chat_t chat;

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

void startChat(void * structure, int size, int socket){
    if(verbose)printf("Dans le thread chat \n");
    while(1){
        if(changesChat == 1){
            if(sendStruct(structure, size, socket) == OK){
                if(verbose)printf("Structure envoyée depuis threadchat\n");
                changesChat = 0;
            }else{
                printf("Aucune structure envoyée depuis thread Chat \n");
            }
        }else{
            if(recepChat(structure,size,socket) == 1){
                if(verbose)printf("Structure recue depuis trhreadChat \n");

                for(int i = 0; i < chat.index; i++){
                    if(verbose)printf("test d'affichage de chat : %s \n", chat.chatTab[i]);
                }
            }else{
                if(verbose)printf("Aucune structure recue depuis threadChat \n");
            }
        }
    }
}