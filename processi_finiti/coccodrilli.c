#include "coccodrilli.h"

//GESTIONE COCCODRILLI

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
    for(int k=0; k<NUMERO_FLUSSI*MAX_COCCODRILLI_PER_FLUSSO; k++) //inizializza l'array di coccodrilli
    {
        cricca[k].pid=0;
        cricca[k].item.id='c';
        cricca[k].item.y=0;
        cricca[k].item.x=0;
        cricca[k].item.dir=0;
    }

    int i=0; //inizializzo un indice che scorre la totalità dei coccodrilli

    for(int n=0; n < MAX_COCCODRILLI_PER_FLUSSO; n++) //scorre i coccodrilli per ogni flusso
    {
        for(int f=0; f<NUMERO_FLUSSI; f++) //scorre i flussi
        {
            pid_t pid=generaCoccodrillo(pipe_fd, fiume[f], (rand()%4)); //genero i singoli coccodrilli 
            //passo un offset random per fare in modo che i coccodrilli vengano generati a distanze diverse tra loro
            cricca[i].pid=pid; //salvo il pid dei coccodrilli nell'array
            i++; //incremento l'indice
        }
    }
}

pid_t generaCoccodrillo(int* pipe_fd, Flusso flusso, int offset)
{
    pid_t pid=fork();
    if(pid<0) //fork fallita
    {
        perror("fork coccodrillo");
        exit(ERRORE_FORK_COCCODRILLI);
    }
    else if(pid==0) //processo figlio
    {
        funzioneCoccodrillo(pipe_fd, flusso, offset); //gestisce un singolo coccodrillo
    }
    return pid;
}

void funzioneCoccodrillo(int* pipe_fd, Flusso flusso, int offset) 
{
    close(pipe_fd[0]); //chiudo la pipe in lettura
    int delay, starting_x; //delay per la velocità del coccodrillo(in base a quella del flusso) e x di partenza (in base all'offset)
    time_t last_shot=time(NULL); //ultimo sparo del coccodrillo
    int shot_delay=(rand()%MAX_INTERVALLO_PROIETTILI)+1; //delay tra uno sparo e l'altro

    if(flusso.dir == DIR_RIGHT) //se il coccodrillo deve andare a destra
    {
        starting_x = -LARGHEZZA_COCCODRILLO - (offset * (LARGHEZZA_COCCODRILLO*2)); //il coccodrillo parte dalla sinistra dello schermo
    }
    else 
    {
        starting_x = NCOLS + (offset * (LARGHEZZA_COCCODRILLO*2)); //altrimenti parte da destra
    }
        
    Processo coccodrillo = {getpid(), {'c', flusso.y+1, starting_x, flusso.dir}}; //creo il coccodrillo con le coordinate corrette

    while(true) //ciclo per l'aggiornamento della posizione del coccodrillo
    {
        write(pipe_fd[1], &coccodrillo, sizeof(Processo)); //scrivo nella pipe

        coccodrillo.item.x+=coccodrillo.item.dir; //incremento la x di 1 o -1 a seconda la direzione

        //wrap around: quando il coccodrillo esce dallo schermo aggiorno la sua posizione per farlo riapparire dalla parte opposta
        if(coccodrillo.item.dir==DIR_RIGHT && coccodrillo.item.x > NCOLS) coccodrillo.item.x=-LARGHEZZA_COCCODRILLO;
        if(coccodrillo.item.dir==DIR_LEFT && coccodrillo.item.x < -LARGHEZZA_COCCODRILLO) coccodrillo.item.x=NCOLS;

        if(time(NULL)-last_shot>=shot_delay) //se è trascorso abbastanza tempo dall'ultimo sparo
        {
            pid_t pid_proiettile=funzioneProiettile(pipe_fd, coccodrillo); //sparo un nuovo proiettile
            
            if(pid_proiettile>0) //se il proiettile è valido
            {
                last_shot=time(NULL); //aggiorno l'ultimo sparo
                shot_delay=(rand()%MAX_INTERVALLO_PROIETTILI)+5; //aggiorno l'intervallo random per il prossimo sparo, almeno 5 secondi
            }
        }
        delay=fdelay(flusso.speed); //imposto il delay dell'aggiornamento in base alla velocità del flusso
        usleep(delay); 
    }
}

int fdelay(int speed)
{
    switch(speed) //controllo la velocità del flusso e restituisco la velocità dei coccodrilli di conseguenza
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
    wattron(wgioco, COLOR_PAIR(COLORI_COCCODRILLO));
    for(int c=0; c<(NUMERO_FLUSSI*MAX_COCCODRILLI_PER_FLUSSO); c++)
    {
        for (int i = 0; i < ALTEZZA_COCCODRILLO; i++) 
        { 
            for (int j = 0; j < LARGHEZZA_COCCODRILLO; j++) 
            {
                mvwaddch(wgioco, cricca[c].item.y+i, cricca[c].item.x+j, cricca[c].item.dir==DIR_RIGHT? spriteCoccodrilloDestra[i][j] : spriteCoccodrilloSinistra[i][j]); 
                //stampo i coccodrilli, uso l'operatore ternario per scegliere la sprite giusta in base alla direzione
            } 
        }
    }
    wattroff(wgioco, COLOR_PAIR(COLORI_COCCODRILLO));
}

//GESTIONE PROIETTILI

void inizializzaProiettili(Processo* astuccio) //inizializza l'array di proiettili
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

pid_t funzioneProiettile(int* pipe_fd, Processo coccodrillo)
{
    pid_t pid=fork();
    if(pid < 0) //fork fallita
    {
        perror("fork proiettile");
        exit(ERRORE_FORK_PROIETTILI);
    }
    else if(pid==0) //processo figlio
    {
        close(pipe_fd[0]); //chiudo la pipe in lettura
        //creo il proiettile con la y del coccodrillo e la x impostata in base alla direzione del coccodrillo(il proiettile parte dalla sinistra del coccodrillo se va verso destra e viceversa)
        Processo proiettile={getpid(), {'-', coccodrillo.item.y+1, coccodrillo.item.x+(coccodrillo.item.dir==DIR_RIGHT? LARGHEZZA_COCCODRILLO : 0), coccodrillo.item.dir}};
        
        while(true)
        {
            write(pipe_fd[1], &proiettile, sizeof(Processo)); //scrivo nella pipe
            proiettile.item.x+=proiettile.item.dir; //aggiorno la x del proiettile
            usleep(DELAY_PROIETTILE);
        }
    }
    return pid;
}

void stampaProiettili(Processo* astuccio, WINDOW* wgioco)
{
    wattron(wgioco, COLOR_PAIR(COLORI_PROIETTILI));
    wattron(wgioco, A_BOLD); //per la stampa in grassetto
    for (int i = 0; i < N_PROIETTILI; i++) //scorro l'array
        {
            if(astuccio[i].pid != -1) //se la cella contiene un proiettile 
            {
                mvwaddch(wgioco, astuccio[i].item.y, astuccio[i].item.x, astuccio[i].item.id); //lo stampo
            }
        }
    
    wattroff(wgioco, A_BOLD);
    wattroff(wgioco, COLOR_PAIR(COLORI_PROIETTILI));
}