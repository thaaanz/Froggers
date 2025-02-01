#include "coccodrilli.h"

const char spriteCoccodrilloDestra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO]={
    ">>>>>>>>>>",
    ">>>>>>>>>>"
};

const char spriteCoccodrilloSinistra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO]={
    "<<<<<<<<<<",
    "<<<<<<<<<<"
};

void avviaCoccodrilli(Flusso* fiume, Thread* cricca, pthread_mutex_t m)
{
    int i=0, n=0; 

    for(int k=0; k<NUMERO_FLUSSI*MAX_COCCODRILLI; k++) //inizializzo tutto l'array
    {
        cricca[k].tid=0;
        cricca[k].item.id='c';
        cricca[k].item.y=0;
        cricca[k].item.x=0;
        cricca[k].item.dir=0;
    }

    while(n<MAX_COCCODRILLI) //n scorre i coccodrilli per ogni riga
    {
        for(int f=0; f<NUMERO_FLUSSI; f++) //f scorre i flussi
        {
            pthread_t tid=generaCoccodrillo(fiume[f], (rand()%4), m, cricca[i].item); //random è l'offset per la partenza dei coccodrilli
            cricca[i].tid=tid; //genero il tid e lo assegno a ogni coccodrillo nell'array
            i++; //i scorre i coccodrilli in ogni flusso (verticalmente)
        }
        n++;
    }
}

pthread_t generaCoccodrillo(Flusso flusso, int offset, pthread_mutex_t m, Oggetto coccodrillo)
{
    pthread_t tid;
    Cocco c={coccodrillo, m, flusso, offset}; //inserisco i dati nella struttura
    
    pthread_create(&tid, NULL, &funzioneCoccodrillo, &c); //creo il thread e gli passo la funzione 

    return tid; //restituisco il tid per metterlo nell'array
}

void* funzioneCoccodrillo(void* parametri)
{
    Cocco c=*(Cocco*) parametri; //cast dell'argomento della thread function

    int delay, starting_x; //velocità e x di partenza del coccodrillo

    if (c.f.dir == DIR_RIGHT) starting_x = -LARGHEZZA_COCCODRILLO - (c.offset * (LARGHEZZA_COCCODRILLO*2)); //se il coccodrillo va verso destra
    else starting_x = NCOLS + (c.offset * (LARGHEZZA_COCCODRILLO*2)); //se va verso sinistra
        
    Thread coccodrillo = {gettid(), {'c', c.f.y+1, starting_x, c.f.dir}};

    while(true)
    {
        scrivi(&c.mutex, coccodrillo); //scrivo sul buffer
        coccodrillo.item.x+=coccodrillo.item.dir; //aggiorno la posizione

        //se il coccodrillo esce dallo schermo viene rigenerato dall'altra parte
        if(coccodrillo.item.dir==DIR_RIGHT && coccodrillo.item.x>NCOLS) coccodrillo.item.x=-LARGHEZZA_COCCODRILLO;
        if(coccodrillo.item.dir==DIR_LEFT && coccodrillo.item.x<-LARGHEZZA_COCCODRILLO) coccodrillo.item.x=NCOLS;

        delay=fdelay(c.f.speed); //stabilisco il delay in base al flusso
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

void stampaCoccodrilli(Thread* cricca, WINDOW* wgioco)
{
    wattron(wgioco, COLOR_PAIR(COLORI_COCCODRILLO));

    for(int c=0; c<(NUMERO_FLUSSI*MAX_COCCODRILLI); c++) //scorro i coccodrilli
    {
        for (int i = 0; i < ALTEZZA_COCCODRILLO; i++) //stampo le righe
        { 
            for (int j = 0; j < LARGHEZZA_COCCODRILLO; j++) //colonne
            {
                mvwaddch(wgioco, cricca[c].item.y+i, cricca[c].item.x+j, cricca[c].item.dir==DIR_RIGHT? spriteCoccodrilloDestra[i][j] : spriteCoccodrilloSinistra[i][j]); 
            } 
        }
    }
    wattroff(wgioco, COLOR_PAIR(COLORI_COCCODRILLO));
}