#include "controllo.h"

void* controllo(void* mutex)
{
    pthread_mutex_t* m=(pthread_mutex_t*) mutex;

    WINDOW* wgioco=newwin(NLINES, NCOLS, HUDLINES, 0); //finestra di gioco con coccodrilli e rana
    WINDOW* whud=newwin(HUDLINES, NCOLS, 0, 0); //finestra che segna le vite e il tempo
    WINDOW* debug=newwin(40,40, 0, NCOLS + TCOLS+ 5);
    WINDOW* wtempo=newwin(NLINES -1, TCOLS, HUDLINES, NCOLS ); // colonna del tempo

    //inizializzazioni
    Thread temp;

    //Flusso* fiume=avviaFlussi();
    Thread rana={0,{'r', NLINES- ALTEZZA_RANA-1, NCOLS/2, NULL }}; // qua in realtà potremmo fare avvia rana che restituisce il tid
    Thread cricca[NUMERO_FLUSSI*MAX_COCCODRILLI];//cricca di coccodrilli
    Thread granate[N_GRANATE];
    Thread astuccio[N_PROIETTILI]; //astuccio dei proiettili
    _Bool tane[NUMERO_TANE]={0}; //essenzialmente se è false la tana è libera 
    int vite=MAX_VITE;

    inizializzaGranate(granate);
    while(true)
    {
        //gestione stampa
        werase(wgioco);
        werase(debug);
        stampaFiume(wgioco);
        stampaTane(wgioco, tane);
        stampaMarciapiede(wgioco);
        stampaSponda(wgioco);
        //stampaCoccodrilli(cricca, wgioco);
        stampaRana(rana.item, wgioco);

        for(int i=0; i<N_GRANATE; i++)
        {
            mvwprintw(debug, 5+i, 4, " %d", granate[i].tid);
            if(granate[i].tid>0)
            {
                stampaGranata(granate[i].item, wgioco);
            }
        }

        //print buffer
        for(int i=0; i<DIM_BUFFER; i++)
        {
            mvwprintw(debug, 2, 1+i, "%c", b.buffer[i].item.id);
        }

        //print pos rana
        mvwprintw(debug, 3, 1, "x: %d y: %d", rana.item.x, rana.item.y);
        temp=leggi(m); // lettura dal buffer
        mvwprintw(debug, 1, 1, "%c", temp.item.id);


        switch(temp.item.id){
            case 'r':
                rana.item.x+=temp.item.x;
                rana.item.y+=temp.item.y;
                if(rana.item.x<=0) rana.item.x=1;
                if(rana.item.y<=0) rana.item.y=1;
                if(rana.item.x>=NCOLS-LARGHEZZA_RANA) rana.item.x=NCOLS-LARGHEZZA_RANA-1;
                if(rana.item.y>=NLINES-ALTEZZA_RANA) rana.item.y=NLINES-ALTEZZA_RANA-1;
                break;
            case 'g':
            //? ma se controllassimo se c'è spazio qua invece che creare i thread e poi cancellarli?
                avviaGranate(rana, mutex);
            case 'o':
                //flash();
                mvwprintw(debug, 10, 4, "%d \t%d\t %d", temp.item.x, temp.item.y, temp.tid);
                //mvwaddch(wgioco, temp.item.y, temp.item.x, temp.item.id); // per il debug
                //stampare le stampa appena arrivano però credo sia un problema con la memorizzazione nell'array
                _Bool trovate = false;

                for (int i = 0; i < N_GRANATE; i++) {
                    if (granate[i].tid == temp.tid) {
                        granate[i] = temp;
                        trovate = true;
                        
                        // Check se è fuori dai bordi
                        if (granate[i].item.x < 0 || granate[i].item.x > NCOLS) {
                            if (granate[i].tid > 0) {
                                    //flash();
                                    pthread_cancel(granate[i].tid)==0;
                                    pthread_join(granate[i].tid, NULL);
                                    granate[i].tid = -1;                
                                }
                        }
                        break;
                    }
                } 
                
                if (!trovate) {
                    for (int i = 0; i < N_GRANATE; i++) {
                        if (granate[i].tid == -1 ) { //! ho cambiato da <0 a ==-1 e ora sembra che vada???
                            granate[i] = temp;
                            trovate = true;
                            break;
                        }
                    }
                }
                
                if (!trovate && temp.tid > 1) {
                    pthread_cancel(temp.tid);
                    pthread_join(temp.tid, NULL);
                }

                break;
        }

        wnoutrefresh(debug);
        wnoutrefresh(wgioco);
        doupdate();
        usleep(5000);
    }

    return NULL;
}