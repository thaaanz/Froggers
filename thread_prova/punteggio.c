#include "punteggio.h"

Punteggio inizializzaPunteggio(){
    //inizializzo i punti a 0
    Punteggio punti;

    punti.tempo=0;
    punti.proiettili=0;
    punti.salti=0;
    punti.tane=0;
    punti.morte=0;

    if(utentePrivilegiato) punti.salti=1000; //bonus .)

    return punti;
}

void checkPuntiSalto(Punteggio* punti, Thread last ){
    if(last.item.id == 'r'){
        if(last.item.y == -ALTEZZA_FLUSSO){ //se la rana salta in avanti aggiungo punti
            punti->salti+=PUNTI_SALTO_AVANTI;
        }
        if(last.item.y == ALTEZZA_FLUSSO){ //se salta all'indietro li tolgo
            punti->salti+=PUNTI_SALTO_INDIETRO;
        }
    }
}

int totalePunti(Punteggio punti){
    int totale=punti.salti + punti.tane + punti.proiettili + punti.tempo - punti.morte;
    if(totale < 0) totale = 0; //non può essere negativo
    return totale;
}