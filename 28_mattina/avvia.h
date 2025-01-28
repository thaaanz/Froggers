#ifndef AVVIA_H
#define AVVIA_H

#include "utilities.h"
#include "punteggio.h"

#define ALTEZZA_MENU 4
#define LARGHEZZA_MENU 31

extern const char spriteMenu[ALTEZZA_MENU][LARGHEZZA_MENU];

void avviaPipe(int* pipe_fd);
void avviancurses();

void cleanup(Processo rana, Processo* cricca, Processo* astuccio, Processo* granate);
Punteggio inizializzaPunteggio();
#endif