#include "avvia.h"

void avviancurses()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
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

Flusso* avviaFlussi()
{
    static Flusso fiume[NUMERO_FLUSSI];
    
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

    return fiume;
}