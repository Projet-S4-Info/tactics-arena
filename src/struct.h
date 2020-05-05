#ifndef struct_h
#define struct_h
/** \file struct.h
 * \brief Defining of major structures.
 */

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "state.h"
#include "substruct.h"

/* BASIC STRUCTURES*/

/** \struct Coord
 * \brief Structure representing a coordinate, used to navigate the gameboard.
 */
typedef struct
{
    int x; //!< X value of the coordinate
    int y; //!< Y value of the coordinate
} Coord;

/** \struct Vector
 * \brief Structure representing a vector.
 */
typedef struct
{
    float x; //!< X value of the vector
    float y; //!< Y value of the vector
} Vector;

/** \struct Timer
 * \brief
 */
typedef struct
{
    Uint32 start; //!<
    Uint32 end; //!<
} Timer;

/** \struct Log
 * \brief
 */
typedef struct
{
    char *message; //!<
    Uint32 time; //!<
} Log;

/** \struct chat_t
 * \brief
 */
typedef struct{
    char chatTab[_NB_MAX_CHAT_][STR_LONG]; //!<
    int index; //!<
}chat_t;

/** \struct music_t
 * \brief
 */
typedef struct{
    char *musicName; //!<
    int startFade; //!<
    Mix_Music *music_p; //!<
}music_t;

/* ENTITY STRUCTURES */

/** \struct action
 * \brief
 */
typedef struct {
    int char_id; //!<
    Coord c; //!<
    abilityId act; //Mvt if movement
} action;

/** \struct Ability
 * \brief
 */
typedef struct
{
    abilityId ab_id; //!<
    int ab_cost; //!<
    int ab_cooldown; //!<
    int range; //If zero then self cast
    targetType target; //!<
    Damage ** damage; //!<
    int nb_coords; //!<
    Coord ** coord; //!<
    int nb_mods; //!<
    Modifier ** mods; //!<
    fnid fn_use; //!<
    int (*function)(Coord, struct entity_t *, StateList *); //!<
    lang eng; //!<
} Ability;

/** \struct Class
 * \brief
 */
typedef struct
{
    classId cla_id; //!<
    char cla_name[STR_SHORT]; //!<
    int basic_stats[NUM_STATS]; //!<
    lang Passive; //!<
    Ability * cla_abilities; //!<
} Class;

/** \struct Entity
 * \brief Structure representing
 */
typedef struct entity_t
{
    int cha_id; //!<
    char cha_name[STR_SHORT]; //!<
    Class * cha_class; //!<
    lifeId active; //!<
    Coord coords; //!<
    Direction direction; //!<
    int idAnim; //!<
    int act_points; //!<
    int stats[NUM_STATS]; //!<
    int status_effect[NUM_STATUS]; //!<
    int ab_cooldown[NUM_AB]; //!<
} Entity;

/*ABILITY STRUCTURES*/

/** \struct Trap_t
 * \brief Structure representing the existence of a Trap.
 */
typedef struct
{
    int cha_id; //!< Trap Id. Is equal to zero if there is no trap, is positive if friendly trap, is negative if ennemy trap. Name of variable is cha_id for use in functions that compare Entity's teams.
    bool visible; //!< Visibility of the trap (is visible if same team or if flare found it).
} Trap_t;

/* TERRAIN STRUCTURES */

/** \struct Tile
 * \brief
 */
typedef struct
{
    int tile_id; //!<
    int selected; //!<
    int hovered; //!<
    Entity * entity; //!<
    Trap_t trap; //!<
    int walkable; //!<
} Tile;

/* TEXTURE STRUCTURES */

/** \struct TabTexture
 * \brief
 */
typedef struct
{
    char * texture_name; //!<
    SDL_Texture *texture; //!<
    SDL_Texture *big_texture; //!<
} TabTexture;

/** \struct CharTexture
 * \brief
 */
typedef struct
{
    char * texture_name; //!<
    SDL_Texture *front; //!<
    SDL_Texture *textures[8][6]; //!< 4 directions and 2 different sizes -> 8 in total and 6 animation sprites
} CharTexture;

/** \struct AnimTexture
 * \brief
 */
typedef struct
{
    abilityId id; //!<
    SDL_Texture *spritesSmall[_NB_MAX_ANIM_]; //!<
    SDL_Texture *spritesBig[_NB_MAX_ANIM_]; //!<
    int nb_steps; //!<
    bool aoe; //!<
    bool on_ground; //!<
    int speed; //!<
    Mix_Chunk *sound_effect; //!<
} AnimTexture;

/** \struct TextCache
 * \brief
 */
typedef struct
{
    char *police; //!<
    int size; //!<
    char *content; //!<
    SDL_Color color; //!<
    SDL_Surface *surface; //!<
    Uint32 time; //!<
} TextCache;

/* COMMUNICATION STRUCTURES */

/** \struct t_user
 * \brief
 */
typedef struct User{
  int id; //!<
  char pseudo[64]; //!<
} t_user;

/** \struct MultiTile
 * \brief
 */
typedef struct{
    int tile_id; //!<
}MultiTile;

/** \struct ServerStatus_t
 * \brief
 */
typedef struct{
    int isServerStartGame; //!<
    char mapNameGame[50]; //!<
    MultiTile multiMap[_X_SIZE_][_Y_SIZE_]; //!<
}ServerStatus_t;

#endif
