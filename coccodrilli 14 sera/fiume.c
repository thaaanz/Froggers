#include "utilities.h"
#include "fiume.h"

Flusso* avviaFlussi()
{
    static Flusso fiume[NUMERO_FLUSSI];
    
    srand(time(NULL));
    int direzione;
    do{
        direzione= -1+(rand()%3);
    }while(direzione==0);
    int coord_y=NLINES-ALTEZZA_RANA-2; //da rivedere quando facciamo il marciapiede

    for(int i=0; i<NUMERO_FLUSSI; i++)
    {
        fiume[i].dir=direzione;
        fiume[i].speed=1+(rand()%MAX_SPEED);
        fiume[i].y=coord_y;

        coord_y-=ALTEZZA_FLUSSO;
        direzione=-direzione;
    }

    return fiume;
}