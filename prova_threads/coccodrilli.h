#ifndef COCCODRILLI_H
#define COCCODRILLI_H

#include "utilities.h"
#include "buffer.h"

typedef struct {
    Oggetto item;
    pthread_mutex_t mutex;
    Flusso f;
    int offset;
} Cocco;

void avviaCoccodrilli(Flusso* fiume, Thread* cricca, pthread_mutex_t m);
pthread_t generaCoccodrillo(Flusso flusso, int offset, pthread_mutex_t m, Oggetto coccodrillo);
void* funzioneCoccodrillo(void* parametri);
int fdelay(int speed);
void stampaCoccodrilli(Thread* cricca, WINDOW* wgioco);

#endif