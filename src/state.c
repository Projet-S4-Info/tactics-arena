#include <stdlib.h>
#include "state.h"

err_t init_list(StateList * list)
{
    list = malloc(sizeof(StateList));
    list->drapeau = malloc(sizeof(List_Elem));
    if(list->drapeau == NULL) return POINTER_NULL;
    list->drapeau->value = NULL;
    list->drapeau->prec = list->drapeau;
    list->drapeau->suiv = list->drapeau;
    list->ec = list->drapeau;
    return OK;
}

int out_of_list(StateList * list)
{
    return list->ec == list->drapeau;
}

int list_empty(StateList * list)
{
    return list->drapeau->suiv == list->drapeau;
}

err_t start_list(StateList * list)
{
    list->ec=list->drapeau->suiv;
    return OK;
}

err_t end_list(StateList * list)
{
    list->ec=list->drapeau->prec;
    return OK;
}

err_t list_next(StateList * list)
{
    list->ec=list->ec->suiv;
    return OK;
}

List_Elem * list_decrease(StateList * list)
{
    list->ec->value->duration--;

    if(list->ec->value->duration==0)
        return list->ec;

    return NULL;
}

err_t list_remove(StateList * list)
{
    List_Elem * temp = list->ec;
    temp->prec->suiv=temp->suiv;
    temp->suiv->prec=temp->prec;
    list->ec=temp->prec;
    free(temp->value);
    free(temp);
    return OK;
}

err_t list_add(StateList * list, Status * v, int entity)
{
    end_list(list);
    
    Status * newstat = malloc(sizeof(Status));
    if(newstat == NULL) return POINTER_NULL;
    *newstat = *v;


    List_Elem * newelem = malloc(sizeof(List_Elem));
    if(newelem == NULL) return POINTER_NULL;
    newelem->value = newstat;
    newelem->entity = entity;
    newelem->prec = list->ec;
    newelem->suiv = list->drapeau;

    list->ec->suiv = newelem;
    list->drapeau->prec = newelem;

    list->ec = newelem;
    return OK;
}

err_t list_destroy(StateList * list)
{
    if(!(list_empty(list)))
    {
        start_list(list);
        while(!(out_of_list(list)))
        {
            list_remove(list);
            list_next(list);
        }
    }

    free(list->drapeau);
    free(list);
    return OK;
}