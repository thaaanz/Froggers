#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <stdbool.h>
#include "rana.h"
#include "coccodrillo.h"

#include "utilities.h"



void avviaPipe(int pipe_fd[]);
void avviancurses();
void avviaRana(int* pipe_fd);
Flusso* avviaFlussi();