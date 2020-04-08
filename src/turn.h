#ifndef turn_h
#define turn_h
#include "struct.h"

bool your_turn();
err_t apply_action(action a);
err_t action_set(action a);
Entity * play_check(Entity *E);
err_t turn();

#endif