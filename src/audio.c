#include <stdio.h>
#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_mixer.h"

Mix_Music* myMus;

int playMenuMusic()
// Play the menu music
{


    /* Initialisation */
    if (SDL_Init(SDL_INIT_AUDIO) != 0 ) {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

    printf("[SDL] Audio driver: %s\n", SDL_GetCurrentAudioDriver());
    
    int i, count = SDL_GetNumAudioDevices(0);
    
    /* -- Boucle debug audio --*/
    // for (i = 0; i < count; ++i) {
    //     printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    // }


    /* On ouvre le device audio */
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) 
    {
        printf("%s", Mix_GetError());
    }

    /* On charge la musique */
    myMus = Mix_LoadMUS("../inc/music/cascade.wav");

    /* On lance la musique */
    Mix_PlayMusic(myMus, 1);

    return 0;
}

int stopMenuMusic()
// Stop the menu music
{
     /* La musique est terminée, on la libère */
    Mix_FreeMusic(myMus);

    /* On libère le matériel */
    Mix_CloseAudio();

    Mix_Quit();

    return 0;
}

int pauseMenuMusic()
// Pause menu music
{
    printf("[AUDIO] Music paused\n");
    Mix_PauseMusic();
    return 1;
}

int resumeMenuMusic()
// Resume menu music
{
    printf("[AUDIO] Music resumed\n");
    Mix_ResumeMusic();
    return 1;
}