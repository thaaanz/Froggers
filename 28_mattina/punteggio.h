#ifndef PUNTEGGIO_H
#define PUNTEGGIO_H

#include "utilities.h"
//punti
#define PUNTI_TANA 200
#define PUNTI_SALTO_AVANTI 20
#define PUNTI_SALTO_INDIETRO -20
#define PUNTI_PROIETTILI 10
#define PUNTI_MORTE 100
#define PUNTI_PER_SEC 0.5

typedef struct{
    int tempo;
    int salti;
    int proiettili;
    int tane;
    int morte;
} Punteggio;

void checkPuntiSalto(Punteggio* punti, Processo last );

#endif