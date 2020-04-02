#ifndef state_h
#define state_h
#include "substruct.h"

typedef struct listelem{
  Status * value;
  int entity;
  struct listelem * suiv;
  struct listelem * prec;
} List_Elem;

typedef struct {
  List_Elem * drapeau;
  List_Elem * ec;
} StateList;

err_t init_list(StateList * list);
bool out_of_list(StateList * list);
bool list_empty(StateList * list);
err_t start_list(StateList * list);
err_t end_list(StateList * list);
err_t list_next(StateList * list);
List_Elem * list_change(StateList * list, int d);
err_t list_remove(StateList * list);
err_t list_add(StateList * list, Status * v, int entity);
bool list_search(StateList * list, int entity);
bool list_check(StateList * list);
err_t list_destroy(StateList * list);


#endif