#ifndef COCCODRILLI_H
#define COCCODRILLI_H

#include "utilities.h"
#include "buffer.h"

extern const char spriteCoccodrilloDestra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO];
extern const char spriteCoccodrilloSinistra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO];

typedef struct {
    Oggetto item;
    Semafori* semafori;
    Flusso f;
    int offset;
} Cocco;



void avviaCoccodrilli(Flusso* fiume, Thread* cricca, Semafori* s);
//pthread_t generaCoccodrillo(Flusso flusso, int offset, Semafori* s, Oggetto coccodrillo);
void* funzioneCoccodrillo(void* parametri);
int fdelay(int speed);
void stampaCoccodrilli(Thread* cricca, WINDOW* wgioco);
void inizializzaProiettili(Thread* astuccio);
void* funzioneProiettile(void* parametri);
void stampaProiettili(Thread* astuccio, WINDOW* wgioco);


#endif