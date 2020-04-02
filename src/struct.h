#ifndef struct_h
#define struct_h

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "state.h"




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

typedef struct {
    int char_id;
    Coord c;
    abilityId act; //-1 if movement
}action;

typedef struct
{
    abilityId ab_id;
    int ab_cost;
    int ab_cooldown;
    int range; //If zero then self cast
    Damage * damage;
    int nb_coords; 
    Coord * coord;
    int nb_mods;
    Modifier * mods;
    bool only_fn;
    err_t (*function)(action);
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
    classId cha_class;
    // Item List
} Character;

typedef struct
{
    int cha_id;
    char cha_name[STR_SHORT];
    Class * cha_class;
    lifeId active;
    Coord coords;
    int act_points;
    int base_stats[NUM_STATS];
    int stat_mods[NUM_STATS];
    int status_effect[NUM_STATUS];
    int ab_cooldown[NUM_AB];
} Entity;

/* TERRAIN STRUCTURES */

typedef struct
{
    int tile_id;
    int selected;
    Entity * entity;
    int walkable;
} Tile;

/* TEXTURE STRUCTURES */

typedef struct
{
    char * texture_name;
    SDL_Texture *texture;
    SDL_Texture *big_texture;
} TabTexture;


/* COMMUNICATION STRUCTURES */

typedef struct User{
  int id;
  char pseudo[64];
} t_user;

typedef struct {
    int char_id;
    char cha_name[STR_SHORT];
    classId cha_class;
    int base_stats[NUM_STATS];
    Coord starting_position;
}init_ent; 

#endif
