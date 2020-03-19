#ifndef substruct_h
#define substruct_h
#include "common.h"

/* ENUMERATIONS */
typedef enum {pv, mv, vis, atk, magic, res_physic, res_magic} statId;

typedef enum {Berserker, Ranger, Goliath, Mage, Valkyrie, Angel} classId;

typedef enum {Slash, Killing_Blow, Fury, Frenzied_Dash} abilityId;

typedef enum {Dead, Alive, Summoned} lifeId;

typedef enum {Cripple, Detained, Provoked, Burning, Freezing, Paralyzed, Blessed, Deadeye} statusId;

typedef enum {Stat_Mod, Status_Eff} flagId;

/*Entity Sub Structures*/

typedef struct
{
    char name[STR_SHORT];
    char desc[STR_LONG];
}lang;

typedef struct
{
    float multiplier;
    statId type;
} Damage;

typedef struct
{
    int value;
    statId stat;
    int duration;
} Effect;

typedef struct
{
    statusId Status;
    int duration;
} Status;

union eff_u
{
    Status status;
    Effect effect;
};

typedef struct
{
    flagId flag;
    union eff_u mod;
    float chance;
}Modifier;

#endif