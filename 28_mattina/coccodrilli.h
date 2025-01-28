#ifndef COCCODRILLI_H
#define COCCODRILLI_H

#include "utilities.h"

//coccodrilli

extern const char spriteCoccodrilloDestra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO];
extern const char spriteCoccodrilloSinistra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO];

void avviaCoccodrilli(int* pipe_fd, Flusso* fiume, Processo* cricca);
pid_t generaCoccodrillo(int* pipe_fd, Flusso flusso, int offset);
void funzioneCoccodrillo(int* pipe_fd, Flusso flusso, int offset);
int fdelay(int speed);
void stampaCoccodrilli(Processo* cricca, WINDOW* wgioco);

//proiettili
void avviaProiettili(Processo* astuccio);
pid_t funzioneProiettile(int* pipe_fd, Processo coccodrillo);
void stampaProiettile(Oggetto proiettile, WINDOW* wgioco);

#endif 