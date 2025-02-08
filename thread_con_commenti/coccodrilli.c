#include "coccodrilli.h"

const char spriteCoccodrilloDestra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO]={
    ">>>>>>>>>>",
    ">>>>>>>>>>"
};

const char spriteCoccodrilloSinistra[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO]={
    "<<<<<<<<<<",
    "<<<<<<<<<<"
};

void avviaCoccodrilli(Flusso* fiume, Thread* cricca, Semafori* s)
{
    int i=0; //inizializzo un indice che scorre la totalità dei coccodrilli 

    for(int k=0; k<NUMERO_FLUSSI*MAX_COCCODRILLI; k++) //inizializzo tutto l'array
    {
        cricca[k].tid=0;
        cricca[k].item.id='c';
        cricca[k].item.y=0;
        cricca[k].item.x=0;
        cricca[k].item.dir=0;
    }

    for(int n=0; n < MAX_COCCODRILLI; n++) //n scorre i coccodrilli per ogni riga
    {
        for(int f=0; f<NUMERO_FLUSSI; f++) //f scorre i flussi
        {
            pthread_t tid;
            
            Cocco c={cricca[i].item, s, fiume[f], (rand()%4)}; //inserisco i dati nella struttura
            //passo un offset random per fare in modo che i coccodrilli vengano generati a distanze diverse tra loro
            if( pthread_create(&tid, NULL, &funzioneCoccodrillo, &c) != 0) {//creo il thread e gli passo la funzione 
                perror("ERRORE CREAZIONE THREAD COCCODRILLO");
                exit(1);
            }
            cricca[i].tid=tid; //genero il tid e lo assegno a ogni coccodrillo nell'array
            i++; //i scorre i coccodrilli in ogni flusso (verticalmente)
        }
    }
}

void* funzioneCoccodrillo(void* parametri)
{
    Cocco c=*((Cocco*) parametri); //cast dell'argomento della thread function
    Params parametro={c.item, c.semafori};
    int delay, starting_x; //delay per la velocità del coccodrillo(in base a quella del flusso) e x di partenza (in base all'offset)
    time_t last_shot=time(NULL); //ultimo sparo del coccodrillo
    int shot_delay=(rand()%10)+1; //delay tra uno sparo e l'altro

    if (c.f.dir == DIR_RIGHT) starting_x = -LARGHEZZA_COCCODRILLO - (c.offset * (LARGHEZZA_COCCODRILLO*2)); //se il coccodrillo va verso destra
    else starting_x = NCOLS + (c.offset * (LARGHEZZA_COCCODRILLO*2)); //se va verso sinistra
    Thread coccodrillo = {pthread_self(), {'c', c.f.y+1, starting_x, c.f.dir}};

    while(true) //ciclo per l'aggiornamento della posizione del coccodrillo
    {
        scrivi(c.semafori, coccodrillo); //scrivo sul buffer
        coccodrillo.item.x+=coccodrillo.item.dir; //incremento la x di 1 o -1 a seconda la direzione

        //wrap around: quando il coccodrillo esce dallo schermo aggiorno la sua posizione per farlo riapparire dalla parte opposta
        if(coccodrillo.item.dir==DIR_RIGHT && coccodrillo.item.x>NCOLS) coccodrillo.item.x=-LARGHEZZA_COCCODRILLO;
        if(coccodrillo.item.dir==DIR_LEFT && coccodrillo.item.x<-LARGHEZZA_COCCODRILLO) coccodrillo.item.x=NCOLS;
        parametro.item=coccodrillo.item;

        //se è trascorso abbastanza tempo dall'ultimo sparo
        if(time(NULL)-last_shot>=shot_delay)
        {
            pthread_t tid_proiettile;//sparo un nuovo proiettile
            if(pthread_create(&tid_proiettile, NULL, &funzioneProiettile, &parametro)!=0){
                perror("ERRORE CREAZIONE THREAD PROIETTILE");
                exit(ERRORE_THREAD_PROIETTILI);
            }
            last_shot=time(NULL);
            shot_delay=(rand()%10)+5;//aggiorno l'intervallo random per il prossimo sparo, almeno 5 secondi
        }

        delay=fdelay(c.f.speed); //imposto il delay dell'aggiornamento in base alla velocità del flusso
        usleep(delay);
    }
}

int fdelay(int speed)
{
    switch(speed)//controllo la velocità del flusso e restituisco la velocità dei coccodrilli di conseguenza
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
                //stampo i coccodrilli, uso l'operatore ternario per scegliere la sprite giusta in base alla direzione 
            } 
        }
    }
    wattroff(wgioco, COLOR_PAIR(COLORI_COCCODRILLO));
}

//PROIETTILI
void inizializzaProiettili(Thread* astuccio)//inizializza l'array di proiettili
{
    for(int i=0; i<N_PROIETTILI; i++)
    {
        astuccio[i].tid=-1;
        astuccio[i].item.id='-';
        astuccio[i].item.y=1;
        astuccio[i].item.x=1;
        astuccio[i].item.dir=0;
    }
}

void* funzioneProiettile(void* parametri) 
{
    Params c=*((Params*) parametri); //cast dell'argomento della thread function
    //creo il proiettile con la y del coccodrillo e la x impostata in base alla direzione del coccodrillo(il proiettile parte dalla sinistra del coccodrillo se va verso destra e viceversa)
    Thread proiettile={pthread_self(), {'-', c.item.y+1, c.item.x+(c.item.dir==DIR_RIGHT? LARGHEZZA_COCCODRILLO : 0), c.item.dir}};
        
    while(1)
    {
        scrivi(c.semafori, proiettile); //scrivo nel buffer
        proiettile.item.x+=proiettile.item.dir;//aggiorno la x del proiettile
        usleep(DELAY_PROIETTILE);
    }
}

void stampaProiettili(Thread* astuccio, WINDOW* wgioco)
{
    wattron(wgioco, COLOR_PAIR(COLORI_PROIETTILI));
    wattron(wgioco, A_BOLD); //per la stampa in grassetto
    for (int i = 0; i < N_PROIETTILI; i++) //scorro l'array
        {
            if (astuccio[i].tid != -1) //se la cella contiene un proiettile 
            {
                mvwaddch(wgioco, astuccio[i].item.y, astuccio[i].item.x, astuccio[i].item.id); //lo stampo
            }
        }
    
    wattroff(wgioco, A_BOLD);
    wattroff(wgioco, COLOR_PAIR(COLORI_PROIETTILI));
}