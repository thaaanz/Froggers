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

void avviaCoccodrilli(int* pipe_fd, Flusso* fiume,Processo* cricca)
{
    int i=0, n=0; 

    for(int k=0; k<NUMERO_FLUSSI*MAX_COCCODRILLI; k++)
    {
        cricca[k].pid=0;
        cricca[k].item.id='c';
        cricca[k].item.y=0;
        cricca[k].item.x=0;
        cricca[k].item.dir=0;
    }

    while(n<MAX_COCCODRILLI)
    {
        for(int f=0; f<NUMERO_FLUSSI; f++)
        {
            pid_t pid=generaCoccodrillo(pipe_fd, fiume[f], (rand()%4)); //!n è l'offset 
            cricca[i].pid=pid;
            //cricca[i].item.y = fiume[f].y + 1;
            //cricca[i].item.dir = fiume[f].dir;
            i++;
        }
        n++;
    }
}

pid_t generaCoccodrillo(int* pipe_fd, Flusso flusso, int offset)
{
    pid_t pid;
    pid=fork();
    if(pid<0)
    {
        perror("fork coccodrillo");
    }
    else if(pid==0)
    {
        funzioneCoccodrillo(pipe_fd, flusso, offset);
    }
    return pid;
}

void funzioneCoccodrillo(int* pipe_fd, Flusso flusso, int offset)
{
    close(pipe_fd[0]);
    
    //!questo è di claudio    
    int delay, starting_x;
    //Processo coccodrillo={getpid(), {'c', flusso.y+1, flusso.dir==DIR_RIGHT? -LARGHEZZA_COCCODRILLO : NCOLS, flusso.dir}};
    if (flusso.dir == DIR_RIGHT) {
        starting_x = -LARGHEZZA_COCCODRILLO - (offset * (NCOLS/MAX_COCCODRILLI));
    } else {
        starting_x = NCOLS + (offset * (NCOLS/MAX_COCCODRILLI));
    }
        
    Processo coccodrillo = {
        getpid(), 
        {'c', 
         flusso.y+1, 
         starting_x, 
         flusso.dir
        }
    };
    //!

    while(true)
    {
        write(pipe_fd[1], &coccodrillo, sizeof(Processo));
        coccodrillo.item.x+=coccodrillo.item.dir;

        if(coccodrillo.item.dir==DIR_RIGHT && coccodrillo.item.x>NCOLS) coccodrillo.item.x=-LARGHEZZA_COCCODRILLO;
        if(coccodrillo.item.dir==DIR_LEFT && coccodrillo.item.x<-LARGHEZZA_COCCODRILLO) coccodrillo.item.x=NCOLS;

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
                mvwaddch(wgioco, cricca[c].item.y+i, cricca[c].item.x+j, cricca[c].item.dir==DIR_RIGHT? spriteCoccodrilloDestra[i][j] : spriteCoccodrilloSinistra[i][j]); 
            } 
        }
    }
}
