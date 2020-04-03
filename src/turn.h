#ifndef turn_h
#define turn_h
#include "struct.h"

err_t apply_action(action a);
Entity * play_check(Entity *E);
err_t turn();

#endif