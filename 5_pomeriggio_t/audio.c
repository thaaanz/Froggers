#include "audio.h"

void inizializzaAudio(GameAudio* audio)
{
    
    if(SDL_Init(SDL_INIT_AUDIO)<0) //inizializza sdl
        return -1;

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)) //inizializza il mixer
        return -1;

    Mix_AllocateChannels(4); //alloca 4 canali audio per i suoni riprodotti contemporaneamente

    audio->musica=Mix_LoadMUS("supermario.mp3");

    audio->salto_rana=Mix_LoadWAV("salto_rana.wav");
    audio->morte_rana=Mix_LoadWAV("morte_rana.wav");
    audio->sparo_granata=Mix_LoadWAV("sparo_granata.wav");
    audio->chiusura_tana=Mix_LoadWAV("chiusura_tana.wav");

    Mix_VolumeMusic(60); //da 0 a 128
    Mix_Volume(-1, 80); //-1 perchè è per tutti i canali
}

void riproduciMusica(GameAudio* audio)
{
    //if (Mix_PlayingMusic() == 0)
        Mix_PlayMusic(audio->musica, -1); //-1 per loop infinito
}

void riproduciSuono(Mix_Chunk* suono)
{
    Mix_PlayChannel(-1, suono, 0); //-1 per il primo canale disponibile, 0 per riprodurre il suono solo una volta
}

void freeAudio(GameAudio* audio) 
{
    Mix_FreeMusic(audio->musica);
    Mix_FreeChunk(audio->salto_rana);
    Mix_FreeChunk(audio->morte_rana);
    Mix_FreeChunk(audio->sparo_granata);
    Mix_FreeChunk(audio->chiusura_tana);

    //SDL_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    Mix_CloseAudio();
}