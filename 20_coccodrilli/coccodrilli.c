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

void avviaCoccodrilli(int* pipe_fd, Flusso* fiume, Processo* cricca)
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
        exit(33);
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
    time_t last_shot=time(NULL);
    int shot_delay=(rand()%10)+5;
    int active_bullets=0;

    if (flusso.dir == DIR_RIGHT) starting_x = -LARGHEZZA_COCCODRILLO - (offset * (LARGHEZZA_COCCODRILLO*2));
    else starting_x = NCOLS + (offset * (LARGHEZZA_COCCODRILLO*2));
        
    Processo coccodrillo = {getpid(), {'c', flusso.y+1, starting_x, flusso.dir}};
    //!

    //signal(SIGCHLD, SIG_IGN);

    while(true)
    {
        write(pipe_fd[1], &coccodrillo, sizeof(Processo));
        coccodrillo.item.x+=coccodrillo.item.dir;

        if(coccodrillo.item.dir==DIR_RIGHT && coccodrillo.item.x>NCOLS) coccodrillo.item.x=-LARGHEZZA_COCCODRILLO;
        if(coccodrillo.item.dir==DIR_LEFT && coccodrillo.item.x<-LARGHEZZA_COCCODRILLO) coccodrillo.item.x=NCOLS;

        if(active_bullets<MAX_BULLETS_PER_COCCO && time(NULL)-last_shot>=shot_delay)
        {
            pid_t pid_proiettile=funzioneProiettile(pipe_fd, coccodrillo);
            
            if(pid_proiettile>0)
            {
                active_bullets++;
                last_shot=time(NULL);
                shot_delay=(rand()%10)+2;
            }
        }

        //active_bullets=0;
        int status;
        while(waitpid(-1, &status, WNOHANG)>0)
        {
            if(WIFEXITED(status))
            {
                active_bullets--;
                if(active_bullets<0) active_bullets=0;
            }
        }

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

//todo proiettili

void avviaProiettili(Processo* astuccio)
{
    for(int i=0; i<N_PROIETTILI; i++)
    {
        astuccio[i].pid=-1;
        astuccio[i].item.id='-';
        astuccio[i].item.y=0;
        astuccio[i].item.x=0;
        astuccio[i].item.dir=0;
    }
}

pid_t funzioneProiettile(int* pipe_fd, Processo coccodrillo) //* meglio passare un oggetto?
{
    pid_t pid=fork();
    if(pid<0)
    {
        perror("fork proiettile");
        return -1;
    }
    else if(pid==0)
    {
        close(pipe_fd[0]);
        Processo proiettile={getpid(), {'-', coccodrillo.item.y+1, coccodrillo.item.x+(coccodrillo.item.dir==DIR_RIGHT? LARGHEZZA_COCCODRILLO : 0), coccodrillo.item.dir}};
        
        time_t start=time(NULL);
        
        while(proiettile.item.x>0 && proiettile.item.x<NCOLS)
        {
            write(pipe_fd[1], &proiettile, sizeof(Processo));
            proiettile.item.x+=proiettile.item.dir;
            //if(proiettile.item.x<0||proiettile.item.x>NCOLS)
            usleep(DELAY/2);
        }

        proiettile.item.dir=0;
        write(pipe_fd[1], &proiettile, sizeof(Processo));
        _exit(5);
    }
    return pid;
}

void stampaProiettile(Oggetto proiettile, WINDOW* wgioco)
{
    mvwaddch(wgioco, proiettile.y, proiettile.x, proiettile.id);
}
