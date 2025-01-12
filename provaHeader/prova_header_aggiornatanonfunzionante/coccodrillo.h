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

#define MAX_COCCODRILLI 2



void stampaCoccodrilli(Processo * coccodrilli, WINDOW* window);
void avviaCoccodrilli(Flusso* fiume, Processo* pidCoccodrilli, int pipe_fd[]);
void coccodrillo(Flusso flusso, int pipe_fd[]);
int fdelay(int speed);