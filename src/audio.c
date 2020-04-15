#include <stdio.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "common.h"

Mix_Music* myMus;
Mix_Music* multiMus;

int playMenuMusic(int nb)
// Play the menu music
{


    /* Initialisation */
    if (SDL_Init(SDL_INIT_AUDIO) != 0 ) {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

    if(verbose)printf("[SDL] Audio driver: %s\n", SDL_GetCurrentAudioDriver());
    
    int i, count = SDL_GetNumAudioDevices(0);
    
     /*-- Boucle debug audio --*/
     for (i = 0; i < count; ++i) {
         printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    }


    /* On ouvre le device audio */
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) 
    {
        printf("%s", Mix_GetError());
    }

     /* On charge la musique */
    myMus = Mix_LoadMUS("../inc/music/cascade.wav");
    multiMus = Mix_LoadMUS("../inc/music/Battle-Chiptune.wav");

    /* On lance la musique */
    if(nb == 1){
        Mix_PlayMusic(myMus, 1);
    }else if(nb == 2){
        Mix_PlayMusic(multiMus, -1);
    }
    return 0;
}

int stopMenuMusic(int nb)
// Stop the menu music
{
     /* La musique est terminée, on la libère */
    if(nb == 1){
        Mix_FreeMusic(myMus);
    }else if(nb == 2){
        Mix_FreeMusic(multiMus);
    }


    /* On libère le matériel */
    Mix_CloseAudio();

    Mix_Quit();

    return 0;
}

int pauseMenuMusic()
// Pause menu music
{
    if(verbose)printf("[AUDIO] Music paused\n");
    Mix_PauseMusic();
    return 1;
}

int resumeMenuMusic()
// Resume menu music
{
    if(verbose)printf("[AUDIO] Music resumed\n");
    Mix_ResumeMusic();
    return 1;
}