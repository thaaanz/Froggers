#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <stdbool.h>

#include "utilities.h"

#define MAX_COCCODRILLI 6


void stampaCoccodrillo(Oggetto coccodrillo, WINDOW* window, int n);
void stampaCoccodrilli(Processo* coccodrilli[], WINDOW* window);
void avviaCoccodrilli(Flusso* fiume, Processo* coccodrilli[], int pipe_fd[], WINDOW* w);
void coccodrillo(Flusso flusso, int pipe_fd[]);
int fdelay(int speed);