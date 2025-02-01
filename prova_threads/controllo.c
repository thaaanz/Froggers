#include "controllo.h"

void* controllo(void* mutex)
{
    pthread_mutex_t* m=(pthread_mutex_t*) mutex; //cast dell'argomento

    //inizializzo le finestre
    WINDOW* wgioco=newwin(NLINES, NCOLS, HUDLINES, 0); //finestra di gioco con coccodrilli e rana
    WINDOW* whud=newwin(HUDLINES, NCOLS, 0, 0); //finestra che segna le vite e il tempo
    WINDOW* debug=newwin(40,20, 0, NCOLS + TCOLS+ 5);
    WINDOW* wtempo=newwin(NLINES -1, TCOLS, HUDLINES, NCOLS ); // colonna del tempo

    Thread temp;
    Thread cricca[NUMERO_FLUSSI*MAX_COCCODRILLI]; //cricca di coccodrilli
    Flusso* fiume=avviaFlussi();

    _Bool tane[NUMERO_TANE]={0}; //essenzialmente se è false la tana è libera 


    while(true)
    {
        werase(wgioco);
        stampaCoccodrilli(cricca, wgioco);
        stampaFiume(wgioco);
        stampaTane(wgioco, tane);
        stampaMarciapiede(wgioco);
        stampaSponda(wgioco);
        wnoutrefresh(wgioco);
        doupdate();

        temp=leggi(m); // lettura dal buffer

        switch(temp.item.id)
        {
            case 'c': //se legge un coccodrillo
                for(int i=0; i<NUMERO_FLUSSI*MAX_COCCODRILLI; i++) //scorre l'array
                {
                    if(cricca[i].tid==temp.tid) //cerca il coccodrillo corrispondente
                    {
                        cricca[i]=temp; //aggiora il coccodrillo
                    }
                }
                break;
        }

        usleep(DELAY/10);
    }

}   