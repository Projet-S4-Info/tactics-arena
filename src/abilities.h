#ifndef abilities_h
#define abilities_h
#include "struct.h"

bool Killing_Blow_fn(Coord c, Entity * e, StateList * list);
bool Fury_fn(Coord c, Entity * e, StateList * list);

bool Focus_fn(Coord c, Entity * e, StateList * list);
bool Trap_fn(Coord c, Entity * e, StateList * list);

bool Banner_fn(Coord c, Entity * e, StateList * list);

bool mage_switch(Coord c, Entity * e, StateList * list);
bool FlameCharge_fn(Coord c, Entity * e, StateList * list);
bool Flare_fn(Coord c, Entity * e, StateList * list);
bool Blizzard_fn(Coord c, Entity * e, StateList * list);
bool Volt_Switch_fn(Coord c, Entity * e, StateList * list);
bool Lightning_Chain_fn(Coord c, Entity * e, StateList * list);

bool Thrust_fn(Coord c, Entity * e, StateList * list);
bool Life_Transfer_fn(Coord c, Entity * e, StateList * list);
bool Gates_of_Valhalla_fn(Coord c, Entity * e, StateList * list);

bool Last_Sacrfice_fn(Coord c, Entity * e, StateList * list);

#endif