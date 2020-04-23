
/* =============== DEPENDENCES =============== */

#include <stdio.h>
#include <math.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "audio.h"
#include "game_window.h"
#include "characters.h"
#include "common.h"
#include "grid.h"
#include "gameplay.h"
#include "turn.h"
#include "display.h"
#include "textures.h"
#include "init.h"
#include "string.h"
#include "border.h"
#include "text.h"
#include "struct.h"
#include "map.h"

/* =============== CONSTANTES =============== */

#define _NB_MAX_TEXTURES_ 50

/* =============== VARIABLES =============== */

/* =============== FONCTIONS DE GESTION DES TEXTURES D'ANIMATIONS =============== */

err_t addAnimTexture(abilityId id, char *name, int start_index, int end_index, bool on_ground)
// Pre-load animation textures for future loading
{
    int index = 0;
    int tabIndex = 0;
    char tempSmall[STR_LONG];
    char tempBig[STR_LONG];

    while (animTextures[index].spritesSmall[0] != NULL)
    {
        index++;
    }

    if (verbose == 2)
        printf("\033[36;01m[ANIMATIONS]\033[00m : Ajout de la texture [%s] à l'id %d\n", name, index);

    animTextures[index].id = id;
    animTextures[index].nb_steps = end_index - start_index + 1;
    animTextures[index].on_ground = on_ground;

    for (int i = start_index; i <= end_index; i++)
    {
        sprintf(tempSmall, "../inc/sprites/attacks/sprite_indiv/64_64/%s%d.png", name, i);
        sprintf(tempBig, "../inc/sprites/attacks/sprite_indiv/128_128/%s%d.png", name, i);
        if (verbose == 2)
            printf("\033[36;01m[ANIMATIONS]\033[00m : Chargement de la texture : %s\n", tempSmall);
        animTextures[index].spritesSmall[tabIndex] = loadTexture(renderer, loadImage(tempSmall));
        /*if (verbose == 2) printf("\033[36;01m[ANIMATIONS]\033[00m : Ajout de la texture [%s] à l'id %d\n", name, index);
        animTextures[index].spritesBig[tabIndex] = loadTexture(renderer, loadImage(tempBig));*/
        tabIndex++;
    }

    return OK;
}

SDL_Texture *getAnimTexture(abilityId id, int index, bool big)
// Returns the texture of a given animation
{
    SDL_Texture *result = NULL;

    for (int i = 0; i < NB_AB; i++)
    {
        if (id == animTextures[i].id)
        {
            if (big)
                result = animTextures[i].spritesBig[index];
            else
                result = animTextures[i].spritesSmall[index];
            break;
        }
    }

    if (result == NULL)
    {
        printf("\033[31;01m[ANIMATIONS ERROR]\033[00m : Aucune animation trouvée pour l'abilité ID %d - Chargement de la texture impossible\n", id);
        exit(EXIT_FAILURE);
    }

    return result;
}

int getAnimSteps(abilityId id)
// Returns the number of sprites for the animation of the given ability id
{
    int result = -1;

    for (int i = 0; i < NB_AB; i++)
    {
        if (id == animTextures[i].id)
        {
            result = animTextures[i].nb_steps;
            break;
        }
    }

    if (result == -1)
    {
        printf("\033[31;01m[ANIMATIONS ERROR]\033[00m : Aucune animation trouvée pour l'abilité ID %d - Aucun retour sur le nombre d'animations\n", id);
        exit(EXIT_FAILURE);
    }

    return result;
}

AnimTexture getAnim(abilityId id)
// Return the ability from an id
{
    AnimTexture result;
    bool found = FALSE;

    for (int i = 0; i < NB_AB; i++)
    {
        if (id == animTextures[i].id)
        {
            result = animTextures[i];
            found = TRUE;
            break;
        }
    }

    if (found == FALSE)
    {
        printf("\033[31;01m[ANIMATIONS ERROR]\033[00m : Aucune animation trouvée pour l'abilité ID %d - Aucune animation chargée\n", id);
        exit(EXIT_FAILURE);
    }

    return result;
}

err_t loadAnimationTextures()
// Load all the animations relative textures
{
    // Ranger abilities animations
    addAnimTexture(Bolt, "ranger/bolt", 64, 69, FALSE);
    addAnimTexture(Focus, "ranger/focus_aura", 64, 68, FALSE);
    addAnimTexture(Deadeye, "ranger/eye", 64, 69, FALSE);

    // Berserker abilities animations
    addAnimTexture(Frenzied_Dash, "berserker/earthquake_ultimate", 64, 68, TRUE);

    return OK;
}

bool isLoaded(abilityId id)
// Returns if the given ability has been loaded
{
    bool result = FALSE;

    for (int i = 0; i < NB_AB && result == FALSE; i++)
    {
        if (id == animTextures[i].id)
        {
            result = TRUE;
        }
    }

    return result;
}

bool isOnGround(abilityId id)
// Returns if the animation have to be played on the ground or not
{
    return getAnim(id).on_ground;
}

/* =============== FONCTIONS D'AFFICHAGE DES ANIMATIONS =============== */

err_t play_ability_animation(Ability ab, Coord pos)
{
    Coord temp;
    if (!isOnGround(ab.ab_id))
    {
        pos.x--;
        pos.y++;
    }

    displayMap(renderer, XPOS, YPOS);

    for (int i = 0; i < getAnimSteps(ab.ab_id); i++)
    {
        for (int j = 0; j < ab.nb_coords; j++)
        {
            Coord drawPos = add_coords(pos, *((*(ab.coord))+j));
            if (isWalkable(drawPos) && isInGrid(drawPos))
            {
                temp = to2D(drawPos);
                if (pxBase == 64)
                    displaySprite(renderer, getAnimTexture(ab.ab_id, i, FALSE), temp.x, temp.y);
                else
                    displaySprite(renderer, getAnimTexture(ab.ab_id, i, TRUE), temp.x, temp.y);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    return OK;
}