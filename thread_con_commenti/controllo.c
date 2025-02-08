#include "controllo.h"

void *controllo(void *semafori)
{
    Semafori *s = (Semafori *)semafori;

    WINDOW *wgioco = newwin(NLINES, NCOLS, HUDLINES, 0); // finestra di gioco con coccodrilli e rana
    WINDOW *whud = newwin(HUDLINES, NCOLS, 0, 0);        // finestra che segna le vite e il tempo
    WINDOW *wtempo = newwin(NLINES - 1, TCOLS, HUDLINES, NCOLS); // colonna del tempo

    wbkgd(whud, COLOR_PAIR(COLORI_HUD)); //imposto il colore di bg delle finestre
    wbkgd(wtempo, COLOR_PAIR(COLORI_TEMPO));

    //audio
    GameAudio audio; //inizializzo la strttura contenente gli audio di gioco
    inizializzaAudio(&audio);//inizilizzo l'audio
    riproduciMusica(&audio);//faccio partire la musica di sottofondo
    _Bool tane[NUMERO_TANE] = {0}; // essenzialmente se è false la tana è libera
    int vite = MAX_VITE; //inizializzo le vite
    Punteggio punti = inizializzaPunteggio();

    while(vite > 0){ //fino a che sei in vita
        //grafica new manche
        stampaNewManche(wgioco);
        // inizializzazioni
        Flusso fiume[NUMERO_FLUSSI];
        avviaFlussi(fiume); //crea i flussi
        Thread rana = avviaRana(s); //avvia thread rana
        Thread cricca[NUMERO_FLUSSI * MAX_COCCODRILLI]; // cricca di coccodrilli
        Thread granate[N_GRANATE]; //creo array granate
        Thread astuccio[N_PROIETTILI]; // creo array astuccio dei proiettili
        Thread temp; //thread temporaneo per la lettura dal buffer

        Oggetto mine[N_MINE]; //creo le mine (ostacoli in più nella sponda)
        inizializzaMine(mine); //inizializza array mine

        avviaCoccodrilli(fiume, cricca, s); //avvia thread coccodrilli
        inizializzaGranate(granate); //inizializza array granate
        inizializzaProiettili(astuccio); //inizializza array proiettili

        time_t start = time(NULL); //tempo di inizio
        _Bool reset = false; // la uso per capire se devo resettare posizione rana e tempo

        while (!reset)
        {
            reset = false;
            // gestione stampa
            werase(wgioco);

            handleTempo(wtempo, start); //gestisco il tempo
            handleHud(whud, vite, punti); //stampa di punteggio e vite

            stampaFiume(wgioco); //stampo i flussi
            stampaTane(wgioco, tane); //stampo le tane
            stampaMarciapiede(wgioco); //stampo il marciapiede di partenza
            stampaSponda(wgioco); //stampo la sponda sotto le tane
            stampaCoccodrilli(cricca, wgioco); //stampo i coccodrilli
            stampaRana(rana.item, wgioco); //stampo la rana
            stampaMine(mine, wgioco); //stampo le mine
            stampaGranate(granate, wgioco); //stampo le granate 
            stampaProiettili(astuccio, wgioco); //stampo i proiettili

            //aggiorno le finestre
            wnoutrefresh(wgioco);
            redrawwin(whud);
            wnoutrefresh(whud);
            redrawwin(wtempo);
            wnoutrefresh(wtempo);
            doupdate();

            temp = leggi(s); // lettura dal buffer 

            switch (temp.item.id)
            {
            case 'r': //caso rana
                riproduciSuono(audio.salto_rana);
                //aggiorno la poszione
                rana.tid=temp.tid;
                rana.item.x += temp.item.x;
                rana.item.y += temp.item.y;
                //mi assicuro che la rana resti allinterno dei limiti della finestra di gioco
                if (rana.item.x <= 0)
                    rana.item.x = 1;
                if (rana.item.y <= 0)
                    rana.item.y = 1;
                if (rana.item.x >= NCOLS - LARGHEZZA_RANA)
                    rana.item.x = NCOLS - LARGHEZZA_RANA - 1;
                if (rana.item.y >= NLINES - ALTEZZA_RANA)
                    rana.item.y = NLINES - ALTEZZA_RANA - 1;
                break;
            case 'q':
                cleanup; freeAudio(&audio); endwin(); return 0;
            case 'g':  // caso granata (quando la rana spara)
                //controllo se c'è almeno una granata disponibile
                for(int i=0; i < N_GRANATE; i++){
                    if(granate[i].tid == -1){ //quando trovo lo spazio disponibile
                        avviaGranate(rana, semafori);
                        break;
                    }
                }
                break;
            case 'o': //caso granata (aggiornamento posizione)
                if(temp.item.dir==0) riproduciSuono(audio.sparo_granata); //per fare il suono solo una volta
                //l'attributo direzione restava inutilizzato quindi lo uso come controllo per fare in modo che il suono venga riprodotto solo una volta per sparo e non ogni volta che la pipe legge una granata
                posizionaSparo(temp, granate, N_GRANATE); //inserisco la granata nell'array per aggiornare la sua posizione
                break;

            case 'c': // caso coccodrillo  
                for (int i = 0; i < NUMERO_FLUSSI * MAX_COCCODRILLI; i++) // scorre l'array
                {
                    if (cricca[i].tid == temp.tid) // cerca il coccodrillo corrispondente
                    {
                        cricca[i] = temp; // aggiorna il coccodrillo
                    }
                }
                break;
            case '-': // caso proiettile
                posizionaSparo(temp, astuccio, N_PROIETTILI); //inserisco il proiettile nell'array per aggiornare la sua posizione
                break;
            }

            reset = detectCollisione(&rana, cricca, astuccio, granate,mine, tane, temp, wgioco, &vite, time(NULL) - start, &punti, audio);

            if (reset)
            { // resetto la posizione della rana e il tempo
                rana.item.x = NCOLS / 2 - LARGHEZZA_RANA / 2;
                rana.item.y = NLINES - ALTEZZA_RANA - 1;
                start = time(NULL);
                //termino granate e proiettili
                for (int i = 0; i < N_PROIETTILI; i++)
                {
                    if(astuccio[i].tid != -1){
                        if (pthread_kill(astuccio[i].tid, 0) == 0) {
                            if (pthread_cancel(astuccio[i].tid) == 0) {
                                pthread_join(astuccio[i].tid, NULL);
                            }
                            else exit(ERRORE_CANCEL_PROIETTILI);
                        }
                    }
                }
                for (int i = 0; i < N_GRANATE; i++)
                {
                    if (granate[i].tid != -1)
                    {
                        if (pthread_kill(granate[i].tid, 0) == 0) {
                            if (pthread_cancel(granate[i].tid) == 0) {
                                pthread_join(granate[i].tid, NULL);
                            }
                        }
                    }
                }
                // inizializza di nuovo tutto
                inizializzaProiettili(astuccio);
                inizializzaGranate(granate);
                inizializzaMine(mine);
            }

            usleep(DELAY_CONTROLLO);
        }
        // fuori dal loop della manche
        if (checkWin(tane))
        {
            break; // se vinci esci dal loop principale
        }
        cleanup(rana, cricca, astuccio, granate);
        usleep(DELAY_CONTROLLO);
    }
    //fuori dal loop principale
    menuFinale(punti, vite);
    delwin(wgioco);
    delwin(whud);
    delwin(wtempo);
    freeAudio(&audio);
    pthread_exit(NULL);
}

BoundingBox createBoundingBox(int x, int y, int larghezza, int altezza)
{
    BoundingBox box = {
        .x = x,
        .y = y,
        .width = larghezza,
        .height = altezza};
    return box;
}

_Bool checksovrapposizione(BoundingBox a, BoundingBox b)
{
    return (a.x + a.width <= b.x + b.width && // l'estremo destro di a èa sinistra dell'estremo destro di b
            a.x + a.width > b.x && // l'estremo destro di a è a destra dell'estremo sinistro di b
            a.y < b.y + b.height && // a è più in basso dell'estremo superiore di b
            a.y + a.height > b.y); // a è più in alto dell'estremo inferiore di b
}

_Bool checkWin(_Bool *tane)
{
    for (int i = 0; i < NUMERO_TANE; i++) //scorro le tane
    {
        if (!tane[i]) //controllo se ce n'è una libera
            return false; // nel caso non si ha ancora vinto la partita
    }
    return true; //si ha vinto
}

int checkTaneCollision(int x, int y, GameAudio audio)
{
    for (int i = 0; i < NUMERO_TANE; i++)
    { // per ogni tana
        int taneX = 8 + i * (6 + 7); //calcolo in quale x inizia la tana
        if (x >= taneX && x + LARGHEZZA_RANA < taneX + 6 && y < ALTEZZA_TANE) //controllo se la rana sta dentro la tana
        { 
            riproduciSuono(audio.chiusura_tana);
            return i; // return del numero della tana chiusa
        }
    }
    return -1;
}

void handleMorteRana(Thread *rana, int *vite, Punteggio *punti, WINDOW *wgioco, GameAudio audio)
{
    riproduciSuono(audio.morte_rana);
    (*vite)--;
    punti->morte += PUNTI_MORTE;
}

// funzione principale per le collisioni
_Bool detectCollisione(Thread *rana, Thread *cricca, Thread *astuccio, Thread *granate, Oggetto* mine, _Bool *tane, Thread last, WINDOW *wgioco, int *vite, int secondi, Punteggio *punti, GameAudio audio)
{
    checkPuntiSalto(punti, last);
    // crea la box rana
    BoundingBox ranaBox = createBoundingBox(rana->item.x, rana->item.y, LARGHEZZA_RANA, ALTEZZA_RANA);

    // Check tempo
    if (secondi > MAX_TEMPO) // se si supera il tempo massimo
    {
        handleMorteRana(rana, vite, punti, wgioco, audio); // si muore
        return true; //reset di posizione e tempo
    }

    // Check delle varie zone
    if (rana->item.y > NLINES - ALTEZZA_MARCIAPIEDE)
    {
        return false; // sta nel marciapiede quindi è safe
    }

    // Check tane
    if (rana->item.y < ALTEZZA_TANE) // se la rana è nell'altezza delle tane
    {
        int taneIndex = checkTaneCollision(rana->item.x, rana->item.y, audio); //controllo se sta dentro una tana
        if (taneIndex >= 0)
        {
            if (!tane[taneIndex])// se la tana è libera
            {                              
                tane[taneIndex] = 1;       // la occupa
                punti->tane += PUNTI_TANA; // assegna i punti
                punti->tempo += (MAX_TEMPO - secondi) * PUNTI_PER_SEC;
                (*vite) = MAX_VITE;
                return true; // vanno resettati tempo e posizione rana
            }
            else
            { // se la tana è occupata
                handleMorteRana(rana, vite, punti, wgioco, audio);
                return true; // vanno resettati tempo e posizione rana
            }
        }
        else
        { // miss della tana
            handleMorteRana(rana, vite, punti, wgioco, audio);
            return true; //vanno resettati tempo e posizione rana
        }
    }

    // check sponda
    if(rana->item.y > ALTEZZA_TANE && 
       rana->item.y < ALTEZZA_TANE + ALTEZZA_SPONDA) { // se è all'altezza della sponda
        //check rana e mine
        for(int i=0; i < N_MINE; i++){ //scorro le mine
            BoundingBox mineBox = {mine[i].x, mine[i].y, 1, 1}; // creo la box per la mina
            if(checksovrapposizione(mineBox, ranaBox)){ // se la prende la rana muore
                handleMorteRana(rana, vite, punti, wgioco, audio);
                return true; //resetto tempo e posizione rana
            }
        }
        return false;  //safe 
    }

    // Check collisione con coccodrillo
    _Bool isOnCoccodrillo = false; //flag per sapere se la rana sta sul coccodrillo
    for (int i = 0; i < NUMERO_FLUSSI * MAX_COCCODRILLI; i++) //scorro i coccodrilli
    {
        if (cricca[i].tid != -1)
        { // prima controlla se è attivo
            BoundingBox coccoBox = createBoundingBox(cricca[i].item.x, cricca[i].item.y, LARGHEZZA_COCCODRILLO, ALTEZZA_COCCODRILLO); //creo la box coccodrillo

            if (checksovrapposizione(ranaBox, coccoBox)) //se la rana sta sopra il coccodrillo
            {
                isOnCoccodrillo = true;

                // se è il coccodrillo che si sta muovendo si muove anche la rana
                if (cricca[i].tid == last.tid)
                {
                    rana->item.x += last.item.dir;

                    // check dei confini dello schermo
                    if (rana->item.x < 1)
                    {
                        rana->item.x = 1;
                    }
                    if (rana->item.x > NCOLS - LARGHEZZA_RANA - 1)
                    {
                        rana->item.x = NCOLS - LARGHEZZA_RANA - 1;
                    }
                }
                break;
            }
        }
    }

    // se non è su un coccodrillo e sta nel fiume la rana muore
    if (!isOnCoccodrillo &&
        rana->item.y > ALTEZZA_TANE + ALTEZZA_SPONDA &&
        rana->item.y < NLINES - ALTEZZA_MARCIAPIEDE)
    {
        handleMorteRana(rana, vite, punti, wgioco, audio);
        return true;
    }

    // check rana e proiettile
    for (int i = 0; i < N_PROIETTILI; i++) //scorro l'array dei proiettili
    {
        if (astuccio[i].tid < 0)
            continue; // se non è stato inizializzato va al prossimo slot
        BoundingBox proiettileBox = createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1); //creo box proiettile
        if (checksovrapposizione(ranaBox, proiettileBox)) //se prende la rana
        {
            handleMorteRana(rana, vite, punti, wgioco, audio); //la rana muore
            return true; //vanno resettati tempo e posizione rana
        }
    }

    // check granate e proiettili
    for (int i = 0; i < N_PROIETTILI; i++) //scorro i proiettili
    {
        if (astuccio[i].tid != -1) //se è attivo
        {
            BoundingBox proiettileBox = createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1); //creo la box proiettile
            for (int j = 0; j < N_GRANATE; j++) //scorro le granate
            {
                if (granate[j].tid != -1) //se è attiva
                {
                    BoundingBox granataBox = createBoundingBox(granate[j].item.x, granate[j].item.y, 1, 1); //creo la box granata
                    if (checksovrapposizione(granataBox, proiettileBox)) // se si sovrappongono
                    {
                        punti->proiettili += PUNTI_PROIETTILI; //aggiungo i punti
                        //termino i thread nel caso di collisione
                        if (astuccio[i].tid != -1)
                        {
                            pthread_cancel(astuccio[i].tid);
                            pthread_join(astuccio[i].tid, NULL);
                            astuccio[i].tid = -1;
                        }

                        if (granate[j].tid != -1)
                        {
                            pthread_cancel(granate[j].tid);
                            pthread_join(granate[j].tid, NULL);
                            granate[j].tid = -1;
                        }
                    }
                }
            }
        }
    }
    return false; //nessun reset
}

void handleHud(WINDOW *whud, int vite, Punteggio punti)
{
    werase(whud);
    box(whud, ACS_VLINE, ACS_HLINE);
    for (int i = 0; i < vite; i++)
    { // per ogni vita stampa una rana
        stampaRana((Oggetto){' ', 1, i * LARGHEZZA_RANA + 1, 0}, whud);
    }
    mvwprintw(whud, 1, 30, "punteggio: %d", totalePunti(punti));
    wnoutrefresh(whud);
}

void handleTempo(WINDOW *wtempo, int start)
{
    werase(wtempo);
    box(wtempo, ACS_VLINE, ACS_HLINE);
    int secondi = time(NULL) - start; //tempo passato dall'inizio della partita
    float ratio = (MAX_TEMPO - secondi) / (float)MAX_TEMPO; // quanto tempo è passato rispetto al massimo
    int limit = ratio * (NLINES - 1);                       //  indica quante righe occupo
    for (int i = limit; i > 0; i--)
    {
        //cambio colore in base a quanto tempo manca
        short color_pair;
        if (ratio > 0.7)
            color_pair = GREEN_TEMPO; // verde
        else if (ratio > 0.3)
            color_pair = YELLOW_TEMPO; // giallo
        else
            color_pair = RED_TEMPO; // rosso
        for (int j = 1; j < TCOLS - 1; j++) //Stampo la barra del tempo
        {
            wattron(wtempo, COLOR_PAIR(color_pair));
            mvwaddch(wtempo, NLINES - i, j, ' ' | A_REVERSE);
            wattroff(wtempo, COLOR_PAIR(color_pair));
        }
    }
    wnoutrefresh(wtempo);
}

void posizionaSparo(Thread p, Thread* array, int dim_array)
{
    _Bool found=false; //variabile per segnalare se ho trovato il posto corretto nell'array

    if(p.item.x<0 || p.item.x>NCOLS) //se lo sparo(proiettile o granata) è uscito dallo schermo
    {
        for(int i=0; i<dim_array; i++) //lo cerco nell'array
        {
            if(array[i].tid==p.tid) //una volta che lo trovo
            {
                if (array[i].tid != -1)
                    {          
                        if (pthread_kill(array[i].tid, 0) == 0) {
                            if (pthread_cancel(array[i].tid) == 0) {
                                pthread_join(array[i].tid, NULL);
                            }
                            else exit(ERRORE_CANCEL_SPARO);
                        }
                    array[i].tid = -1;
                    }
                array[i].tid=-1; //resetto la cella dell'array in modo da potermi memorizzare un nuovo sparo
                found=true; //segnalo di aver terminato per poter uscire dalla funzione
                break;
            }
        }
    }
    if(found) return NULL; //se ho cancellato correttamente lo sparo torno nel controllo

    for(int i=0; i<dim_array; i++) //se lo sparo non deve essere cancellato
    {
        if(array[i].tid==p.tid) //cerco la sua posizione nell'array per aggiornarlo
        {
            array[i]=p; //lo aggiorno
            found=true; //esco
            break;
        }
    }

    if(!found) //se lo sparo non è contenuto nell'array
    {
        for(int i=0; i<dim_array; i++) //cerco una cella vuota per inserirlo
        {
            if(array[i].tid==-1) //quando la trovo
            {
                array[i]=p; //lo inserisco
                found=true; //esco
                break;
            }
        }
    }

    if(!found) //se non trovo un posto in cui inserirlo
    {
        if(p.tid != -1)
        {
            if (pthread_kill(p.tid, 0) == 0) {
                if (pthread_cancel(p.tid) == 0) {
                    pthread_join(p.tid, NULL);
                }
                else exit(ERRORE_CANCEL_SPARO);
            }
        }
    }
}