#include "utilities.h"
#include "campo.h"

const char spriteTane[ALTEZZA_TANE][LARGHEZZA_TANE]={
    "       _____        _____        _____        _____        _____       ",
    "      |     |      |     |      |     |      |     |      |     |      ",
    "      |     |      |     |      |     |      |     |      |     |      ",
    "______|     |______|     |______|     |______|     |______|     |______"
};

void stampaTane(WINDOW * window, _Bool * occupazione){
    for(int i =0; i < ALTEZZA_TANE; i++) {
        for (int j=0; j < LARGHEZZA_TANE; j++){
            mvwaddch(window, i+1 , j+1, spriteTane[i][j]); // +1 perché sennò va sui bordi
        }
    }
}

void stampaMarciapiede(WINDOW* window){
    for(int i =1; i < NCOLS-1; i ++){ //=1 e -1 perché sennò va sui bordi
        mvwaddch(window, NLINES-ALTEZZA_MARCIAPIEDE+1, i, ACS_HLINE);
    }
}