#include "utilities.h"
#include "avvia.h"
#include "rana.h"
#include "controllo.h"

int  main()
{
    avviancurses();

    int pipe_fd[2];
    int pipe_inversa[2];
    avviaPipe(pipe_fd);
    avviaPipe(pipe_inversa);

    controllo(pipe_fd, pipe_inversa);
    endwin();
}
