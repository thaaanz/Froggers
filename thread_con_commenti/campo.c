#include "utilities.h"
#include "campo.h"

const char spriteTane[ALTEZZA_TANE][LARGHEZZA_TANE]={
    "        _____        _____        _____        _____        _____        ",
    "       |     |      |     |      |     |      |     |      |     |       ",
    "       |     |      |     |      |     |      |     |      |     |       ",
    "_______|     |______|     |______|     |______|     |______|     |_______"
};

void stampaTane(WINDOW * window, _Bool * occupazione)
{
    wattron(window, COLOR_PAIR(COLORI_TANE)); //attivo gli attributi dei colori per la stampa delle tane
    for(int i =0; i < ALTEZZA_TANE; i++) 
    {
        for (int j=0; j < LARGHEZZA_TANE; j++)
        {
            mvwaddch(window, i+1 , j, spriteTane[i][j]); //stampo le tane
        }
    }
    wattroff(window, COLOR_PAIR(COLORI_TANE)); //disattiva gli attributi

    for (int i=0; i < NUMERO_TANE; i++) //per ogni tana
    {
        if(occupazione[i]) //controllo se è chiusa o meno
        {
            wattron(window, COLOR_PAIR(COLORI_TANE));
            for(int j=0; j < ALTEZZA_TANE-1; j++)
            {
                mvwprintw(window, j+2, (8 + (i) * 6 + i * 7), "~~~~~"); //se è chiusa la stampo di conseguenza
            }
            wattroff(window, COLOR_PAIR(COLORI_TANE));
        }
    }
}

void stampaMarciapiede(WINDOW* window){
    wattron(window, COLOR_PAIR(COLORI_MARCIAPIEDE));
    for(int i = 0; i < NCOLS ; i++){
        for(int j = 0; j < ALTEZZA_MARCIAPIEDE; j ++){
            mvwaddch(window, NLINES - ALTEZZA_MARCIAPIEDE + j +1, i, ' ' | A_REVERSE);//stampa il marciapiede, A_REVERSE inverte i colori per stampare quello di sfondo
        }
    }
    wattroff(window, COLOR_PAIR(COLORI_MARCIAPIEDE));
}

void stampaSponda(WINDOW* window){
    wattron(window, COLOR_PAIR(COLORI_SPONDA));
    for(int i = 0; i < NCOLS ; i++){
        for(int j = 0; j < ALTEZZA_SPONDA; j ++){
            mvwaddch(window, ALTEZZA_TANE+ j +1, i, ' ' | A_REVERSE); // stampa la sponda tra il fiume e le tane
        }
    }
    wattroff(window, COLOR_PAIR(COLORI_SPONDA));
}

void stampaFiume ( WINDOW* window){
    wattron(window, COLOR_PAIR(COLORI_FIUME));
    for(int i = 0; i < NCOLS ; i++){
        for(int j = 0; j < NLINES - ALTEZZA_MARCIAPIEDE - ALTEZZA_SPONDA -ALTEZZA_TANE; j ++){
            mvwaddch(window, ALTEZZA_TANE + ALTEZZA_SPONDA + j +1, i, ' ' | A_REVERSE); //stampa il fiume
        }
    }
    wattroff(window, COLOR_PAIR(COLORI_FIUME));
}

void stampaMine(Oggetto* mine, WINDOW* window){
    wattron(window, COLOR_PAIR(COLORI_MINE));
    for(int i = 0; i < N_MINE; i++){
        if(mine[i].id == 'm'){
            mvwprintw(window, mine[i].y, mine[i].x, "X"); //stampa le mine
        }
    }
    wattroff(window, COLOR_PAIR(COLORI_MINE));
}