#ifndef COCCODRILLI_H
#define COCCODRILLI_H

#include "utilities.h"

Processo* avviaCoccodrilli(int* pipe_fd, Flusso* fiume);
pid_t generaCoccodrillo(int* pipe_fd, Flusso flusso);
void funzioneCoccodrillo(int* pipe_fd, Flusso flusso);
int fdelay(int speed);
void stampaCoccodrilli(Processo* cricca, WINDOW* wgioco);

extern const char spriteCoccodrilloDestra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO];
extern const char spriteCoccodrilloSinistra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO];

#endif 