#ifndef COCCODRILLI_H
#define COCCODRILLI_H

#include "utilities.h"

void avviaCoccodrilli(int* pipe_fd, Flusso* fiume, Processo* cricca);
pid_t generaCoccodrillo(int* pipe_fd, Flusso flusso, int offset);
void funzioneCoccodrillo(int* pipe_fd, Flusso flusso, int offset);
int fdelay(int speed);
void stampaCoccodrilli(Processo* cricca, WINDOW* wgioco);

extern const char spriteCoccodrilloDestra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO];
extern const char spriteCoccodrilloSinistra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO];

#endif 