#include "controllo.h"

void* controllo(void* mutex)
{
    pthread_mutex_t* m=(pthread_mutex_t*) mutex;

    WINDOW* wgioco=newwin(NLINES, NCOLS, HUDLINES, 0); //finestra di gioco con coccodrilli e rana
    WINDOW* whud=newwin(HUDLINES, NCOLS, 0, 0); //finestra che segna le vite e il tempo
    WINDOW* debug=newwin(40,20, 0, NCOLS + TCOLS+ 5);
    WINDOW* wtempo=newwin(NLINES -1, TCOLS, HUDLINES, NCOLS ); // colonna del tempo

    //inizializzazioni
    Oggetto temp;

    //Flusso* fiume=avviaFlussi();
    Oggetto rana={'r', NLINES- ALTEZZA_RANA-1, NCOLS/2, NULL };
    Processo cricca[NUMERO_FLUSSI*MAX_COCCODRILLI];//cricca di coccodrilli
    Processo granate[N_GRANATE];
    Processo astuccio[N_PROIETTILI]; //astuccio dei proiettili
    _Bool tane[NUMERO_TANE]={0}; //essenzialmente se è false la tana è libera 
    int vite=MAX_VITE;

    while(true)
    {
        //gestione stampa
        werase(wgioco);
        stampaFiume(wgioco);
        stampaTane(wgioco, tane);
        stampaMarciapiede(wgioco);
        stampaSponda(wgioco);
        //stampaCoccodrilli(cricca, wgioco);
        stampaRana(rana, wgioco);
        wnoutrefresh(wgioco);
        doupdate();

        for(int i=0; i<DIM_BUFFER; i++)
        {
            mvprintw(25+i, 5, "%c", b.buffer[i].id);
        }

        temp=leggi(m); // lettura dal buffer

        switch(temp.id){
            case 'r':
                rana.x+=temp.x;
                rana.y+=temp.y;
                if(rana.x<=0) rana.x=1;
                if(rana.y<=0) rana.y=1;
                if(rana.x>=NCOLS-LARGHEZZA_RANA) rana.x=NCOLS-LARGHEZZA_RANA-1;
                if(rana.y>=NLINES-ALTEZZA_RANA) rana.y=NLINES-ALTEZZA_RANA-1;
                break;
            case 'o':
                flash();
                break;
        }
        
        
        
        usleep(3000);
    }

    return NULL;
}