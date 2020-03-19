#ifndef state_h
#define state_h

typedef struct {
  int nb_tours;
  int id_buff;
  char * buff_name;
} State;

typedef struct {
  State * value;
  struct StateList * suiv;
} StateList;

typedef struct {
  StateList * first;
} List;


#endif
