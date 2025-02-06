#ifndef RANA_H
#define RANA_H

#include "utilities.h"
#include "buffer.h"

extern const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA];
extern const Thread quit;
extern const Thread spara;

Thread avviaRana (Semafori* semafori);
void* funzioneRana(void * semafori);
void stampaRana(Oggetto r, WINDOW* w);
void inizializzaGranate(Thread* granate);
void avviaGranate(Thread rana, Semafori* semafori);
void* granata(void* params);
void stampaGranate(Thread* granate, WINDOW* w);

#endif