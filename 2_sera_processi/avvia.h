#ifndef AVVIA_H
#define AVVIA_H

#include "utilities.h"
#include "punteggio.h"
#include "menu.h"

void avviaPipe(int* pipe_fd);
void avviancurses();

void cleanup(Processo rana, Processo* cricca, Processo* astuccio, Processo* granate);
Flusso* avviaFlussi();
Punteggio inizializzaPunteggio();
#endif