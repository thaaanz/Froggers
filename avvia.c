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
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
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
    init_pair(COLORI_GRANATA, COLOR_BLACK, COLOR_RED);
    init_pair(COLORI_PROIETTILI, COLOR_BLACK, COLOR_RED);
    init_pair(GREEN_TEMPO, COLOR_GREEN, COLOR_GREEN);
    init_pair(YELLOW_TEMPO, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(RED_TEMPO, COLOR_RED, COLOR_RED);
    init_pair(COLORI_MINE, COLOR_BLACK, COLOR_RED);

}

Flusso* avviaFlussi()
{
    static Flusso fiume[NUMERO_FLUSSI]; // static in modo che non venga distrutto al termine della funzione
    
    srand(time(NULL));

    int direzione;
    do{
        direzione= -1+(rand()%3);
    }while(direzione==0); // controllo la direzione non sia 0

    int coord_y=NLINES-ALTEZZA_RANA-ALTEZZA_MARCIAPIEDE; 

    for(int i=0; i < NUMERO_FLUSSI; i++)
    {
        fiume[i].dir=direzione;
        fiume[i].speed=1+(rand()%MAX_SPEED);
        fiume[i].y=coord_y;

        coord_y-=ALTEZZA_FLUSSO;
        direzione=-direzione;
    }

    return fiume;
}

void cleanup(Processo rana, Processo* cricca, Processo* astuccio, Processo* granate)
{
    if(rana.pid>1) {
        kill(rana.pid, 9);
        waitpid(rana.pid, NULL, 0);
        }
    for(int i=0; i<NUMERO_FLUSSI*MAX_COCCODRILLI; i++)
    {
        if(cricca[i].pid>1){
            kill(cricca[i].pid, 9);
            waitpid(cricca[i].pid, NULL, 0);
        }
    }
    for(int i=0; i<N_PROIETTILI; i++)
    {
        if(astuccio[i].pid > 1){
            kill(astuccio[i].pid, 9);
            waitpid(astuccio[i].pid, NULL, 0);
        }
        
    }
    for(int i=0; i<N_GRANATE; i++)
    {
        if(granate[i].pid > 1){
            kill(granate[i].pid, 9);
            waitpid(granate[i].pid, NULL, 0);
        }
        
    }
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

Oggetto* inizializzaMine(Oggetto* mine){
    int starting_x=rand() % (NCOLS - DIST_MAX_MINE * N_MINE);
    for(int i=0 ; i < N_MINE; i++)
    {
        mine[i].x=starting_x;
        starting_x += DIST_MIN_MINE + rand()%DIST_MAX_MINE;
        mine[i].y=ALTEZZA_TANE + ALTEZZA_RANA+1;
        mine[i].id='m';
        mine[i].dir=0;
    }
    return mine;
}