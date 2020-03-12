#ifndef struct_h
#define struct_h

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "state.h"

#define STR_SHORT 50
#define STR_LONG 256
#define NUM_STATS 7
#define NUM_CLASS 6
#define NUM_AB 4
#define NUM_STATUS 8

/* ENUMERATIONS */
typedef enum {pv, mv, vis, atk, magic, res_physic, res_magic} statId;

typedef enum {Berserker, Ranger, Goliath, Mage, Valkyrie, Angel} classId;

typedef enum {Slash, Killing_Blow, Fury, Frenzied_Dash} abilityId;

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
    int ab_cost;
    int ab_cooldown;
    int range;
    Damage damage;
    int nb_coords; //0 is self cast
    Coord * coord;
    int nb_effects;
    Effect * effect;
    Status status;
    int (*function)(int, Coord *, StateList *); //Takes caster ID, Coordinates, and pointer to state chain
    lang eng;
    char sprite_folder[STR_LONG];
} Ability;

typedef struct
{
    classId cla_id;
    char cla_name[STR_SHORT];
    int basic_stats[NUM_STATS];
    lang Passive;
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

typedef union{
    int mov;
    char atk;
}comm_union;

typedef struct {
    int flag;
    comm_union action;
}comm;



#endif
