#ifndef struct_h
#define struct_h
/** \file struct.h
 * \brief Defining major structures.
 * \author Robin BUTEL, Thibault DOUCET, Lucien CHAUVIN
 * \details Code used to define major structures seperated into multiple categories : BASIC, ENTITY, ABILITY, TERRAIN, TEXTURE and COMMUNICATION.
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
 * \details Is the same as Coord but wtih floats instead of ints.
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
 * \brief Structure representing necessary information for the communication of actions done, wether between functions, threads, or players.
 */
typedef struct {
    int char_id;    //!< Character Id of the caster/user.
    Coord c;        //!< Coordinate where the action was used.
    abilityId act;  //!< Id of the action/ability cast (use %NUM_AB to find number local to selected Entity).
} action;

/** \struct Ability
 * \brief Structure representing a castable ability.
 */
typedef struct
{
    abilityId ab_id;                                        //!< Ability Id.
    int ab_cost;                                            //!< Ability cost in action points (act_points).
    int ab_cooldown;                                        //!< Ability Cooldown.
    int range;                                              //!< Ability Range, used alongside caster's vision to determine appliable range.
    targetType target;                                      //!< Indicates which tiles can be targeted by this ability.
    Damage ** damage;                                       //!< A double pointer to this ability's damage structure.
    int nb_coords;                                          //!< Ability zone size, number of coordinates where ability will be applied, used to navigate through coord.
    Coord ** coord;                                         //!< A double pointer to this ability's list of coordinates which indicate it's zone.
    int nb_mods;                                            //!< Ability's number of modifiers, used to navigate through mods.
    Modifier ** mods;                                       //!< A double pointer to this ability's list of applicable modifiers.
    fnid fn_use;                                            //!< Indicates when to use special ability function method.
    int (*function)(Coord, struct entity_t *, StateList *); //!< Method that allows special behavior to happen during casting of ability. NULL if none is required.
    lang eng;                                               //!< Name and Desription of Ability in English.
} Ability;

/** \struct Class
 * \brief Structure representing the classes of the characters.
 */
typedef struct
{
    classId cla_id;                 //!< Class Id.
    char cla_name[STR_SHORT];       //!< Class Name.
    int basic_stats[NUM_STATS];     //!< Class' basic stats arranged in an array respecting the order of statId.
    lang Passive;                   //!< Name and Description of that class' passive ability.
    Ability * cla_abilities;        //!< Pointer to class' abilities.
} Class;

/** \struct entity_t
 * \brief  Defined as Entity. Structure representing the characters played by the player and his opponent.
 */
typedef struct entity_t
{
    int cha_id;                         //!< Character Id, positive if ally, negative if ennemy, is equal to classId + 1.
    char cha_name[STR_SHORT];           //!< Character Name.
    Class * cha_class;                  //!< Pointer to Character Class.
    lifeId active;                      //!< Boolean indicating if the character is alive or dead.
    Coord coords;                       //!< Coord indicating character's current position on the gameboard.
    Direction direction;                //!< Indicates direction character is facing.
    int idAnim;                         //!< Indicates animation stage.
    int act_points;                     //!< Number of action points (3 normally, 1 if paralyzed).
    int stats[NUM_STATS];               //!< Character Stats arranged in an array respecting the order of statId.
    int status_effect[NUM_STATUS];      //!< Array indicating current status_effects of character, respecting the order of statusId.
    int ab_cooldown[NUM_AB];            //!< Array indicating current ability cooldowns respecting the order of abilityId.
} Entity;
/** \typedef Entity
 * \brief Structure representing the characters played by the player and his opponent.
 */

/*ABILITY STRUCTURES*/

/** \struct Trap_t
 * \brief Structure representing the existence of a Trap.
 * \details Each tile of the gameboard will have a Trap_t structure.
 */
typedef struct
{
    int cha_id;     //!< Trap Id. Is equal to zero if there is no trap, is positive if friendly trap, is negative if ennemy trap. Name of variable is cha_id for use in functions that compare Entity's teams.
    bool visible;   //!< Visibility of the trap (is visible if same team or if flare found it).
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

/** \struct User
 * \brief
 */
typedef struct User{
  int id; //!<
  char pseudo[64]; //!<
} t_user;
/** \typedef t_user
 * \brief
 */

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
