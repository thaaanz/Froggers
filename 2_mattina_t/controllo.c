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
    Flusso fiume[NUMERO_FLUSSI];
    avviaFlussi(fiume);
    Thread rana={0,{'r', NLINES- ALTEZZA_RANA-1, NCOLS/2, NULL }}; // qua in realtà potremmo fare avvia rana che restituisce il tid
    Thread cricca[NUMERO_FLUSSI*MAX_COCCODRILLI];//cricca di coccodrilli
    Thread granate[N_GRANATE];
    Thread astuccio[N_PROIETTILI]; //astuccio dei proiettili
    _Bool tane[NUMERO_TANE]={0}; //essenzialmente se è false la tana è libera 
    int vite=MAX_VITE;
    //Punteggio punteggio=inizializzaPunteggio();

    Punteggio punti;

    punti.tempo=0;
    punti.proiettili=0;
    punti.salti=0;
    punti.tane=0;
    punti.morte=0;

    avviaCoccodrilli(fiume, cricca, m);

    inizializzaGranate(granate);
    avviaProiettili(astuccio);

    time_t start=time(NULL);
    _Bool reset=false; // la uso per capire se devo resettare posizione rana e tempo

    while(vite>0)
    {
        reset=false;
        //gestione stampa
        werase(wgioco);
        werase(debug);
        stampaFiume(wgioco);
        stampaTane(wgioco, tane);
        stampaMarciapiede(wgioco);
        stampaSponda(wgioco);
        stampaCoccodrilli(cricca, wgioco);
        stampaRana(rana.item, wgioco);

        for(int i=0; i<N_GRANATE; i++)
        {
            mvwprintw(debug, 5+i, 4, " %d", granate[i].tid);
            if(granate[i].tid != -1)
            {
                stampaGranata(granate[i].item, wgioco);
            }
        }

        for(int i=0; i<N_PROIETTILI; i++)
        {
            if(astuccio[i].tid != -1)
            {
                stampaProiettile(astuccio[i].item, wgioco);
                mvwprintw(debug, i, 20, " %d %d %d", astuccio[i].item.y, astuccio[i].item.x, astuccio[i].tid);
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
                mvwprintw(debug, 3, 1, " %d %d", temp.item.x, temp.tid);
                _Bool trovati = false;

                for (int i = 0; i < N_PROIETTILI; i++) {
                    if (astuccio[i].tid == temp.tid) {
                        astuccio[i] = temp;
                        trovati = true;
                        
                        // Check se è fuori dai bordi
                        if (astuccio[i].item.x < 0 || astuccio[i].item.x > NCOLS) {
                            if (astuccio[i].tid != -1) {
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
                
                if (!trovati && temp.tid != -1) {
                    pthread_cancel(temp.tid);
                    pthread_join(temp.tid, NULL);
                }

                break;
        }

        reset=detectCollisione(&rana, cricca, astuccio, granate, tane, temp, wgioco, debug, &vite, time(NULL)-start, &punti);

        if(reset){ //resetto la posizione della rana e il tempo
            rana.item.x=NCOLS / 2 - LARGHEZZA_RANA / 2;
            rana.item.y=NLINES - ALTEZZA_RANA - 1;
            start=time(NULL);
            for(int i=0; i < N_PROIETTILI; i++){
                if(astuccio[i].tid != -1){
                    pthread_cancel(astuccio[i].tid);
                    pthread_join(astuccio[i].tid, NULL);
                }
            }
            usleep(200);
            //inizializza di nuovo tutto 
            avviaProiettili(astuccio); 
            inizializzaGranate(granate);
        }

        if(checkWin(tane)){
            break; // se vinci esci dal loop principale
        }
        
        wnoutrefresh(debug);
        wnoutrefresh(wgioco);
        doupdate();
        usleep(500);
    }

    return NULL;
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
            a.x + a.width > b.x &&
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


void handleMorteRana(Thread* rana, int* vite, Punteggio* punti, WINDOW* wgioco) {
    (*vite)--;
    punti->morte += PUNTI_MORTE;
    
    werase(wgioco);
    //!potremmo fare un figlet con manche over? o si incasina con la temporizzazione? 
    mvwprintw(wgioco, NLINES/2, NCOLS/2 - 5, "Vita persa");
    wnoutrefresh(wgioco);
    doupdate();

    }

// funzione principale per le collisioni
_Bool detectCollisione(Thread* rana, Thread* cricca, Thread* astuccio, Thread* granate, _Bool* tane, Thread last, WINDOW* wgioco, WINDOW* debug, int* vite, int secondi, Punteggio* punti) {
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
            if(!tane[taneIndex]) { // se la tana è libera
                tane[taneIndex] = 1; // la occupa
                punti->tane += PUNTI_TANA; //assegna i punti
                punti->tempo+= ( MAX_TEMPO - secondi)*PUNTI_PER_SEC;
                (*vite)=MAX_VITE;
                return true; //vanno resettati tempo e posizione rana
            }
            else { // se la tana è occupata
                handleMorteRana(rana, vite, punti, wgioco);
                return true;
            }
        }else { // miss della tana
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
        if(cricca[i].tid != -1) {  // prima controlla se è attivo
            BoundingBox coccoBox = createBoundingBox(cricca[i].item.x, cricca[i].item.y, LARGHEZZA_COCCODRILLO, ALTEZZA_COCCODRILLO);

            if(checksovrapposizione(ranaBox, coccoBox)) {
                isOnCoccodrillo = true;
                
                // se è il coccodrillo che si sta muovendo si muove anche la rana 
                if(cricca[i].tid == last.tid) {
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
        if(astuccio[i].tid < 0) continue; // se non è stato inizializzato va al prossimo slot
        BoundingBox proiettileBox=createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1);
        if(checksovrapposizione(ranaBox, proiettileBox)){
            handleMorteRana(rana, vite, punti, wgioco);
            return true;
        }
    }
    

    // check granate e proiettili
    for(int i=0; i < N_PROIETTILI; i++){
        if(astuccio[i].tid != -1)
        {
            BoundingBox proiettileBox=createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1);
            for(int j=0; j< N_GRANATE; j++){
                if(granate[j].tid != -1)
                {
                    BoundingBox granataBox=createBoundingBox(granate[j].item.x, granate[j].item.y, 1, 1);
                    if(checksovrapposizione(granataBox, proiettileBox)){
                        punti->proiettili+=PUNTI_PROIETTILI;

                        if(astuccio[i].tid != -1){
                            pthread_cancel(astuccio[i].tid);
                            pthread_join(astuccio[i].tid, NULL);
                            astuccio[i].tid=-1;
                        }
                        
                        if(granate[j].tid != -1){
                            pthread_cancel(granate[j].tid);
                            pthread_join(granate[j].tid, NULL);
                            granate[j].tid=-1;
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
    if(totale < 0) totale=0;
    mvwprintw(whud,1, 30, "punteggio: %d", totale );
}

void handleTempo(WINDOW* wtempo, int start){
    werase(wtempo);
    box(wtempo, ACS_VLINE, ACS_HLINE);
    int secondi=time(NULL)-start;
    float ratio= (MAX_TEMPO-secondi) / (float)MAX_TEMPO; // quanto tempo è passato rispetto al massimo
    int limit= ratio * (NLINES-1); //  indica quante righe occupo
    for(int i=limit; i > 0 ; i--){
        short color_pair;
        //todo qua i codici del color pair sono messi a caso era solo per fare una prova (che funziona) DA CAMBIARE 
        if(ratio > 0.7) color_pair = 1;        // verde
        else if(ratio > 0.3) color_pair = 3;    // giallo
        else color_pair = 4;                    // rosso
        for(int j=1; j < TCOLS-1; j++){
            wattron(wtempo, COLOR_PAIR(color_pair));
            mvwaddch(wtempo, NLINES-i, j, ' ' | A_REVERSE);
            wattroff(wtempo, COLOR_PAIR(color_pair));
        }     
    }
}