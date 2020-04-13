#ifndef substruct_h
#define substruct_h
#include "common.h"

#define NUM_STATS 7
#define NUM_CLASS 6
#define NUM_AB 4
#define NUM_STATUS 10

/* ENUMERATIONS */
typedef enum {pv, mv, vis, atk, magic, res_physic, res_magic} statId;

typedef enum {Berserker, Ranger, Goliath, Mage, Valkyrie, Angel} classId;

typedef enum {Slash, Killing_Blow, Fury, Frenzied_Dash, Bolt, Focus, Trap, Deadeye, Bash, Shields_Up, Detain, Banner, Fireball, FlameCharge, Flare, Eruption, Icy_Winds, Freeze, Frozen_Armor, Blizzard, Shock, Volt_Switch, Shock_Therapy, Lightning_Chain, Thrust, Odins_Eyes, Life_Transfer, Gates_of_Valhalla, Condemn, Holy_Storm, Last_Sacrfice, Gods_Blessing, Mvt, Aura} abilityId;

typedef enum {Dead, Alive} lifeId;

typedef enum {Cripple, Detained, Provoked, Burning, Freezing, Paralyzed, Blessed, Piercing, Guarding, Summoned} statusId;

typedef enum {FOES, ALLIES, BOTH, FREE_TILE, ANY_TILE} targetType;

typedef enum {NONE, DURING, BEFORE, ONLY, AFTER} fnid;

typedef enum {N, E, S, W} Direction;

typedef enum {ONGOING, WIN, LOSE} winId;

struct entity_t;

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

//STRINGS OF ENUMERATIONS FOR PRINTING PURPOSES
extern char statName[NUM_STATS][STR_SHORT];
extern char IdNames[Aura+1][STR_SHORT];
extern char statusName[NUM_STATUS][STR_SHORT];
extern char targetNames[5][STR_SHORT];
extern char fnNames[5][STR_SHORT];

#endif