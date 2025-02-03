#ifndef AVVIA_H
#define AVVIA_H

#include "utilities.h"
#include "punteggio.h"
#include "menu.h"

//void avviaPipe(int* pipe_fd);
void avviancurses();

void cleanup(Thread rana, Thread* cricca, Thread* astuccio, Thread* granate, WINDOW* wgioco, WINDOW* whud, WINDOW* debug, WINDOW* wtempo);
Punteggio inizializzaPunteggio();
void avviaFlussi(Flusso* fiume);
#endif