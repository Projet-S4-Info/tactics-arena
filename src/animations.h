#ifndef animations_h
#define animations_h


/* =============== DEPENDENCES =============== */


#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "audio.h"
#include "struct.h"


/* =============== CONSTANTES =============== */





/* =============== VARIABLES =============== */

err_t addAnimTexture(abilityId id, char *name, int start_index, int end_index, bool aoe, bool on_ground, int speed);

SDL_Texture *getAnimTexture(abilityId id, int index, bool big);

int getAnimSteps(abilityId id);

err_t loadAnimationTextures();

err_t play_ability_animation(Ability ab, Coord pos);

bool isLoaded(abilityId id);

bool isOnGround(abilityId id);


#endif
