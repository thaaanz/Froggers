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
}

/*void cleanup(Processo rana, Processo* cricca, Processo* astuccio, Processo* granate)
{
    if(rana.pid>1) kill(rana.pid, 9);
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
}*/

/*
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
*/
