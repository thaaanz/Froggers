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