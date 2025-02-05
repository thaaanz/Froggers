#include "controllo.h"

void *controllo(void *semafori)
{
    Semafori *s = (Semafori *)semafori;

    WINDOW *wgioco = newwin(NLINES, NCOLS, HUDLINES, 0); // finestra di gioco con coccodrilli e rana
    WINDOW *whud = newwin(HUDLINES, NCOLS, 0, 0);        // finestra che segna le vite e il tempo
    WINDOW *wtempo = newwin(NLINES - 1, TCOLS, HUDLINES, NCOLS); // colonna del tempo

    wbkgd(whud, COLOR_PAIR(COLORI_HUD));
    wbkgd(wtempo, COLOR_PAIR(COLORI_TEMPO));

    //audio
    GameAudio audio;
    inizializzaAudio(&audio);
    riproduciMusica(&audio);
    _Bool tane[NUMERO_TANE] = {0}; // essenzialmente se è false la tana è libera
    int vite = MAX_VITE;
    Punteggio punti = inizializzaPunteggio();

    while(vite > 0){
        // inizializzazioni
        Flusso fiume[NUMERO_FLUSSI];
        avviaFlussi(fiume);
        Thread rana = avviaRana(s);
        Thread cricca[NUMERO_FLUSSI * MAX_COCCODRILLI]; // cricca di coccodrilli
        Thread granate[N_GRANATE];
        Thread astuccio[N_PROIETTILI]; // astuccio dei proiettili
        Thread temp;

        avviaCoccodrilli(fiume, cricca, s);
        inizializzaGranate(granate);
        inizializzaProiettili(astuccio);    

        time_t start = time(NULL);
        _Bool reset = false; // la uso per capire se devo resettare posizione rana e tempo

        while (!reset)
        {
            reset = false;
            // gestione stampa
            werase(wgioco);
            handleTempo(wtempo, start);
            stampaFiume(wgioco);
            handleHud(whud, vite, punti);
            stampaTane(wgioco, tane);
            stampaMarciapiede(wgioco);
            stampaSponda(wgioco);
            stampaCoccodrilli(cricca, wgioco);
            stampaRana(rana.item, wgioco);

            for (int i = 0; i < N_GRANATE; i++)
            {
                if (granate[i].tid != -1)
                {
                    stampaGranata(granate[i].item, wgioco);
                }
            }

            for (int i = 0; i < N_PROIETTILI; i++)
            {
                if (astuccio[i].tid != -1)
                {
                    stampaProiettile(astuccio[i].item, wgioco);
                }
            }

            temp = leggi(s); // lettura dal buffer 

            switch (temp.item.id)
            {
            case 'r':
                riproduciSuono(audio.salto_rana);
                rana.tid=temp.tid;
                rana.item.x += temp.item.x;
                rana.item.y += temp.item.y;
                if (rana.item.x <= 0)
                    rana.item.x = 1;
                if (rana.item.y <= 0)
                    rana.item.y = 1;
                if (rana.item.x >= NCOLS - LARGHEZZA_RANA)
                    rana.item.x = NCOLS - LARGHEZZA_RANA - 1;
                if (rana.item.y >= NLINES - ALTEZZA_RANA)
                    rana.item.y = NLINES - ALTEZZA_RANA - 1;
                break;
            case 'g':
                for(int i=0; i < N_GRANATE; i++){
                    if(granate[i].tid == -1){
                        avviaGranate(rana, semafori);
                        break;
                    }
                }
                break;
            case 'o':
                if(temp.item.dir==0) riproduciSuono(audio.sparo_granata); //per fare il suono solo una volta
                _Bool trovate = false;
                for (int i = 0; i < N_GRANATE; i++)
                {
                    if (granate[i].tid == temp.tid)
                    {
                        granate[i] = temp;
                        trovate = true;

                        // Check se è fuori dai bordi
                        if (granate[i].item.x < 0 || granate[i].item.x > NCOLS)
                        {
                            if (granate[i].tid != -1)
                            {  // se è fuori dai bordi cancello il thread
                                pthread_cancel(granate[i].tid);
                                pthread_join(granate[i].tid, NULL);
                                granate[i].tid = -1;
                            }
                        }
                        break;
                    }
                }

                if (!trovate)
                {
                    for (int i = 0; i < N_GRANATE; i++)
                    {
                        if (granate[i].tid == -1)
                        { 
                            granate[i] = temp;
                            trovate = true;
                            break;
                        }
                    }
                }

                if (!trovate && temp.tid > 1)
                {
                    pthread_cancel(temp.tid);
                    pthread_join(temp.tid, NULL);
                }

                break;

            case 'c':                                                     // se legge un coccodrillo
                for (int i = 0; i < NUMERO_FLUSSI * MAX_COCCODRILLI; i++) // scorre l'array
                {
                    if (cricca[i].tid == temp.tid) // cerca il coccodrillo corrispondente
                    {
                        cricca[i] = temp; // aggiorna il coccodrillo
                    }
                }
                break;
            case '-':
                _Bool trovati = false;

                for (int i = 0; i < N_PROIETTILI; i++)
                {
                    if (astuccio[i].tid == temp.tid)
                    {
                        astuccio[i] = temp;
                        trovati = true;

                        // Check se è fuori dai bordi
                        if (astuccio[i].item.x < 0 || astuccio[i].item.x > NCOLS)
                        {
                            if (astuccio[i].tid != -1)
                            {
                                pthread_cancel(astuccio[i].tid);
                                pthread_join(astuccio[i].tid, NULL);
                                astuccio[i].tid = -1;
                            }
                        }
                        break;
                    }
                }

                if (!trovati)
                {
                    for (int i = 0; i < N_PROIETTILI; i++)
                    {
                        if (astuccio[i].tid == -1)
                        { 
                            astuccio[i] = temp;
                            trovati = true;
                            break;
                        }
                    }
                }

                if (!trovati && temp.tid != -1)
                {
                    pthread_cancel(temp.tid);
                    pthread_join(temp.tid, NULL);
                }

                break;
            }

            reset = detectCollisione(&rana, cricca, astuccio, granate, tane, temp, wgioco, &vite, time(NULL) - start, &punti, audio);

            if (reset)
            { // resetto la posizione della rana e il tempo
                rana.item.x = NCOLS / 2 - LARGHEZZA_RANA / 2;
                rana.item.y = NLINES - ALTEZZA_RANA - 1;
                start = time(NULL);
                for (int i = 0; i < N_PROIETTILI; i++)
                {
                    if (astuccio[i].tid != -1)
                    {
                        pthread_cancel(astuccio[i].tid);
                        pthread_join(astuccio[i].tid, NULL);
                    }
                }
                for (int i = 0; i < N_GRANATE; i++)
                {
                    if (granate[i].tid != -1)
                    {
                        pthread_cancel(granate[i].tid);
                        pthread_join(granate[i].tid, NULL);
                    }
                }
                usleep(DELAY_CONTROLLO);
                // inizializza di nuovo tutto
                inizializzaProiettili(astuccio);
                inizializzaGranate(granate);
            }


            wnoutrefresh(wgioco);
            doupdate();
            usleep(DELAY_CONTROLLO);
        }
        // fuori dal loop principale
        if (checkWin(tane))
        {
            break; // se vinci esci dal loop principale
        }
        cleanup(rana, cricca, astuccio, granate);
    }
    menuFinale(punti, vite);
    delwin(wgioco);
    delwin(whud);
    delwin(wtempo);
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
    return (a.x + a.width <= b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
}

_Bool checkWin(_Bool *tane)
{
    for (int i = 0; i < NUMERO_TANE; i++)
    {
        if (!tane[i])
            return false; // se ce n'è una libera non si ha ancora vinto la partita
    }
    return true;
}

int checkTaneCollision(int x, int y, GameAudio audio)
{
    for (int i = 0; i < NUMERO_TANE; i++)
    { // per ogni tana
        int taneX = 8 + i * (6 + 7);
        if (x >= taneX && x + LARGHEZZA_RANA < taneX + 6 && y < ALTEZZA_TANE)
        { // check se sta dentro la tana
            riproduciSuono(audio.chiusura_tana);
            return i;
        }
    }
    return -1;
}

void handleMorteRana(Thread *rana, int *vite, Punteggio *punti, WINDOW *wgioco, GameAudio audio)
{
    riproduciSuono(audio.morte_rana);
    (*vite)--;
    punti->morte += PUNTI_MORTE;

    werase(wgioco);
    //! potremmo fare un figlet con manche over? o si incasina con la temporizzazione?
    mvwprintw(wgioco, NLINES / 2, NCOLS / 2 - 5, "Vita persa");
    wnoutrefresh(wgioco);
    doupdate();
}

// funzione principale per le collisioni
_Bool detectCollisione(Thread *rana, Thread *cricca, Thread *astuccio, Thread *granate, _Bool *tane, Thread last, WINDOW *wgioco, int *vite, int secondi, Punteggio *punti, GameAudio audio)
{
    checkPuntiSalto(punti, last);
    // crea la box rana
    BoundingBox ranaBox = createBoundingBox(rana->item.x, rana->item.y, LARGHEZZA_RANA, ALTEZZA_RANA);

    // Check tempo
    if (secondi > MAX_TEMPO)
    {
        handleMorteRana(rana, vite, punti, wgioco, audio);
        return true;
    }

    // Check delle varie zone
    if (rana->item.y > NLINES - ALTEZZA_MARCIAPIEDE)
    {
        return false; // sta nel marciapiede quindi safe
    }

    // Check tane
    if (rana->item.y < ALTEZZA_TANE)
    {
        int taneIndex = checkTaneCollision(rana->item.x, rana->item.y, audio);
        if (taneIndex >= 0)
        {
            if (!tane[taneIndex])
            {                              // se la tana è libera
                tane[taneIndex] = 1;       // la occupa
                punti->tane += PUNTI_TANA; // assegna i punti
                punti->tempo += (MAX_TEMPO - secondi) * PUNTI_PER_SEC;
                (*vite) = MAX_VITE;
                return true; // vanno resettati tempo e posizione rana
            }
            else
            { // se la tana è occupata
                handleMorteRana(rana, vite, punti, wgioco, audio);
                return true;
            }
        }
        else
        { // miss della tana
            handleMorteRana(rana, vite, punti, wgioco, audio);
            return true;
        }
    }

    // check sponda
    if (rana->item.y > ALTEZZA_TANE &&
        rana->item.y < ALTEZZA_TANE + ALTEZZA_SPONDA)
    {
        return false; // safe
    }

    // Check collisione con coccodrillo
    _Bool isOnCoccodrillo = false;
    for (int i = 0; i < NUMERO_FLUSSI * MAX_COCCODRILLI; i++)
    {
        if (cricca[i].tid != -1)
        { // prima controlla se è attivo
            BoundingBox coccoBox = createBoundingBox(cricca[i].item.x, cricca[i].item.y, LARGHEZZA_COCCODRILLO, ALTEZZA_COCCODRILLO);

            if (checksovrapposizione(ranaBox, coccoBox))
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

    // se non è su un coccodrillo e sta nel fiume morte assicurata
    if (!isOnCoccodrillo &&
        rana->item.y > ALTEZZA_TANE + ALTEZZA_SPONDA &&
        rana->item.y < NLINES - ALTEZZA_MARCIAPIEDE)
    {
        handleMorteRana(rana, vite, punti, wgioco, audio);
        return true;
    }

    // check rana e proiettile
    for (int i = 0; i < N_PROIETTILI; i++)
    {
        if (astuccio[i].tid < 0)
            continue; // se non è stato inizializzato va al prossimo slot
        BoundingBox proiettileBox = createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1);
        if (checksovrapposizione(ranaBox, proiettileBox))
        {
            handleMorteRana(rana, vite, punti, wgioco, audio);
            return true;
        }
    }

    // check granate e proiettili
    for (int i = 0; i < N_PROIETTILI; i++)
    {
        if (astuccio[i].tid != -1)
        {
            BoundingBox proiettileBox = createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1);
            for (int j = 0; j < N_GRANATE; j++)
            {
                if (granate[j].tid != -1)
                {
                    BoundingBox granataBox = createBoundingBox(granate[j].item.x, granate[j].item.y, 1, 1);
                    if (checksovrapposizione(granataBox, proiettileBox))
                    {
                        punti->proiettili += PUNTI_PROIETTILI;

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
    return false;
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
    int secondi = time(NULL) - start;
    float ratio = (MAX_TEMPO - secondi) / (float)MAX_TEMPO; // quanto tempo è passato rispetto al massimo
    int limit = ratio * (NLINES - 1);                       //  indica quante righe occupo
    for (int i = limit; i > 0; i--)
    {
        short color_pair;
        // todo qua i codici del color pair sono messi a caso era solo per fare una prova (che funziona) DA CAMBIARE
        if (ratio > 0.7)
            color_pair = GREEN_TEMPO; // verde
        else if (ratio > 0.3)
            color_pair = YELLOW_TEMPO; // giallo
        else
            color_pair = RED_TEMPO; // rosso
        for (int j = 1; j < TCOLS - 1; j++)
        {
            wattron(wtempo, COLOR_PAIR(color_pair));
            mvwaddch(wtempo, NLINES - i, j, ' ' | A_REVERSE);
            wattroff(wtempo, COLOR_PAIR(color_pair));
        }
    }
    wnoutrefresh(wtempo);
}