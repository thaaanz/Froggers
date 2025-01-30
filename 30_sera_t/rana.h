#ifndef RANA_H
#define RANA_H

#include "utilities.h"
#include "buffer.h"

extern const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA];
extern const Oggetto quit;
extern const Oggetto spara;

void* funzioneRana(void * mutex);
void stampaRana(Oggetto r, WINDOW* w);

#endif