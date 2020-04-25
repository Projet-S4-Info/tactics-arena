#ifndef struct_h
#define struct_h

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "state.h"
#include "substruct.h"

#define _X_SIZE_ 30
#define _Y_SIZE_ 30

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

typedef struct
{
    Uint32 start;
    Uint32 end;
} Timer;

typedef struct
{
    char *message;
    Uint32 time;
} Log;

typedef struct{
    char chatTab[_NB_MAX_CHAT_][STR_LONG];
    int index;
}chat_t;

/* ENTITY STRUCTURES */

typedef struct {
    int char_id;
    Coord c;
    abilityId act; //Mvt if movement
} action;

typedef struct
{
    abilityId ab_id;
    int ab_cost;
    int ab_cooldown;
    int range; //If zero then self cast
    targetType target;
    Damage ** damage;
    int nb_coords; 
    Coord ** coord;
    int nb_mods;
    Modifier ** mods;
    fnid fn_use;
    int (*function)(Coord, struct entity_t *, StateList *);
    lang eng;
} Ability;

typedef struct
{
    classId cla_id;
    char cla_name[STR_SHORT];
    int basic_stats[NUM_STATS];
    lang Passive;
    Ability * cla_abilities;
} Class;

typedef struct
{
    int cha_id;
    char cha_name[STR_SHORT];
    classId cha_class;
    // Item List
} Character;

typedef struct entity_t
{
    int cha_id;
    char cha_name[STR_SHORT];
    Class * cha_class;
    lifeId active;
    Coord coords;
    Direction direction;
    int idAnim;
    int act_points;
    int base_stats[NUM_STATS];
    int stat_mods[NUM_STATS];
    int status_effect[NUM_STATUS];
    int ab_cooldown[NUM_AB];
} Entity;

/*ABILITY STRUCTURES*/

typedef struct
{
    int cha_id;
    bool visible;
} Trap_t;

/* TERRAIN STRUCTURES */

typedef struct
{
    int tile_id;
    int selected;
    int hovered;
    Entity * entity;
    Trap_t trap;
    int walkable;
} Tile;

/* TEXTURE STRUCTURES */

typedef struct
{
    char * texture_name;
    SDL_Texture *texture;
    SDL_Texture *big_texture;
} TabTexture;

typedef struct
{
    char * texture_name;
    SDL_Texture *front;
    SDL_Texture *textures[8][6]; // 4 directions and 2 different sizes -> 8 in total and 6 animation sprites
} CharTexture;

typedef struct
{
    abilityId id;
    SDL_Texture *spritesSmall[_NB_MAX_ANIM_];
    SDL_Texture *spritesBig[_NB_MAX_ANIM_];
    int nb_steps;
    bool aoe;
    bool on_ground;
    int speed;
    Mix_Chunk *sound_effect;
} AnimTexture;

typedef struct
{
    char *police;
    int size;
    char *content;
    SDL_Color color;
    SDL_Surface *surface;
    Uint32 time;
} TextCache;



/* COMMUNICATION STRUCTURES */

typedef struct User{
  int id;
  char pseudo[64];
} t_user;

typedef struct {
    int char_id;
    char cha_name[STR_SHORT];
    classId cha_class;
    Coord starting_position;
}init_ent;

typedef struct{
    int tile_id;
    int walkable;
}Tile_multi_t;

typedef struct{
    int isServerStartGame;
    char mapNameGame[50];
}ServerStatus_t;

#endif
