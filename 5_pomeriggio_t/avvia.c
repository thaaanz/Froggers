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
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    //nodelay(stdscr, true);
    start_color(); 

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
}

void cleanup(Thread rana, Thread* cricca, Thread* astuccio, Thread* granate)
{

    for(int i=0; i<NUMERO_FLUSSI*MAX_COCCODRILLI; i++)
    {
        if(cricca[i].tid != -1){
            pthread_cancel(cricca[i].tid);
            pthread_join(cricca[i].tid, NULL);
        }
    }

    for(int i=0; i<N_PROIETTILI; i++)
    {
        if(astuccio[i].tid != -1){
            pthread_cancel(astuccio[i].tid);
            pthread_join(astuccio[i].tid, NULL);
        }
        
    }
    
    for(int i=0; i<N_GRANATE; i++)
    {
        if(granate[i].tid != -1){
            pthread_cancel(granate[i].tid);
            pthread_join(granate[i].tid, NULL);
        }
    }
    
     
        pthread_cancel(rana.tid);
        pthread_join(rana.tid, NULL);
}


Punteggio inizializzaPunteggio(){
    Punteggio punti;

    punti.tempo=0;
    punti.proiettili=0;
    punti.salti=0;
    punti.tane=0;
    punti.morte=0;

    if(utentePrivilegiato) punti.salti=1000; //;)

    return punti;
}


void avviaFlussi(Flusso* fiume)
{    
    srand(time(NULL));
    int direzione;
    
    do{
        direzione= -1+(rand()%3);
    }while(direzione==0);

    int coord_y=NLINES-ALTEZZA_RANA-ALTEZZA_MARCIAPIEDE; 

    for(int i=0; i<NUMERO_FLUSSI; i++)
    {
        fiume[i].dir=direzione;
        fiume[i].speed=1+(rand()%MAX_SPEED);
        fiume[i].y=coord_y;

        coord_y-=4;
        direzione=-direzione;
    }

}