#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <time.h>

#include "avvia.h"


int main(){
    int pipe_fd[2];
    avviaPipe(pipe_fd);

    avviancurses();
    avviaGioco();
    return 0;
}