#include "utilities.h"
#include "coccodrilli.h"

const char spriteCoccodrilloDestra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO]={
    ">>>>>>>>>>",
    ">>>>>>>>>>"
};

const char spriteCoccodrilloSinistra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO]={
    "<<<<<<<<<<",
    "<<<<<<<<<<"
};

Processo* avviaCoccodrilli(int* pipe_fd, Flusso* fiume)
{
    int i=0, n=0; 
    Processo cricca[NUMERO_FLUSSI*MAX_COCCODRILLI];

    while(n<MAX_COCCODRILLI)
    {
        for(int f=0; f<NUMERO_FLUSSI; f++)
        {
            pid_t pid=generaCoccodrillo(pipe_fd, fiume[f]); 
            cricca[i].pid=pid;
            i++;
        }
        n++;
    }
    return cricca;
}

pid_t generaCoccodrillo(int* pipe_fd, Flusso flusso)
{
    pid_t pid;
    pid=fork();
    if(pid<0)
    {
        perror("fork coccodrillo");
    }
    else
    {
        funzioneCoccodrillo(pipe_fd, flusso);
    }
    return pid;
}

void funzioneCoccodrillo(int* pipe_fd, Flusso flusso)
{
    close(pipe_fd[0]);
    int delay;
    Processo coccodrillo={getpid(), {'c', flusso.y+1, flusso.dir==DIR_RIGHT? -LARGHEZZA_COCCODRILLO : NCOLS, flusso.dir}};

    while(true)
    {
        write(pipe_fd[1], &coccodrillo, sizeof(Processo));
        coccodrillo.item.x+=coccodrillo.item.dir;

        if(coccodrillo.item.dir==DIR_RIGHT && coccodrillo.item.x>NCOLS) coccodrillo.item.x=-LARGHEZZA_COCCODRILLO;
        if(coccodrillo.item.dir==DIR_LEFT && coccodrillo.item.x<LARGHEZZA_COCCODRILLO) coccodrillo.item.x=NCOLS;

        delay=fdelay(flusso.speed);
        usleep(delay);
    }
}

int fdelay(int speed)
{
    switch(speed)
    {
        case 1: return 500000; break;
        case 2: return 400000; break;
        case 3: return 300000; break;
        case 4: return 200000; break;
        case 5: return 100000; break;
    }
}

void stampaCoccodrilli(Processo* cricca, WINDOW* wgioco)
{
    for(int c=0; c<(NUMERO_FLUSSI*MAX_COCCODRILLI); c++)
    {
        for (int i = 0; i < ALTEZZA_COCCODRILLO; i++) 
        { 
            for (int j = 0; j < LARGHEZZA_COCCODRILLO; j++) 
            {
                mvwaddch(wgioco, cricca->item.y+i, cricca->item.x+j, cricca->item.dir==DIR_RIGHT? spriteCoccodrilloDestra[i][j] : spriteCoccodrilloSinistra[i][j]); 
            } 
        }
        usleep(100000+(rand()%500000));
    }
}
