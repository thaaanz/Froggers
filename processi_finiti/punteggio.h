#ifndef PUNTEGGIO_H
#define PUNTEGGIO_H

#include "utilities.h"
#include "menu.h"

Punteggio inizializzaPunteggio();
void checkPuntiSalto(Punteggio* punti, Processo last);
int totalePunti(Punteggio punti);

#endif