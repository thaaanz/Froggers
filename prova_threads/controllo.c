#include "controllo.h"

void* controllo(void* mutex)
{
    pthread_mutex_t* m=(pthread_mutex_t*) mutex; //cast dell'argomento

    //inizializzo le finestre
    WINDOW* wgioco=newwin(NLINES, NCOLS, HUDLINES, 0); //finestra di gioco con coccodrilli e rana
    WINDOW* whud=newwin(HUDLINES, NCOLS, 0, 0); //finestra che segna le vite e il tempo
    WINDOW* debug=newwin(40,70, 0, NCOLS + TCOLS+ 5);
    WINDOW* wtempo=newwin(NLINES -1, TCOLS, HUDLINES, NCOLS ); // colonna del tempo

    Thread temp;
    Thread cricca[NUMERO_FLUSSI*MAX_COCCODRILLI]; //cricca di coccodrilli
    Thread astuccio[N_PROIETTILI];
    Flusso fiume[NUMERO_FLUSSI];
    avviaFlussi(fiume);
    avviaCoccodrilli(fiume, cricca, m);
    avviaProiettili(astuccio);

    _Bool tane[NUMERO_TANE]={0}; //essenzialmente se è false la tana è libera 


    while(true)
    {
        werase(wgioco);
        werase(debug);
        stampaFiume(wgioco);
        stampaTane(wgioco, tane);
        stampaMarciapiede(wgioco);
        stampaSponda(wgioco);
        stampaCoccodrilli(cricca, wgioco);

        for(int i=0; i<N_PROIETTILI; i++)
        {
            if(astuccio[i].tid!=-1)
            {
                stampaProiettile(astuccio[i].item, wgioco);
            }
        }

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
                        cricca[i]=temp; //aggiorna il coccodrillo
                    }
                }
                break;

            case '-':
                _Bool trovati = false;

                for (int i = 0; i < N_PROIETTILI; i++) {
                    if (astuccio[i].tid == temp.tid) {
                        astuccio[i] = temp;
                        trovati = true;
                        
                        // Check se è fuori dai bordi
                        if (astuccio[i].item.x < 0 || astuccio[i].item.x > NCOLS) {
                            if (astuccio[i].tid > 0) {
                                    //flash();
                                    pthread_cancel(astuccio[i].tid);
                                    pthread_join(astuccio[i].tid, NULL);
                                    astuccio[i].tid = -1;                
                                }
                        }
                        break;
                    }
                } 
                
                if (!trovati) {
                    for (int i = 0; i < N_PROIETTILI; i++) {
                        if (astuccio[i].tid == -1 ) { //! ho cambiato da <0 a ==-1 e ora sembra che vada???
                            astuccio[i] = temp;
                            trovati = true;
                            break;
                        }
                    }
                }
                
                if (!trovati && temp.tid > 1) {
                    pthread_cancel(temp.tid);
                    pthread_join(temp.tid, NULL);
                }

                break;
        }

        usleep(DELAY/10);
    }

}   