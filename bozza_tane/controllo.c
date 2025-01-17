#include "utilities.h"
#include "fiume.h"
#include "rana.h"
#include "controllo.h"
#include "avvia.h"
#include "coccodrilli.h"
#include "campo.h"

void controllo(int* pipe_fd, int* pipe_inversa);
//_Bool detectCollisione (Processo* rana, Processo* cricca, Processo* granate, Processo last, int* vite, WINDOW* debug);
_Bool detectCollisione(Processo*rana, Processo *cricca, Processo *granate, Processo last, int *vite, WINDOW* debug, _Bool * tane);

void controllo(int* pipe_fd, int* pipe_inversa)
{
    WINDOW* wgioco=newwin(NLINES, NCOLS, HUDLINES, 0); //finestra di gioco con coccodrilli e rana
    WINDOW* whud=newwin(HUDLINES, NCOLS, 0, 0); //finestra che segna le vite e il numero di livello
    WINDOW* debug=newwin(20,20, 0, NCOLS);
    //todo pensavo di fare una finestra verticale al lato che segna il tempo però poi vediamo
    
    Flusso* fiume=avviaFlussi();
    Processo rana=avviaRana(pipe_fd, pipe_inversa);
    Processo cricca[NUMERO_FLUSSI*MAX_COCCODRILLI];//cricca di coccodrilli
    Processo granate[N_GRANATE];
    Processo temp;
    _Bool tane[NUMERO_TANE]={0}; //essenzialmente se è false la tana è libera ()

    temp.pid==0; //così al primo detect collisione non succede nulla

    avviaCoccodrilli(pipe_fd, fiume, cricca); 

    for(int i=0; i<N_GRANATE; i++)
    {
        granate[i].item.id=NULL;
    }

    close(pipe_fd[1]);
    close(pipe_inversa[0]);

    int vite=MAX_VITE;
    time_t start=time(NULL);
    while(vite>0)
    {
        detectCollisione(&rana, cricca, granate, temp, &vite, debug, tane);
        //gestione stampa
        werase(wgioco);
        box(wgioco, ACS_VLINE, ACS_HLINE);
        box(whud,ACS_VLINE, ACS_HLINE );
            //per ora il tempo lo stampo nella box sopra poi vediamo
        mvwprintw(whud, 1, 1, "tempo passato: %d", time(NULL)- start);
        mvwprintw(whud, 1, 20, "vite: %d", vite);
        stampaTane(wgioco, tane);
        stampaMarciapiede(wgioco);
        stampaCoccodrilli(cricca, wgioco);
        stampaRana(rana.item, wgioco);
        

        for(int i=0; i<N_GRANATE; i++)
        {
            if(granate[i].item.id!=0)
            {
                stampaGranata(granate[i].item, wgioco);
            }
        }

        

        //check flussi
        for(int i=0; i<NUMERO_FLUSSI; i++)
        {
            mvwprintw(wgioco, fiume[i].y, 2, "Y:%d, dir:%d, speed:%d", fiume[i].y, fiume[i].dir, fiume[i].speed);
        }

        wnoutrefresh(wgioco);
        wnoutrefresh(whud);
        doupdate();


        //lettura dalla pipe 
        read(pipe_fd[0], &temp, sizeof(Processo));

        switch(temp.item.id)
        {
            case 'r':
                rana.item.x+=temp.item.x;
                rana.item.y+=temp.item.y;
                if(rana.item.x<=0) rana.item.x=1;
                if(rana.item.y<=0) rana.item.y=1;
                if(rana.item.x>=NCOLS-LARGHEZZA_RANA) rana.item.x=NCOLS-LARGHEZZA_RANA-1;
                if(rana.item.y>=NLINES-ALTEZZA_RANA) rana.item.y=NLINES-ALTEZZA_RANA-1;
                break;
            case 'q':
                cleanup(); break;
            case 'o':
                int i=(temp.item.y - ALTEZZA_MARCIAPIEDE)/ALTEZZA_FLUSSO;
                if(temp.item.dir==DIR_RIGHT) i*=2;
                granate[i]=temp;
                break;
            case 'g': //scrive due volte perchè sono due granate
                write(pipe_inversa[1], &rana, sizeof(Processo));
                write(pipe_inversa[1], &rana, sizeof(Processo));
                break;
            case 'c':
                for(int i=0; i<NUMERO_FLUSSI*MAX_COCCODRILLI; i++)
                {
                    if(cricca[i].pid==temp.pid)
                    {
                        cricca[i]=temp;
                    }
                }
        }
        usleep(1000);
    }

    //todo funzione che ammazza tutto quanto quando si esce dal loop principale
}
 

_Bool detectCollisione(Processo *rana, Processo* cricca, Processo *granate, Processo last, int *vite, WINDOW* debug, _Bool * tane) {

    //! RANA SU COCCODRILLO E ACQUA
    for (int i = 0; i < NUMERO_FLUSSI * MAX_COCCODRILLI; i++) {
        if (cricca[i].pid == 0) continue;
        
        // Se rana e coccodrillo sono sulla stessa riga e il processo è quello giusto
        if(rana->item.y == cricca[i].item.y && last.pid == cricca[i].pid){ 
            if(rana->item.x  >=  cricca[i].item.x && rana->item.x + LARGHEZZA_RANA <=  cricca[i].item.x + LARGHEZZA_COCCODRILLO){
                int offset=rana->item.x - cricca[i].item.x + cricca[i].item.dir;
                rana->item.x = cricca[i].item.x + offset;

                //rana->item.x+=cricca[i].item.dir;
                if (rana->item.x + LARGHEZZA_RANA < cricca[i].item.x) {
                    rana->item.x = cricca[i].item.x;
                }
                if (rana->item.x > cricca[i].item.x + LARGHEZZA_COCCODRILLO) {
                    rana->item.x = cricca[i].item.x + LARGHEZZA_COCCODRILLO - LARGHEZZA_RANA;
                }

                //controlla che la rana non esca dallo schermo
                if (rana->item.x < 1) rana->item.x = 1;
                if (rana->item.x > NCOLS - LARGHEZZA_RANA) 
                    rana->item.x = NCOLS - LARGHEZZA_RANA;

                mvwprintw(debug, 6, 0, "       ");
                break;
            }
            else{//se non è su un coccodrillo
                mvwprintw(debug, 6, 0, "fiume");
            }
        }
    }
    
    //!RANA SU TANA

    //ringraziami perché non ho usato un Bool
    if(rana->item.y < ALTEZZA_SPONDA){
        for(int i = 0; i < NUMERO_TANE ; i ++){
            //scorro le tane e vedo se la x della rana corrisponde a una tana poi controllo se quella tana è già stata occupata
            if(rana->item.x == (8 + (i) * 6 + i * 7)){ // questa formula è empirica davvero non saprei spiegare
                mvwprintw(debug, 8, 0, "tana");
                if(tane[i]){
                    mvwprintw(debug, 9, 0, "tana occupata");
                }
                else{
                    mvwprintw(debug, 9 , 0, "tana libera");
                    tane[i]=true;
                }
            }
            
        }

        // a prescindere da dove sia se supera una certa altezza va riportata giù
        rana->item.y=NLINES;
        rana->item.x=NCOLS/2;
    }
    
    wrefresh(debug);
    return false;
}

