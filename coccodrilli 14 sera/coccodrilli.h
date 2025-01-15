#ifndef COCCODRILLI_H
#define COCCODRILLI_H

#include "utilities.h"

void stampaCoccodrillo(Oggetto coccodrillo, WINDOW* window, int n);
void stampaCoccodrilli(Processo* coccodrilli, WINDOW* window);
void avviaCoccodrilli(Flusso* fiume, Processo* coccodrilli, int *pipe_fd, WINDOW* wgioco);
void coccodrillo(Flusso flusso, int pipe_fd[], int j, WINDOW* window);
int fdelay(int speed);
_Bool isCarattereCoccodrillo(char c);

#endif 