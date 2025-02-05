#include "utilities.h"
#include "campo.h"

const char spriteTane[ALTEZZA_TANE][LARGHEZZA_TANE]={
    "        _____        _____        _____        _____        _____        ",
    "       |     |      |     |      |     |      |     |      |     |       ",
    "       |     |      |     |      |     |      |     |      |     |       ",
    "_______|     |______|     |______|     |______|     |______|     |_______"
};

void stampaTane(WINDOW * window, _Bool * occupazione){
    wattron(window, COLOR_PAIR(COLORI_TANE));
    for(int i =0; i < ALTEZZA_TANE; i++) {
        for (int j=0; j < LARGHEZZA_TANE; j++){
            mvwaddch(window, i+1 , j, spriteTane[i][j]); // +1 perché sennò va sui bordi
        }
    }
    wattroff(window, COLOR_PAIR(COLORI_TANE));
    //stampa nel caso siano occupate
    for (int i=0; i < NUMERO_TANE; i++){
        if(occupazione[i]){
            wattron(window, COLOR_PAIR(COLORI_TANE));
            for(int j=0; j < ALTEZZA_TANE-1; j++){
                mvwprintw(window, j+2, (9 + (i) * 6 + i * 7), "~~~~~");
            }
            wattroff(window, COLOR_PAIR(COLORI_TANE));
        }
        else{
            wattron(window, COLOR_PAIR(COLORI_SPONDA));
            for(int j=0; j < ALTEZZA_TANE-1; j++){
                mvwprintw(window, j+2, (8 + (i) * 6 + i * 7), "     ");
            }
            wattroff(window, COLOR_PAIR(COLORI_SPONDA));
        }
    }
    
}

void stampaMarciapiede(WINDOW* window){
    wattron(window, COLOR_PAIR(COLORI_MARCIAPIEDE));
    for(int i = 0; i < NCOLS ; i++){
        for(int j = 0; j < ALTEZZA_MARCIAPIEDE; j ++){
            mvwaddch(window, NLINES - ALTEZZA_MARCIAPIEDE + j +1, i, ' ' | A_REVERSE);
        }
    }
    wattroff(window, COLOR_PAIR(COLORI_MARCIAPIEDE));
}

void stampaSponda(WINDOW* window){
    wattron(window, COLOR_PAIR(COLORI_SPONDA));
    for(int i = 0; i < NCOLS ; i++){
        for(int j = 0; j < ALTEZZA_SPONDA; j ++){
            mvwaddch(window, ALTEZZA_TANE+ j +1, i, ' ' | A_REVERSE);
        }
    }
    wattroff(window, COLOR_PAIR(COLORI_SPONDA));
}

void stampaFiume ( WINDOW* window){
    wattron(window, COLOR_PAIR(COLORI_FIUME));
    for(int i = 0; i < NCOLS ; i++){
        for(int j = 0; j < NLINES - ALTEZZA_MARCIAPIEDE - ALTEZZA_SPONDA -ALTEZZA_TANE; j ++){
            mvwaddch(window, ALTEZZA_TANE + ALTEZZA_SPONDA + j +1, i, ' ' | A_REVERSE);
        }
    }
    wattroff(window, COLOR_PAIR(COLORI_FIUME));
}
