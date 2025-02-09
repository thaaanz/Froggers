#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

//definisco una struttura GameAudio che contiene tutti i suoni del gioco
typedef struct{
    Mix_Music* musica_gioco;
    Mix_Music* musica_menu;
    Mix_Chunk* salto_rana;
    Mix_Chunk* morte_rana;
    Mix_Chunk* sparo_granata;
    Mix_Chunk* chiusura_tana;
    Mix_Chunk* freccia_menu;
} GameAudio;

void inizializzaAudio(GameAudio* audio);
void riproduciMusica(Mix_Music* musica);
void riproduciSuono(Mix_Chunk* suono);
void freeAudio(GameAudio* audio);

#endif