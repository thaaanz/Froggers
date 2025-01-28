#include "avvia.h"

void avviaPipe(int* pipe_fd)
{
    if(pipe(pipe_fd)==-1)
    {
        perror("pipe fallita");
        exit(1);
    }
}

void avviancurses()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
}

void cleanup(Processo rana, Processo* cricca, Processo* astuccio, Processo* granate)
{
    if(rana.pid>1) kill(rana.pid, 9);
    for(int i=0; i<NUMERO_FLUSSI*MAX_COCCODRILLI; i++)
    {
        if(cricca[i].pid>1) (cricca[i].pid, 9);
        waitpid(cricca[i].pid, NULL, 0);
    }
    for(int i=0; i<N_PROIETTILI; i++)
    {
        waitpid(astuccio[i].pid, NULL, 0);
    }
    for(int i=0; i<N_GRANATE; i++)
    {
        waitpid(granate[i].pid, NULL, 0);
    }
}