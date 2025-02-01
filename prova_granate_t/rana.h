#ifndef RANA_H
#define RANA_H

#include "utilities.h"
#include "buffer.h"

extern const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA];
extern const Thread quit;
extern const Thread spara;

void* funzioneRana(void * mutex);
void stampaRana(Oggetto r, WINDOW* w);
void inizializzaGranate(Thread* granate);
void avviaGranate(Thread rana, pthread_mutex_t* mutex);
void* granata(void* params);
void stampaGranata(Oggetto g, WINDOW* w);

#endif