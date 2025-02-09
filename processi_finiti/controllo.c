#include "controllo.h"

void controllo(GameAudio audio)
{
    WINDOW* wgioco=newwin(NLINES, NCOLS, HUDLINES, 0); //finestra di gioco con coccodrilli e rana
    WINDOW* whud=newwin(HUDLINES, NCOLS, 0, 0); //finestra che segna le vite e il punteggio
    WINDOW* wtempo=newwin(NLINES -1, TCOLS, HUDLINES, NCOLS ); // colonna del tempo

    wbkgd(whud, COLOR_PAIR(COLORI_HUD)); //imposto il colore di bg delle finestre
    wbkgd(wtempo, COLOR_PAIR(COLORI_TEMPO));

    _Bool tane[NUMERO_TANE]={0}; //se il valore è 0 la tana è libera
    int vite=MAX_VITE; //inizializzo le vite
    Punteggio punteggio=inizializzaPunteggio(); //inizializzo il punteggio

    riproduciMusica(audio.musica_gioco); //faccio partire la musica di sottofondo

    while(vite>0) //mentre ho ancora vite
    {
        stampaNewManche();

        int pipe_fd[2]; //pipe principale per le comunicazioni al controllo
        int pipe_inversa[2]; //pipe inversa per le comunicazioni dal controllo
        avviaPipe(pipe_fd); //inizializzo le pipe
        avviaPipe(pipe_inversa);

        //inizializzazioni
        Flusso* fiume=avviaFlussi(); //crea i flussi del fiume
        Processo rana=avviaRana(pipe_fd, pipe_inversa); //crea la rana
        Processo cricca[NUMERO_FLUSSI*MAX_COCCODRILLI_PER_FLUSSO];//cricca di coccodrilli, crea i coccodrilli
        Processo granate[N_GRANATE]; //creo le granate
        Processo astuccio[N_PROIETTILI]; //astuccio dei proiettili, creo i proiettili
        Processo temp; //Processo temporaneo per la lettura dalla pipe
        
        Oggetto mine[N_MINE]; //creo le mine (ostacoli in più nella sponda)
        inizializzaMine(mine); //inizializzo le mine

        avviaCoccodrilli(pipe_fd, fiume, cricca); //avvia i processi coccodrillo
        inizializzaProiettili(astuccio); // inizializza l'array dei proiettili
        inizializzaGranate(granate); //inizializza l'array delle granate

        close(pipe_fd[1]); //chiudo la pipe principale in scrittura
        close(pipe_inversa[0]); //chiudo la pipe secondaria in lettura

        time_t start=time(NULL); //inizializzo il tempo di partenza
        
        _Bool reset=false; //la uso per capire se devo resettare la posizione della rana e il tempo
        
        while(!reset)
        {
            reset=false;

            //gestione stampa
            werase(wgioco); 
            werase(whud);
            werase(wtempo);

            handleHud(whud, vite, punteggio); //gestisco le vite e il punteggio
            handleTempo(wtempo, start); //gestisco il tempo

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

            read(pipe_fd[0], &temp, sizeof(Processo)); //leggo dalla pipe principale

            switch(temp.item.id) //controllo che elemento ho letto attraverso l'id
            {
                case 'r': // caso rana
                    riproduciSuono(audio.salto_rana);
                    //aggiorno la posizione
                    rana.item.x+=temp.item.x; 
                    rana.item.y+=temp.item.y;
                    //mi assicuro che la rana resti allinterno dei limiti della finestra di gioco
                    if(rana.item.x<=0) rana.item.x=1;
                    if(rana.item.y<=0) rana.item.y=1;
                    if(rana.item.x>=NCOLS-LARGHEZZA_RANA) rana.item.x=NCOLS-LARGHEZZA_RANA-1;
                    if(rana.item.y>=NLINES-ALTEZZA_RANA) rana.item.y=NLINES-ALTEZZA_RANA-1;
                    break;
                case 'q':
                    cleanup; freeAudio(&audio); endwin(); return 0;
                case 'g': //caso granata(quando la rana spara)
                    for(int i=0; i<N_GRANATE; i++) //cerco uno spazio libero nell'array
                    {
                        if(granate[i].pid == -1) //quando lo trovo
                        {
                            // scrivo nella pipe inversa le coordinate della rana in modo che queste vengano passate alle granate
                            write(pipe_inversa[1], &rana, sizeof(Processo));
                            break;
                        }
                    }
                    break;
                case 'o': // caso granata(aggiornamento posizione)
                    if(temp.item.dir==0) riproduciSuono(audio.sparo_granata);
                    //l'attributo direzione restava inutilizzato quindi lo uso come controllo per fare in modo che il suono venga riprodotto solo una volta per sparo e non ogni volta che la pipe legge una granata
                    posizionaSparo(temp, granate, N_GRANATE); //inserisco la granata nell'array per aggiornare la sua posizione
                    break;
                case 'c': // caso coccodrillo
                    for(int i=0; i<NUMERO_FLUSSI*MAX_COCCODRILLI_PER_FLUSSO; i++) //cerco il coccodrillo nell'array
                    {
                        if(cricca[i].pid==temp.pid) //quando lo trovo
                        {
                            cricca[i]=temp; //aggiorno la sua posizione
                        }
                    }
                    break;
                case '-': // caso proiettile
                    posizionaSparo(temp, astuccio, N_PROIETTILI); //inserisco il proiettile nell'array per aggiornare la sua posizione
                    break;
            }

            reset=detectCollisione(&rana, cricca, astuccio, granate, mine, tane, temp, wgioco, &vite, time(NULL)-start, &punteggio, audio); 

            if(reset)
            { //resetto la posizione della rana e il tempo
                rana.item.x=NCOLS / 2 - LARGHEZZA_RANA / 2;
                rana.item.y=NLINES - ALTEZZA_RANA - 1;
                start=time(NULL);
                //termino i processi dei proiettili e delle granate
                for(int i=0; i < N_PROIETTILI; i++)
                { 
                    if(astuccio[i].pid > 1)
                    {
                        if(kill(astuccio[i].pid, 9)==-1)
                        {
                            exit(ERRORE_KILL_PROIETTILI);
                        }
                        waitpid(astuccio[i].pid, NULL, 0);
                    }
                }

                for(int i=0; i < N_GRANATE; i++)
                {
                    if(granate[i].pid > 1)
                    {
                        if(kill(granate[i].pid, 9)==-1)
                        {
                            exit(ERRORE_KILL_GRANATE);
                        }
                        waitpid(granate[i].pid, NULL, 0);
                    }
                }
                //inizializzo di nuovo tutto 
                inizializzaProiettili(astuccio); 
                inizializzaGranate(granate);
                inizializzaMine(mine);
            }

            usleep(DELAY_CONTROLLO);

            if(checkWin(tane))
            {
            break; // se vinci esci dal loop principale
            }
        }

        cleanup(rana, cricca, astuccio, granate);
        usleep(DELAY_CONTROLLO);

    }
    
    //fuori dal loop principale
    menuFinale(punteggio, vite, &audio);
    delwin(wgioco);
    delwin(whud);
    delwin(wtempo);
}

BoundingBox createBoundingBox(int x, int y, int larghezza, int altezza) 
{
    BoundingBox box={
        .x = x,
        .y = y,
        .width = larghezza,
        .height = altezza
    };
    return box;
}

_Bool checksovrapposizione(BoundingBox a, BoundingBox b)
{
    return (a.x + a.width <= b.x + b.width && // l'estremo destro di a è a sinistra dell'estremo destro di b
            a.x + a.width > b.x && // l'estremo destro di a è a destra dell'estremo sinistro di b
            a.y < b.y + b.height && // a è più in basso dell'estremo superiore di b
            a.y + a.height > b.y); // a è più in alto dell'estremo inferiore di b
}

_Bool checkWin(_Bool* tane)
{
    for(int i=0; i < NUMERO_TANE; i ++) //scorro le tane
    {
        if(!tane[i]) return false; //se ce n'è una libera non si ha ancora vinto la partita
    }
    return true; //se le tane sono tutte chiuse la partita è vinta
}

int checkTaneCollision(int x, int y, GameAudio audio) 
{
    for(int i = 0; i < NUMERO_TANE; i++) //per ogni tana
    { 
        int taneX = 8 + i * (6 + 7);  //calcolo la x di partenza della tana
        if(x >= taneX && x + LARGHEZZA_RANA < taneX + 6 && y < ALTEZZA_TANE) //controllo se la rana sta dentro la tana
        {
            riproduciSuono(audio.chiusura_tana); //riproduco il suono della chiusura della tana
            return i; //restituisco il numero della tana chiusa
        }
    }
    return -1;
}

void handleMorteRana(Processo* rana, int* vite, Punteggio* punti, WINDOW* wgioco, GameAudio audio) 
{
    riproduciSuono(audio.morte_rana); 
    (*vite)--; //decremento le vite
    punti->morte += PUNTI_MORTE; //assegno i punti
    
    if(*vite==0) sleep(1); //se perdo, aspetto un secondo
}

// funzione principale per le collisioni
_Bool detectCollisione(Processo* rana, Processo* cricca, Processo* astuccio, Processo* granate, Oggetto* mine, _Bool* tane, Processo last, WINDOW* wgioco, int* vite, int secondi, Punteggio* punti, GameAudio audio) 
{
    checkPuntiSalto(punti, last); //assegno i punti per il salto avanti/indietro
    
    BoundingBox ranaBox = createBoundingBox(rana->item.x, rana->item.y, LARGHEZZA_RANA,ALTEZZA_RANA); // crea la box della rana

    if(secondi > MAX_TEMPO) //se scade il tempo
    {
        handleMorteRana(rana, vite, punti, wgioco, audio); //la rana muore
        return true; //reset
    }

    // Check delle varie zone
    if(rana->item.y > NLINES - ALTEZZA_MARCIAPIEDE) //se la rana è sul marciapiede
    {  
        return false; //no reset, è al sicuro
    }

    // Check tane 
    if(rana->item.y < ALTEZZA_TANE) //se la rana è nella zona delle tane
    {
        int taneIndex = checkTaneCollision(rana->item.x, rana->item.y, audio); //controllo se sta dentro una tana
        if(taneIndex >= 0) 
        {
            if(!tane[taneIndex]) // se la tana è libera
            { 
                tane[taneIndex] = 1; // la occupa
                punti->tane += PUNTI_TANA; //assegna i punti
                punti->tempo+= ( MAX_TEMPO - secondi)*PUNTI_PER_SEC;
                (*vite)=MAX_VITE; //resetta le vite
                return true; //reset
            }
            else // se la tana è occupata
            { 
                handleMorteRana(rana, vite, punti, wgioco, audio); //la rana muore
                return true; //reset
            }
        }
        else //se salta e non centra la tana
        { 
            handleMorteRana(rana, vite,punti,  wgioco, audio); //la rana muore
            return true; //reset
        }
    }

    // check sponda
    if(rana->item.y > ALTEZZA_TANE && rana->item.y < ALTEZZA_TANE + ALTEZZA_SPONDA) //se la rana è sulla sponda
    {
        for(int i=0; i < N_MINE; i++) //scorro le mine
        {
            BoundingBox mineBox = {mine[i].x, mine[i].y, 1, 1}; //creo le box delle mine
            if(checksovrapposizione(mineBox, ranaBox)) //se la rana si scontra con la mina
            {
                handleMorteRana(rana, vite, punti, wgioco, audio); //la rana muore
                return true; //reset
            }
        }
        return false; //safe 
    }

    // Check collisione con coccodrillo
    _Bool isOnCoccodrillo = false; //flag per controllare se la rana è sul coccodrillo
    for(int i = 0; i < NUMERO_FLUSSI * MAX_COCCODRILLI_PER_FLUSSO; i++) //scorro i coccodrilli
    {
        if(cricca[i].pid != 0) //controllo se il coccodrillo è attivo
        {  
            BoundingBox coccoBox = createBoundingBox(cricca[i].item.x, cricca[i].item.y, LARGHEZZA_COCCODRILLO, ALTEZZA_COCCODRILLO); //creo le box dei coccodrilli

            if(checksovrapposizione(ranaBox, coccoBox)) //se la rana è sul coccodrillo
            {
                isOnCoccodrillo = true; //set della flag
                
                if(cricca[i].pid == last.pid) //quando il coccodrillo si muove
                {
                    rana->item.x += last.item.dir; //si muove anche la rana
                    
                    // controllo i limiti dello schermo
                    if(rana->item.x < 1) 
                    {
                        rana->item.x = 1;
                    }
                    if(rana->item.x > NCOLS - LARGHEZZA_RANA - 1) 
                    {
                        rana->item.x = NCOLS - LARGHEZZA_RANA - 1;
                    }
                }
                break;
            }
        }
    }

    // se la rana non è su un coccodrillo e sta nel fiume
    if(!isOnCoccodrillo && rana->item.y > ALTEZZA_TANE + ALTEZZA_SPONDA && rana->item.y < NLINES - ALTEZZA_MARCIAPIEDE) 
    {
        handleMorteRana(rana, vite,punti, wgioco, audio); //la rana muore
        return true; //reset
    }
    
    //check rana e proiettile
    for(int i=0; i < N_PROIETTILI; i++)
    {
        if(astuccio[i].pid < 0) continue; //se la cella non è valida si scorre alla prossima
        
        BoundingBox proiettileBox=createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1); //creo le box dei proiettili
        if(checksovrapposizione(ranaBox, proiettileBox)) //se il proiettile colpisce la rana
        {
            handleMorteRana(rana, vite, punti, wgioco, audio); //la rana muore
            return true; //reset
        }
    }

    // check granate e proiettili
    for(int i=0; i < N_PROIETTILI; i++) //scorro i proiettili
    {
        if(astuccio[i].pid >1) //se il proiettile è attivo
        {
            BoundingBox proiettileBox=createBoundingBox(astuccio[i].item.x, astuccio[i].item.y, 1, 1); //creo le box dei proiettili
            
            for(int j=0; j< N_GRANATE; j++) //scorro le granate
            {
                if(granate[j].pid >1) //se la granata è attiva
                {
                    BoundingBox granataBox=createBoundingBox(granate[j].item.x, granate[j].item.y, 1, 1); //creo le box delle granate
                    if(checksovrapposizione(granataBox, proiettileBox)) //se la granata e il proiettile si scontrano
                    {
                        punti->proiettili+=PUNTI_PROIETTILI; //assegno i punti
                        //faccio la kill del proiettile e della granata scontrati
                        if(astuccio[i].pid > 1)
                        {
                            if(kill(astuccio[i].pid, 9)==-1)
                            {
                                exit(ERRORE_KILL_PROIETTILI);
                            }
                            astuccio[i].pid=-1;
                        }
                        
                        if(granate[j].pid >1)
                        {
                            if(kill(granate[j].pid, 9)==-1)
                            {
                                exit(ERRORE_KILL_GRANATE);
                            }
                            granate[j].pid=-1;
                        }
                    }
                } 
            }
        }    
    }
    return false;
}

void handleHud(WINDOW* whud, int vite, Punteggio punti)
{
    box(whud, ACS_VLINE, ACS_HLINE);

    for(int i=0; i < vite; i++) //per ogni vita 
    { 
        stampaRana((Oggetto){' ', 1, i* LARGHEZZA_RANA + 1, 0}, whud); //stampo una rana
    }

    int totale=totalePunti(punti); //calcolo i punti
    mvwprintw(whud, 1, 30, "punteggio: %d", totale );
}

void handleTempo(WINDOW* wtempo, int start)
{
    int secondi=time(NULL)-start; //tempo trascorso dall'inizio della partita
    float ratio=(MAX_TEMPO-secondi) / (float)MAX_TEMPO; //quanto tempo è passato rispetto al massimo
    int limit= ratio * (NLINES-1); //indica quante righe occupo
    for(int i=limit; i > 0 ; i--)
    {
        //cambio colore in base a quanto tempo manca
        short color_pair;
        if(ratio > 0.3) color_pair = GREEN_TEMPO;
        //else if(ratio > 0.3) color_pair = YELLOW_TEMPO;
        else color_pair = RED_TEMPO;

        for(int j=1; j < TCOLS-1; j++)
        {
            wattron(wtempo, COLOR_PAIR(color_pair));
            mvwaddch(wtempo, NLINES-i, j, ' ' | A_REVERSE); //stampo la barra del tempo
            wattroff(wtempo, COLOR_PAIR(color_pair));
        }     
    }
}

void posizionaSparo(Processo p, Processo* array, int dim_array)
{
    _Bool found=false; //variabile per segnalare se ho trovato il posto corretto nell'array

    if(p.item.x<0 || p.item.x>NCOLS) //se lo sparo(proiettile o granata) è uscito dallo schermo
    {
        for(int i=0; i<dim_array; i++) //lo cerco nell'array
        {
            if(array[i].pid==p.pid) //una volta che lo trovo
            {
                if(p.pid>1)
                {
                    if(kill(array[i].pid, 9)==-1) //lo cancello
                    {
                        exit(ERRORE_KILL_SPARO);
                    }
                }
                array[i].pid=-1; //resetto la cella dell'array in modo da potermi memorizzare un nuovo sparo
                found=true; //segnalo di aver terminato per poter uscire dalla funzione
                break;
            }
        }
    }
    if(found) return NULL; //se ho cancellato correttamente lo sparo torno nel controllo

    for(int i=0; i<dim_array; i++) //se lo sparo non deve essere cancellato
    {
        if(array[i].pid==p.pid) //cerco la sua posizione nell'array per aggiornarlo
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
            if(array[i].pid<0) //quando la trovo
            {
                array[i]=p; //lo inserisco
                found=true; //esco
                break;
            }
        }
    }

    if(!found) //se non trovo un posto in cui inserirlo
    {
        if(p.pid>1)
        {
            if(kill(p.pid, 9)==-1) //lo cancello perchè l'array è pieno
            {
                exit(ERRORE_KILL_SPARO);
            }
        }
    }
}