#ifndef COCCODRILLI_H
#define COCCODRILLI_H

#include "utilities.h"

void stampaCoccodrillo(Oggetto coccodrillo, WINDOW* window, int n);
void stampaCoccodrilli(Processo* coccodrilli, WINDOW* window);
void avviaCoccodrilli(Flusso* fiume, Processo* coccodrilli, int *pipe_fd);
void coccodrillo(Flusso flusso, int pipe_fd[], int j);
int fdelay(int speed);

#endif 