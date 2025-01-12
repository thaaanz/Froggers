#define ALTEZZA_RANA 2
#define LARGHEZZA_RANA 5

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <stdbool.h>

void rana(int pipe_fd[]);
void stampaRana(Oggetto pos, WINDOW* wgioco);