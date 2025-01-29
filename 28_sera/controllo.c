#include "controllo.h"

void controllo(int* pipe_fd, int* pipe_inversa)
{
    WINDOW* wgioco=newwin(NLINES, NCOLS, HUDLINES, 0); //finestra di gioco con coccodrilli e rana
    WINDOW* whud=newwin(HUDLINES, NCOLS, 0, 0); //finestra che segna le vite e il tempo
    WINDOW* debug=newwin(40,20, 0, NCOLS + TCOLS+ 5);
    WINDOW* wtempo=newwin(NLINES, TCOLS, HUDLINES, NCOLS + 1); // colonna del tempo
    
    Flusso* fiume=avviaFlussi();
    Processo rana=avviaRana(pipe_fd, pipe_inversa);
    Processo cricca[NUMERO_FLUSSI*MAX_COCCODRILLI];//cricca di coccodrilli
    Processo granate[N_GRANATE];
    Processo astuccio[N_PROIETTILI]; //astuccio dei proiettili
    Processo temp;
    _Bool tane[NUMERO_TANE]={0}; //essenzialmente se è false la tana è libera ()
    int vite=MAX_VITE;
    Punteggio punteggio=inizializzaPunteggio();


    temp.pid=0; //così al primo detect collisione non succede nulla

    avviaProiettili(astuccio); // inizializza i pid 
    avviaCoccodrilli(pipe_fd, fiume, cricca); 
    inizializzaGranate(granate);

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
        handleHud(whud, vite, punteggio);
        handleTempo(wtempo, start);
        mvwprintw(whud, 1, 20, "vite: %d", vite);
        stampaTane(wgioco, tane);
        stampaMarciapiede(wgioco);
        stampaCoccodrilli(cricca, wgioco);
        stampaRana(rana.item, wgioco);
        

        for(int i=0; i<N_GRANATE; i++)
        {
            if(granate[i].pid>0)
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
        
       for(int i=0; i<N_PROIETTILI; i++)
       {
        mvwprintw(debug, i+1, 1, "slot %d, x %d, y %d", i, astuccio[i].item.x, astuccio->item.y);
       }
        

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
                _Bool trovate = false;
                if(temp.item.x < 1 ||  temp.item.x > NCOLS)
                {
                    for(int i = 0; i < N_GRANATE; i++)
                    {
                        if(granate[i].pid==temp.pid)
                        {
                            if(temp.pid>1) kill(granate[i].pid, 9);
                            granate[i].pid=-1;
                            trovate=true;
                            break;
                        }
                    }
                }
                if(trovate) break;
                for(int i = 0; i < N_GRANATE; i++) 
                {
                    if(granate[i].pid == temp.pid) {
                        granate[i] = temp;
                        trovate = true;
                        break;
                    }
                }
                if(!trovate) 
                {
                    for(int i = 0; i < N_PROIETTILI; i++) 
                    {
                        if(granate[i].pid<0) {  // slot libero
                            granate[i] = temp;
                            trovate=true;
                            break;
                        }
                    }
                }
                if(!trovate)
                {
                    if(temp.pid>1) kill(temp.pid, 9);
                }
                break;
            case 'g': 
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
                    for(int i = 0; i < N_PROIETTILI; i++)
                    {
                        if(astuccio[i].pid==temp.pid)
                        {
                            if(astuccio[i].pid>1) kill(astuccio[i].pid, 9);
                            astuccio[i].pid=-1;
                            trovato=true;
                            break;
                        }
                    }
                }
                //if(trovato) break;
                //trovato=false;
                if(!trovato)
                {
                    for(int i = 0; i < N_PROIETTILI; i++) 
                    {
                        if(astuccio[i].pid == temp.pid) {
                            astuccio[i] = temp;
                            trovato = true;
                            break;
                        }
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
                    if(temp.pid>1) kill(temp.pid, 9);
                }
                break;
        }

        reset=detectCollisione(&rana, cricca, astuccio, granate, tane, temp, wgioco, debug, &vite, time(NULL)-start, &punteggio);

        if(reset){ //resetto la posizione della rana e il tempo
            rana.item.x=NCOLS / 2 - LARGHEZZA_RANA / 2;
            rana.item.y=NLINES - ALTEZZA_RANA - 1;
            start=time(NULL);
            for(int i=0; i < N_PROIETTILI; i++){
                if(astuccio[i].pid > 1){
                    kill(astuccio[i].pid, 9);
                    waitpid(astuccio[i].pid, NULL, 0);
                }
            }
            usleep(10000);
            avviaProiettili(astuccio); //inizializza di nuovo tutto 
            inizializzaGranate(granate);
        }

        if(checkWin(tane)){
            break; // se vinci esci dal loop principale
        }
        usleep(1100);
    }
    flash();
    menuFinale(punteggio, vite);
    cleanup(rana, cricca, astuccio, granate);
}

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
    return (a.x + a.width <= b.x + b.width &&
            a.x + a.width >= b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
}

_Bool checkWin(_Bool* tane){
    for(int i=0; i < NUMERO_TANE; i ++){
        if(!tane[i]) return false; //se ce n'è una libera non si ha ancora vinto la partita
    }
    return true;
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

void handleMorteRana(Processo* rana, int* vite, Punteggio* punti, WINDOW* wgioco) {
    (*vite)--;
    punti->morte += PUNTI_MORTE;
    
    werase(wgioco);
    //!potremmo fare un figlet con manche over? o si incasina con la temporizzazione? 
    mvwprintw(wgioco, NLINES/2, NCOLS/2 - 5, "Vita persa");
    wnoutrefresh(wgioco);
    doupdate();

    }

// funzione principale per le collisioni
_Bool detectCollisione(Processo* rana, Processo* cricca, Processo* astuccio, Processo* granate, _Bool* tane, Processo last, WINDOW* wgioco, WINDOW* debug, int* vite, int secondi, Punteggio* punti) {
    checkPuntiSalto(punti, last);
    // crea la box rana
    BoundingBox ranaBox = createBoundingBox(rana->item.x, rana->item.y, LARGHEZZA_RANA,ALTEZZA_RANA);

    // Check tempo
    if(secondi > MAX_TEMPO) {
        handleMorteRana(rana, vite, punti, wgioco);
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
                punti->tane += PUNTI_TANA;
                punti->tempo+= ( MAX_TEMPO - secondi)*PUNTI_PER_SEC;
                (*vite)=MAX_VITE;
                return true;
            }
            else { // altrimenti la tana è occupata
                handleMorteRana(rana, vite, punti, wgioco);
                return true;
            }
        }else {
                handleMorteRana(rana, vite,punti,  wgioco);
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
                }
                break;
            }
        }
    }

    // se non è su un coccodrillo e sta nel fiume morte assicurata
    if(!isOnCoccodrillo && 
       rana->item.y > ALTEZZA_TANE + ALTEZZA_SPONDA && 
       rana->item.y < NLINES - ALTEZZA_MARCIAPIEDE) {
        handleMorteRana(rana, vite,punti, wgioco);
        return true;
    }


    
    //check rana e proiettile
    for(int i=0; i < N_PROIETTILI; i++){
        if(astuccio[i].pid < 0) continue; // se non è stato inizializzato va al prossimo ciclo
        BoundingBox proiettileBox=createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1);
        if(checksovrapposizione(ranaBox, proiettileBox)){
            handleMorteRana(rana, vite, punti, wgioco);
            return true;
        }
    }
    

    // check granate e proiettili
    for(int i=0; i < N_PROIETTILI; i++){
        if(astuccio[i].pid >1)
        {
            BoundingBox proiettileBox=createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1);
            for(int j=0; j< N_GRANATE; j++){
                if(granate[j].pid >1)
                {
                    BoundingBox granataBox=createBoundingBox(granate[j].item.x, granate[j].item.y, 1, 1);
                    if(checksovrapposizione(granataBox, proiettileBox)){
                        punti->proiettili+=PUNTI_PROIETTILI;

                        if(astuccio[i].pid > 1){
                            kill(astuccio[i].pid, 9);
                            astuccio[i].pid=-1;
                        }
                        
                        if(granate[i].pid >1){
                            kill(granate[j].pid, 9);
                            granate[j].pid=-1;
                        }
                        
                        
                        
                    }
                } 
            }
        }
        
    }
    return false;
}

void handleHud(WINDOW* whud, int vite, Punteggio punti){
    werase(whud);
    box(whud,ACS_VLINE, ACS_HLINE );

    for(int i=0; i < vite; i++){ // per ogni vita stampa una rana
        stampaRana((Oggetto){' ', 1, i* LARGHEZZA_RANA + 1, 0}, whud);
    }

    int totale=0;
    totale+=punti.tane;
    totale+=punti.proiettili;
    totale+=punti.salti;
    totale+=punti.tempo;
    totale-=punti.morte;
    if(totale <0) totale=0;
    mvwprintw(whud,1, 30, "punteggio: %d", totale );
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
