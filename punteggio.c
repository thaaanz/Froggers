#include "punteggio.h"

void checkPuntiSalto(Punteggio* punti, Processo last ){
    if(last.item.id == 'r'){
        if(last.item.y == -ALTEZZA_FLUSSO){ //sta andando avanti
            punti->salti+=PUNTI_SALTO_AVANTI;
        }
        if(last.item.y == ALTEZZA_FLUSSO){ //sta andando indietro
            punti->salti+=PUNTI_SALTO_INDIETRO;
        }
    }
}

int totalePunti(Punteggio punti){
    int totale=punti.salti + punti.tane + punti.proiettili + punti.tempo - punti.morte;
    if(totale < 0) totale = 0;
    return totale;
}