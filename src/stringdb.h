#ifndef stringdb_h
#define stringdb_h
#include "common.h"

char error_message[NUM_ERROR][STR_LONG] = {"", 
                                            "ERROR: Pointer is Null\n", 
                                            "ERROR : SDL\n", 
                                            "ERROR : Coordinates out of limits\n", 
                                            "ERROR : Ability coordinates were not initialised correctly.\n"};


char statName[NUM_STATS][STR_SHORT] = {"pv", "mv", "vis", "atk", "magic", "res_physic", "res_magic"};
char IdNames[Aura+1][STR_SHORT] = {"Slash", "Killing_Blow", "Fury", "Frenzied_Dash", "Bolt", "Focus", "Trap", "Deadeye", "Bash", "Shields_Up", "Detain", "Banner", "Fireball", "FlameCharge", "Flare", "Eruption", "Icy_Winds", "Freeze", "Frozen_Armor", "Blizzard", "Shock", "Volt_Switch", "Shock_Therapy", "Lightning_Chain", "Thrust", "Odins_Eyes", "Life_Transfer", "Gates_of_Valhalla", "Condemn", "Holy_Storm", "Last_Sacrfice", "Gods_Blessing", "Mvt", "Aura"};
char statusName[NUM_STATUS][STR_SHORT] = {"Crippled", "Detained", "Provoked", "Burning", "Frozen", "Paralyzed", "Blessed", "Piercing", "Guarding", "Summoned"};
char targetNames[5][STR_SHORT] = {"FOES", "ALLIES", "BOTH", "FREE_TILE", "ANY_TILE"};
char fnNames[5][STR_SHORT] = {"NONE", "DURING", "BEFORE", "ONLY", "AFTER"};

#endif