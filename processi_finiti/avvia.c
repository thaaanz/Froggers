#include "avvia.h"

void avviaPipe(int* pipe_fd) //per avviare le pipe
{
    if(pipe(pipe_fd)==-1)
    {
        perror("pipe fallita");
        exit(ERRORE_AVVIO_PIPE);
    }
}

void avviancurses()
{
    setlocale(LC_ALL, ""); //per la stampa del carattere unicode usato per le mine
    initscr(); //inizializza lo schermo e ncurses
    noecho(); //disattiva l'echoing
    cbreak(); //disattiva il buffering
    curs_set(0); //rende non visibile il cursore
    keypad(stdscr, TRUE); //attiva le freccettte direzionali
    start_color(); //avvia i colori
    srand(time(NULL)); //inizializza la generazione di numeri casuali

    //inizializzazione dei colori
    init_pair(COLORI_RANA, COLOR_BLACK, COLOR_GREEN);
    init_color(COLOR_DARK_GREEN, 0, 500, 0);
    init_pair(COLORI_COCCODRILLO, COLOR_DARK_GREEN, COLOR_DARK_BLUE);
    init_pair(COLORI_HUD, COLOR_MAGENTA, COLOR_BLACK);
    init_color(COLOR_GRAY, 190, 190, 190);
    init_pair(COLORI_TANE, COLOR_GRAY, COLOR_DARK_GREEN);
    init_pair(COLORI_TEMPO, COLOR_RED, COLOR_BLACK);
    init_color(COLOR_DARK_BLUE, 10, 10, 400);
    init_pair(COLORI_FIUME, COLOR_DARK_BLUE, COLOR_BLUE);
    init_pair(COLORI_MARCIAPIEDE, COLOR_GRAY, COLOR_WHITE);
    init_color(COLOR_BROWN, 600, 300, 0);
    init_pair(COLORI_SPONDA, COLOR_BROWN, COLOR_BROWN);
    init_pair(COLORI_PROIETTILI, COLOR_BLACK, COLOR_RED);
    init_pair(GREEN_TEMPO, COLOR_GREEN, COLOR_GREEN);
    init_pair(YELLOW_TEMPO, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(RED_TEMPO, COLOR_RED, COLOR_RED);
    init_pair(COLORI_MINE, COLOR_BLACK, COLOR_RED);
}

Flusso* avviaFlussi()
{
    static Flusso fiume[NUMERO_FLUSSI]; //static in modo che non venga distrutto al termine della funzione

    //genero la direzione del primo flusso in modo random in modo che sia 1 o -1
    int direzione;
    do{
        direzione= -1+(rand()%3);
    }while(direzione==0); //controllo che la direzione non sia 0

    int coord_y=NLINES-ALTEZZA_RANA-ALTEZZA_MARCIAPIEDE; //stabilisco la y del primo flusso(quello più in basso)

    for(int i=0; i < NUMERO_FLUSSI; i++) //inizializzo tutti i flussi
    {
        fiume[i].dir=direzione; //direzione alternata
        fiume[i].speed=1+(rand()%MAX_SPEED); //velocità(da 1 a 5)
        fiume[i].y=coord_y; //y

        coord_y-=ALTEZZA_FLUSSO; //aggiorno la variabile y per il prossimo flusso
        direzione=-direzione; //inverto la direzione per il prossimo flusso
    }

    return fiume;
}

Oggetto* inizializzaMine(Oggetto* mine) //inizializza tre mine, ostacoli aggiutivi nella sponda del fiume sotto le tane
{
    int starting_x=rand() % (NCOLS - DIST_MAX_MINE * N_MINE); //inizializza randomicamente la x della prima mina

    for(int i=0 ; i < N_MINE; i++) //crea le altre mine di conseguenza
    {
        mine[i].x=starting_x; 
        mine[i].y=ALTEZZA_TANE + ALTEZZA_RANA+1; //posiziona le mine alla giusta altezza nella sponda
        mine[i].id='m';
        mine[i].dir=0;
        
        starting_x += DIST_MIN_MINE + rand()%DIST_MAX_MINE; //aggiorna la x
    }
    return mine;
}

void cleanup(Processo rana, Processo* cricca, Processo* astuccio, Processo* granate)
{
    if(rana.pid>1) //uccido la rana
    {
        if(kill(rana.pid, 9)==-1)
        {
            exit(ERRORE_KILL_RANA);
        }
        waitpid(rana.pid, NULL, 0);
    }

    for(int i=0; i<NUMERO_FLUSSI*MAX_COCCODRILLI_PER_FLUSSO; i++) //uccido i coccodrilli
    {
        if(cricca[i].pid>1)
        {
            if(kill(cricca[i].pid, 9)==-1)
            {
                exit(ERRORE_KILL_COCCODRILLI);
            }
            waitpid(cricca[i].pid, NULL, 0);
        }
    }

    for(int i=0; i<N_PROIETTILI; i++) //uccido i proiettili ancora validi
    {
        if(astuccio[i].pid > 1)
        {
            if(kill(astuccio[i].pid, 9)==-1)
            {
                exit(ERRORE_KILL_PROIETTILI);
            }
            waitpid(astuccio[i].pid, NULL, 0);
        } 
    }

    for(int i=0; i<N_GRANATE; i++) //uccido le granate ancora valide
    {
        if(granate[i].pid > 1)
        {
            if(kill(granate[i].pid, 9)==-1)
            {
                exit(ERRORE_KILL_GRANATE);
            }
            waitpid(granate[i].pid, NULL, 0);
        }
    }
}


