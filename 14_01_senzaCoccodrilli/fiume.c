#include "utilities.h"
#include "fiume.h"

Flusso* avviaFlussi()
{
    static Flusso fiume[NUMERO_FLUSSI];
    
    srand(time(NULL));
    int direzione= -1+(rand()%3);
    int coord_y=NLINES-ALTEZZA_RANA-2; //da rivedere quando facciamo il marciapiede

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