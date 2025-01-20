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
    WINDOW* whud=newwin(HUDLINES, NCOLS, 0, 0); //finestra che segna le vite e il tempo
    WINDOW* debug=newwin(20,20, 0, NCOLS + TCOLS+ 5);
    WINDOW* wtempo=newwin(NLINES, TCOLS, HUDLINES, NCOLS + 1);
    //todo pensavo di fare una finestra verticale al lato che segna il tempo però poi vediamo
    
    Flusso* fiume=avviaFlussi();
    Processo rana=avviaRana(pipe_fd, pipe_inversa);
    Processo cricca[NUMERO_FLUSSI*MAX_COCCODRILLI];//cricca di coccodrilli
    Processo granate[N_GRANATE];
    Processo astuccio[N_PROIETTILI]; //astuccio dei proiettili
    Processo temp;
    _Bool tane[NUMERO_TANE]={0}; //essenzialmente se è false la tana è libera ()
    int vite=MAX_VITE;
    int punteggio=0; //!poi dobbiamo decidere i vari guadagni di punteggio

    temp.pid==0; //così al primo detect collisione non succede nulla

    avviaProiettili(astuccio); // inizializza i pid 
    avviaCoccodrilli(pipe_fd, fiume, cricca); 

    for(int i=0; i<N_GRANATE; i++)
    {
        granate[i].item.id=NULL;
    }

    close(pipe_fd[1]);
    close(pipe_inversa[0]);

    
    time_t start=time(NULL);
    _Bool reset=false; // la uso per capire se devo resettare posizione rana e tempo
    while(vite>0)
    {
        reset=false;
        //todo aggiungere un check per la vittoria
        //todo mettere almeno i colori
        //todo creare una pipesuono e un processo che gestisca la musica e i suoni di gioco(in quell trovato su github fanno così)
        //gestione stampa
        werase(wgioco);
        box(wgioco, ACS_VLINE, ACS_HLINE);
        handleHud(whud, vite , start);
        handleTempo(wtempo, start);
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

        for(int i=0; i<N_PROIETTILI; i++)
        {
            if(astuccio[i].pid!=-1)
            {
                stampaProiettile(astuccio[i].item, wgioco);
            }
        }
        
        /*
        //check flussi
        for(int i=0; i<NUMERO_FLUSSI; i++)
        {
            mvwprintw(wgioco, fiume[i].y, 2, "Y:%d, dir:%d, speed:%d", fiume[i].y, fiume[i].dir, fiume[i].speed);
        }
        */
        

        wnoutrefresh(wgioco);
        wnoutrefresh(whud);
        wnoutrefresh(wtempo);
        wnoutrefresh(debug);
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
                break;
            case '-':
                _Bool trovato = false;
                if(temp.item.x < 1 ||  temp.item.x > NCOLS)
                {
                    mvwprintw(debug, 1, 1, "è da cancellare");
                    for(int i = 0; i < N_PROIETTILI; i++)
                    {
                        if(astuccio[i].pid==temp.pid)
                        {
                            mvwprintw(debug, 2, 1, "cancellato");
                            kill(astuccio[i].pid, 9);
                            astuccio[i].pid=-1;
                            trovato=true;
                            break;
                        }
                    }
                }
                if(trovato) break;
                trovato=false;
                for(int i = 0; i < N_PROIETTILI; i++) 
                {
                    mvwprintw(debug, 3, 1, "porcodio");
                    if(astuccio[i].pid == temp.pid) {
                        astuccio[i] = temp;
                        trovato = true;
                        break;
                    }
                }
                if(!trovato) 
                {
                    for(int i = 0; i < N_PROIETTILI; i++) 
                    {
                        if(astuccio[i].pid<0) {  // slot libero
                            astuccio[i] = temp;
                            trovato=true;
                            break;
                        }
                    }
                }
                if(!trovato)
                {
                    mvwprintw(debug, 2, 1, "ammazzato: %d", temp.pid);
                    kill(temp.pid, 9);
                }
                break;
        }
        reset=detectCollisione(&rana, cricca, astuccio, granate, tane, temp, wgioco, debug, &vite, time(NULL)-start, &punteggio);
        if(reset){ //resetto la posizione della rana e il tempo
            rana.item.x=NCOLS / 2 - LARGHEZZA_RANA / 2;
            rana.item.y=NLINES - ALTEZZA_RANA - 1;
            start=time(NULL);
            for(int i=0; i < N_PROIETTILI; i++){
                if(astuccio[i].pid >0){
                    kill(astuccio[i].pid, 9);
                }
            }
            avviaProiettili(astuccio); //inizializza di nuovo tutto 
        }
        usleep(1100);
    }

    //todo funzione che ammazza tutto quanto quando si esce dal loop principale
}

//!versione claudio più robusta e che funziona!!!

BoundingBox createBoundingBox(int x, int y, int larghezza, int altezza) {
    BoundingBox box = {
        .x = x,
        .y = y,
        .width = larghezza,
        .height = altezza
    };
    return box;
}

_Bool checksovrapposizione(BoundingBox a, BoundingBox b) {
    return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
}

int checkTaneCollision(int x, int y) {
    for(int i = 0; i < NUMERO_TANE; i++) { //per ogni tana
        int taneX = 8 + i * (6 + 7);  
        if(x >= taneX && x + LARGHEZZA_RANA < taneX + 6 && y < ALTEZZA_TANE) { //check se sta dentro la tana
            return i;
        }
    }
    return -1;
}

// Helper function to handle frog death
void handleMorteRana(Processo* rana, int* vite, WINDOW* wgioco) {
    (*vite)--;
    
    werase(wgioco);
    mvwprintw(wgioco, NLINES/2, NCOLS/2 - 5, "Vita persa");
    wnoutrefresh(wgioco);
    doupdate();
    sleep(1);
}

// funzione principale per le collisioni
_Bool detectCollisione(Processo* rana, Processo* cricca, Processo* astuccio, Processo* granate, _Bool* tane, Processo last, WINDOW* wgioco, WINDOW* debug, int* vite, int secondi, int* punti) {
    // crea la box rana
    BoundingBox ranaBox = createBoundingBox(rana->item.x, rana->item.y, LARGHEZZA_RANA,ALTEZZA_RANA);

    // Check tempo
    if(secondi > MAX_TEMPO) {
        handleMorteRana(rana, vite, wgioco);
        return true;
    }

    // Check delle varie zone
    if(rana->item.y > NLINES - ALTEZZA_MARCIAPIEDE) {  
        return false;// sta nel marciapiede quindi safe
    }

    // Check tane 
    if(rana->item.y < ALTEZZA_TANE){
        int taneIndex = checkTaneCollision(rana->item.x, rana->item.y);
        if(taneIndex >= 0) {
            if(!tane[taneIndex]) {
                tane[taneIndex] = 1;
                *punti += 100;  //!da decidere quanti punti, messi a caso qua
                return true;
            }
            else { // altrimenti la tana è occupata
                handleMorteRana(rana, vite, wgioco);
                return true;
            }
        }else {
                handleMorteRana(rana, vite, wgioco);
                return true;
        }
    }


    // check sponda
    if(rana->item.y > ALTEZZA_TANE && 
       rana->item.y < ALTEZZA_TANE + ALTEZZA_SPONDA) {
        return false;  //safe 
    }

    // Check collisione con coccodrillo
    _Bool isOnCoccodrillo = false;
    for(int i = 0; i < NUMERO_FLUSSI * MAX_COCCODRILLI; i++) {
        if(cricca[i].pid != 0) {  // prima controlla se è attivo
            BoundingBox coccoBox = createBoundingBox(cricca[i].item.x, cricca[i].item.y, LARGHEZZA_COCCODRILLO, ALTEZZA_COCCODRILLO);

            if(checksovrapposizione(ranaBox, coccoBox)) {
                isOnCoccodrillo = true;
                
                // se è il coccodrillo che si sta muovendo si muove anche la rana 
                if(cricca[i].pid == last.pid) {
                    rana->item.x += last.item.dir;
                    
                    // check dei confini dello schermo
                    if(rana->item.x < 1) {
                        rana->item.x = 1;
                    }
                    if(rana->item.x > NCOLS - LARGHEZZA_RANA - 1) {
                        rana->item.x = NCOLS - LARGHEZZA_RANA - 1;
                    }
                    
                    //! A QUANTO PARE NON SCIVOLA PIÙ GODO
                    /*// questo per quando scivola 
                    if(rana->item.x < last.item.x) {
                        rana->item.x = last.item.x;
                    }
                    if(rana->item.x + LARGHEZZA_RANA > 
                       last.item.x + LARGHEZZA_COCCODRILLO) {
                        rana->item.x = last.item.x + 
                                     LARGHEZZA_COCCODRILLO - 
                                     LARGHEZZA_RANA;
                    }*/
                }
                break;
            }
        }
    }

    // se non è su un coccodrillo e sta nel fiume morte assicurata
    if(!isOnCoccodrillo && 
       rana->item.y > ALTEZZA_TANE + ALTEZZA_SPONDA && 
       rana->item.y < NLINES - ALTEZZA_MARCIAPIEDE) {
        handleMorteRana(rana, vite, wgioco);
        return true;
    }


    /*
    //check rana e proiettile
    for(int i=0; i < N_PROIETTILI; i++){
        if(astuccio[i].pid < 0) continue; // se non è stato inizializzato va al prossimo ciclo
        BoundingBox proiettileBox=createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1);
        if(checksovrapposizione(ranaBox, proiettileBox)){
            handleMorteRana(rana, vite, wgioco);
            return true;
        }
    }*/
    

    // check granate e proiettili
    for(int i=0; i < N_PROIETTILI; i++){
        if(astuccio[i].pid < 0) continue;
        BoundingBox proiettileBox=createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1);
        for(int j=0; j< N_GRANATE; j++){
            BoundingBox granataBox=createBoundingBox(granate[i].item.x, granate[i].item.y, 1, 1);
            if(checksovrapposizione(granataBox, proiettileBox)){
                kill(astuccio[i].pid, 9);
                kill(granate[i].pid, 9);
                astuccio[i].pid=-1;
                granate[i].pid=-1;
            }
        }
    }
    return false;
}

//! da qua l'ho fatto io
void handleHud(WINDOW* whud, int vite, int start){
    werase(whud);
    box(whud,ACS_VLINE, ACS_HLINE );

    for(int i=0; i < vite; i++){ // per ogni vita stampa una rana
        stampaRana((Oggetto){' ', 1, i* LARGHEZZA_RANA + 1, 0}, whud);
    }
    mvwprintw(whud, 1, 40, "%d", time(NULL)-start);
}

void handleTempo(WINDOW* wtempo, int start){
    werase(wtempo);
    box(wtempo, ACS_VLINE, ACS_HLINE);
    int secondi=time(NULL)-start;
    float ratio= (MAX_TEMPO-secondi) / (float)MAX_TEMPO; // quanto tempo è passato rispetto al massimo
    int limit= ratio * (NLINES-1); //  indica quante righe occupo
    for(int i=limit; i > 0 ; i--){
        for(int j=1; j < TCOLS-1; j++)
            mvwaddch(wtempo, NLINES-i, j, ' ' | A_REVERSE);
    }
}
