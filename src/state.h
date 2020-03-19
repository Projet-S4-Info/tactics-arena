#ifndef state_h
#define state_h
#include "substruct.h"

typedef struct listelem{
  Status * value;
  struct listelem * suiv;
  struct listelem * prec;
} List_Elem;

typedef struct {
  List_Elem * drapeau;
  List_Elem * ec;
} StateList;

#endif