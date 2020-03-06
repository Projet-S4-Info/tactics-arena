#ifndef struct_h
#define struct_h

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"

#define STR_SHORT 25
#define STR_LONG 50
#define NUM_STATS 7
#define NUM_CLASS 6
#define NUM_AB 4
#define NUM_STATUS 8

/* ENUMERATIONS */
typedef enum {pv, mv, vis, atk, magic, res_physic, res_magic} statId;

typedef enum {Berserker, Ranger, Goliath, Mage, Valkyrie, Angel} classId;

typedef enum {Slash, Bolt, Bash} abilityId;

typedef enum {Dead, Alive, Summoned} lifeId;

typedef enum {Cripple, Detained, Provoked, Burning, Freezing, Paralyzed, Blessed, Deadeye} statusId;

/* BASIC STRUCTURES*/
typedef struct
{
    int x;
    int y;
} Coord;

typedef struct
{
    float x;
    float y;
} Vector;

/* ENTITY STRUCTURES */
typedef struct
{
    int multiplier;
    statId type;
} Damage;

typedef struct
{
    int value;
    statId stat;
    int duration;
    int chance;
} Effect;

typedef struct
{
    statusId Status;
    int duration;
    int chance;
} Status;

typedef struct
{
    abilityId ab_id;
    char ab_name[STR_SHORT];
    int ab_cost;
    int ab_cooldown;
    int self_cast;
    Damage damage;
    int nb_coords;
    Coord * coord;
    int nb_effects;
    Effect * effects;
    Status status;
    int (*function)(int, Coord *, StateList *); //Takes caster ID, Coordinates, and pointer to state chain
    char sprite_folder[STR_LONG];
} Ability;

typedef struct
{
    classId cla_id;
    char cla_name[STR_SHORT];
    int basic_stats[NUM_STATS];
    Ability * cla_abilities;
    char sprite_folder[STR_LONG];
} Class;

typedef struct
{
    int cha_id;
    char cha_name[STR_SHORT];
    Class * cha_class;
    // Item List
} Character;

typedef struct
{
    Character * character;
    int act_points;
    int base_stats[NUM_STATS];
    int stat_mods[NUM_STATS];
    State * status[NUM_STATUS]; //Table to pointers that point to status if active from statechain
} Entity;

/* TERRAIN STRUCTURES */

typedef struct
{
    int tile_id;
    int selected;
    Entity * entity;
} Tile;

/* TEXTURE STRUCTURES */

typedef struct
{
    char * texture_name;
    SDL_Texture *texture;
    SDL_Texture *big_texture;
} TabTexture;


/* COMMUNICATION STRUCTURES */



#endif