#include "avvia.h"


/*void avviaPipe(int* pipe_fd)
{
    if(pipe(pipe_fd)==-1)
    {
        perror("pipe fallita");
        exit(1);
    }
}*/

void avviancurses()
{
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
    init_pair(COLORI_GRANATE, COLOR_BLACK, COLOR_RED);
}

void cleanup(Thread rana, Thread* cricca, Thread* astuccio, Thread* granate)
{

    for(int i=0; i<NUMERO_FLUSSI*MAX_COCCODRILLI; i++) //termino coccodrilli ancora validi
    {
        if(cricca[i].tid != -1){
            if(pthread_kill(cricca[i].tid, 0) == 0) {
                if(pthread_cancel(cricca[i].tid) == 0) {
                    pthread_join(cricca[i].tid, NULL);
                }
            }
            cricca[i].tid=-1;
        }
    }

    for(int i=0; i<N_PROIETTILI; i++) //termino proiettili ancora validi
    {
        if(astuccio[i].tid != -1){
            if(pthread_kill(astuccio[i].tid, 0) == 0) {
                if(pthread_cancel(astuccio[i].tid) == 0) {
                    pthread_join(astuccio[i].tid, NULL);
                }
            }
            astuccio[i].tid=-1;
        }
        
    }
    
    for(int i=0; i<N_GRANATE; i++) //termino granate ancora valide
    {
        if(granate[i].tid != -1){
            if(pthread_kill(granate[i].tid, 0) == 0) {
                if(pthread_cancel(granate[i].tid) == 0) {
                    pthread_join(granate[i].tid, NULL);
                }
            }
            granate[i].tid=-1;
        }
    }
    
    if(pthread_kill(rana.tid, 0) == 0) { //termino rana
        if(pthread_cancel(rana.tid) == 0) {
            pthread_join(rana.tid, NULL);
        }
    }

}


void avviaFlussi(Flusso* fiume)
{    

    int direzione;
    //genero la direzione del primo flusso in modo random in modo che sia 1 o -1
    do{
        direzione= -1+(rand()%3);
    }while(direzione==0);//controllo che la direzione non sia 0

    int coord_y=NLINES-ALTEZZA_RANA-ALTEZZA_MARCIAPIEDE; //stabilisco la y del primo flusso(quello più in basso)

    for(int i=0; i < NUMERO_FLUSSI; i++) //inizializzo tutti i flussi
    {
        fiume[i].dir=direzione; //direzione alternata
        fiume[i].speed=1+(rand()%MAX_SPEED); //velocità(da 1 a 5)
        fiume[i].y=coord_y; //y

        coord_y-=ALTEZZA_FLUSSO; //aggiorno la variabile y per il prossimo flusso
        direzione=-direzione; //inverto la direzione per il prossimo flusso
    }

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