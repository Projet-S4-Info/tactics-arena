#ifndef turn_h
#define turn_h
#include "struct.h"

extern bool is_online;
extern bool game_setup;
extern bool applying_action;
extern bool loop_active;

err_t online_setup();
bool your_turn();
err_t apply_action(action a);
err_t action_set(action a);
err_t test_turn();
err_t set_endturn();
bool play_check(Entity *E);
winId game_loop(err_t (*turn1)(void), err_t (*turn2)(void));
winId init_client();
winId init_server();

#endif