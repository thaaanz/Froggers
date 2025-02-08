#ifndef AVVIA_H
#define AVVIA_H

#include "utilities.h"
#include "punteggio.h"
#include "menu.h"

//void avviaPipe(int* pipe_fd);
void avviancurses();

void cleanup(Thread rana, Thread* cricca, Thread* astuccio, Thread* granate);
void avviaFlussi(Flusso* fiume);
Oggetto* inizializzaMine(Oggetto* mine);
#endif