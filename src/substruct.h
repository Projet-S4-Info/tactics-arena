#ifndef substruct_h
#define substruct_h
#include "common.h"

#define NUM_STATS 7
#define NUM_CLASS 6
#define NUM_AB 4
#define NUM_STATUS 8

/* ENUMERATIONS */
typedef enum {pv, mv, vis, atk, magic, res_physic, res_magic} statId;

typedef enum {Berserker, Ranger, Goliath, Mage, Valkyrie, Angel} classId;

typedef enum {Slash, Killing_Blow, Fury, Frenzied_Dash, Bolt, Focus, Trap, Deadeye, Bash, Shields_Up, Detain, Banner} abilityId;

typedef enum {Dead, Alive, Summoned} lifeId;

typedef enum {Cripple, Detained, Provoked, Burning, Freezing, Paralyzed, Blessed, Piercing, Guarding} statusId;

typedef enum {FOES, ALLIES, BOTH} targetType;

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
    int value;  //IF value is equal to 0 then it is a status effect, otherwise it is a stat modifier
    int stat;
    int duration; //IF duration is equal to zero then it is a permanent change
} Status;

typedef struct
{
    Status effect;
    float chance;
    targetType t;
}Modifier;

#endif