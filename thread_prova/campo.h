#ifndef CAMPO_H
#define CAMPO_H

#include "utilities.h"

extern const char spriteTane[ALTEZZA_TANE][LARGHEZZA_TANE];

void stampaTane( WINDOW * window, _Bool * occupazione);
void stampaMarciapiede(WINDOW* window);
void stampaSponda(WINDOW* window);
void stampaFiume ( WINDOW* window);
void stampaMine(Oggetto* mine, WINDOW* window);

#endif