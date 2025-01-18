#include "utilities.h"
#include "fiume.h"
#include "rana.h"
#include "controllo.h"
#include "avvia.h"
#include "coccodrilli.h"
#include "campo.h"

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
    int vite=MAX_VITE;
    int punteggio=0; //!poi dobbiamo decidere i vari guadagni di punteggio

    temp.pid==0; //così al primo detect collisione non succede nulla

    avviaCoccodrilli(pipe_fd, fiume, cricca); 

    for(int i=0; i<N_GRANATE; i++)
    {
        granate[i].item.id=NULL;
    }

    close(pipe_fd[1]);
    close(pipe_inversa[0]);

    
    time_t start=time(NULL);
    while(vite>0)
    {
        //todo aggiungere un check per la vittoria
        //todo mettere almeno i colori
        //todo creare una pipesuono e un processo che gestisca la musica e i suoni di gioco(in quell trovato su github fanno così)
        //gestione stampa
        werase(wgioco);
        box(wgioco, ACS_VLINE, ACS_HLINE);
        box(whud,ACS_VLINE, ACS_HLINE );
            //per ora il tempo lo stampo nella box sopra poi vediamo
        mvwprintw(whud, 1, 1, "tempo rimasto: %d", MAX_TEMPO - (time(NULL)- start)); //! qua possiamo fare una barra ma non me ne preoccuperei ora
        mvwprintw(whud, 1, 20, "vite: %d", vite);
        stampaTane(wgioco, tane);
        stampaMarciapiede(wgioco);
        stampaCoccodrilli(cricca, wgioco);
        detectCollisione(&rana, cricca, tane, temp, wgioco, debug, &vite, time(NULL)-start, &punteggio);
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
                sleep(10); break; //! questo messo tipo pausa per il debug poi lo cambiamo
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
        usleep(1100);
    }

    //todo funzione che ammazza tutto quanto quando si esce dal loop principale
}
 
void detectCollisione(Processo* rana, Processo* cricca, _Bool* tane, Processo last, WINDOW* wgioco, WINDOW* debug, int* vite, int secondi, int*punti ){

    //wclear(debug);
    chtype ct=mvwinch(wgioco, rana->item.y, rana->item.x); // controllo se la rana sta sopra qualcosa 
    //! per far funzionare questa cosa è importante che la funzione stia tra stampaCoccodrilli e stampaRana
    char c=(char)(ct & A_CHARTEXT);
    mvwprintw(debug, 1,1, "-%c-", c);
    if( c=='<' || c == '>'){ //se è sopra un coccodrillo allora
        mvwprintw(debug, 3,1, "coccodrillo");
        for(int i=0; i < NUMERO_FLUSSI * MAX_COCCODRILLI; i++){ // controllo su quale coccodrillo sta
            if(cricca[i].pid == last.pid){ // se si sta muovendo in quel determinato ciclo posso modificare la posizione
                if(last.item.y == rana->item.y 
                    && rana->item.x >= last.item.x 
                    && rana->item.x + LARGHEZZA_RANA <= last.item.x + LARGHEZZA_COCCODRILLO){ // controllo che sia sopra il coccodrillo che stiamo prendendo in considerazione
                    
                    rana->item.x+=last.item.dir;
                    // check se esce dai bordi dello schermo
                    if(rana->item.x < 1){ 
                        rana->item.x=1;
                    }
                    if(rana->item.x > NCOLS -LARGHEZZA_RANA-1){
                        rana->item.x=NCOLS -LARGHEZZA_RANA-1; // -1 per la box
                    }

                    //essendo che scivola sui coccodrilli faccio in modo che non esca dal coccodrillo sennò si perde una vita a cazzum
                    if(rana->item.x < last.item.x){
                        rana->item.x = last.item.x;
                    }
                    if(rana->item.x + LARGHEZZA_RANA > last.item.x + LARGHEZZA_COCCODRILLO){
                        rana->item.x = last.item.x + LARGHEZZA_COCCODRILLO - LARGHEZZA_RANA;
                    }
                    
                }  
            }
        }
    }
    else{ //? se non è un coccodrillo è uno spazio dimmi se ha senso
        if(rana->item.y > NLINES-ALTEZZA_MARCIAPIEDE){ // sta nel marciapiede quindi non faccio nulla, sta qua solo per il debug
            mvwprintw(debug, 2,1, "marciapiede");
        }
        if(rana->item.y < ALTEZZA_TANE){ // sta all'altezza delle tane
            mvwprintw(debug, 4,1, "tane");
            for(int i=0; i < NUMERO_TANE; i++){ // controllo se sta dentro una tana
                if(rana->item.x == (8 + (i) * 6 + i * 7)){ 
                    mvwprintw(debug, 4,1, "tana beccata");
                    if(tane[i] == 0){ //la tana è libera
                        tane[i]=1;
                        //todo resettare le vite?? e il tempo
                    }           
                }
            }                      
        }
        if(rana->item.y > ALTEZZA_TANE && rana->item.y < ALTEZZA_TANE+ ALTEZZA_SPONDA){ // sta nella sponda safe in alto quindi non faccio nulla, qua per il debug
            mvwprintw(debug, 5,1, "sponda");
        }
        if(rana->item.y > ALTEZZA_TANE+ ALTEZZA_SPONDA && rana->item.y < NLINES - ALTEZZA_MARCIAPIEDE){
            //mvwprintw(debug, 6,1, "qua avresti perso");
            (*vite)--;
            rana->item.y=NLINES-ALTEZZA_RANA-1;
            //todo fare in modo che si azzeri il tempo, magari usando un valore di ritorno al controllo
            werase(wgioco);
            mvwprintw(wgioco, NLINES/2, NCOLS/2, "Vita persa");
            wnoutrefresh(wgioco);
            doupdate();
            sleep(1);

        }
    }        
    //wrefresh(debug);

    if(secondi > MAX_TEMPO){ //! per ora MAX_TEMPO ha un valore a caso poi vediamo bene
        (*vite)--;
        rana->item.y=NLINES-ALTEZZA_RANA-1;
        werase(wgioco);
        mvwprintw(wgioco, NLINES/2, NCOLS/2, "Vita persa");
        wnoutrefresh(wgioco);
        doupdate();
        sleep(1);
        //todo azzerare il tempo sennò si perdono tutte le vite
    }
}

