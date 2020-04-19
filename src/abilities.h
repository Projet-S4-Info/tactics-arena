#ifndef abilities_h
#define abilities_h
#include "struct.h"

int Killing_Blow_fn(Coord c, Entity * e, StateList * list);
int Fury_fn(Coord c, Entity * e, StateList * list);
int Frenzied_Dash_fn(Coord c, Entity * e, StateList * list);

int Focus_fn(Coord c, Entity * e, StateList * list);
int Trap_fn(Coord c, Entity * e, StateList * list);

int Detain_fn(Coord c, Entity *e, StateList * list);
int Banner_fn(Coord c, Entity * e, StateList * list);

int mage_switch(Coord c, Entity * e, StateList * list);
err_t update_mage(Entity *e, abilityId Id);
int FlameCharge_fn(Coord c, Entity * e, StateList * list);
int Flare_fn(Coord c, Entity * e, StateList * list);
int Blizzard_fn(Coord c, Entity * e, StateList * list);
int Volt_Switch_fn(Coord c, Entity * e, StateList * list);
int Lightning_Chain_fn(Coord c, Entity * e, StateList * list);

int Thrust_fn(Coord c, Entity * e, StateList * list);
int Life_Transfer_fn(Coord c, Entity * e, StateList * list);
int Gates_of_Valhalla_fn(Coord c, Entity * e, StateList * list);

int Last_Sacrfice_fn(Coord c, Entity * e, StateList * list);
int Gods_Blessing_fn(Coord c, Entity *e, StateList * list);

#endif