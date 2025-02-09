#ifndef RANA_H
#define RANA_H

#include "utilities.h"

extern const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA];
extern const Processo quit;
extern const Processo spara;

//rana
Processo avviaRana(int* pipe_fd, int* pipe_inversa);
void funzioneRana(int* pipe_fd, int* pipe_inversa);
void stampaRana(Oggetto r, WINDOW* w);

//granate
void inizializzaGranate(Processo* granate);
void avviaGranate(int* pipe_fd, int* pipe_inversa);
void granata(int direzione, int* pipe_fd, Processo* rana);
void stampaGranate(Processo* g, WINDOW* w);

#endif