#include "menu.h"

const char spriteMenu[ALTEZZA_MENU][LARGHEZZA_MENU]={
    " _ __ ___   ___ _ __  _   _ ",
    "| '_ ` _ \\ / _ \\ '_ \\| | | |",
    "| | | | | |  __/ | | | |_| |",
    "|_| |_| |_|\\___|_| |_|\\__,_|"
};

const char spriteGameOver[ALTEZZA_GO][LARGHEZZA_GO]={
    "  __ _  __ _ _ __ ___   ___    _____   _____ _ __ ",
    " / _` |/ _` | '_ ` _ \\ / _ \\  / _ \\ \\ / / _ \\ '__|",
    "| (_| | (_| | | | | | |  __/ | (_) \\ V /  __/ |  ", 
    " \\__, |\\__,_|_| |_| |_|\\___|  \\___/ \\_/ \\___|_|   ",
    " |___/  "
};
const char spriteWin[ALTEZZA_WIN][LARGHEZZA_WIN]={
    " _   _  ___  _   _  __      _____  _ __ | |",
    "| | | |/ _ \\| | | | \\ \\ /\\ / / _ \\| '_ \\| |",
    "| |_| | (_) | |_| |  \\ V  V / (_) | | | |_|",
    " \\__, |\\___/ \\__,_|   \\_/\\_/ \\___/|_| |_(_)",
    "|___/   "
};
const char newMancheSprite[ALTEZZA_NEW][LARGHEZZA_NEW]={
    "                                                  _          ",
    " _ __   _____      __  _ __ ___   __ _ _ __   ___| |__   ___ ",
    "| '_ \\ / _ \\ \\ /\\ / / | '_ ` _ \\ / _` | '_ \\ / __| '_ \\ / _ \\",
    "| | | |  __/\\ V  V /  | | | | | | (_| | | | | (__| | | |  __/",
    "|_| |_|\\___| \\_/\\_/   |_| |_| |_|\\__,_|_| |_|\\___|_| |_|\\___|"
};

const char spriteFrogger[H_FROGGER][W_FROGGER]={
"              _____ ____   ___   ____  ____ _____ ____",
"             |  ___|  _ \\ / _ \\ / ___|/ ___| ____|  _ \\",
"             | |_  | |_) | | | | |  _| |  _|  _| | |_) |",
"             |  _| |  _ <| |_| | |_| | |_| | |___|  _ <",
"             |_|   |_|  \\_\\___/ \\____|\\____|_____|_| \\_\\",
"",
" ____  _____ ____  _   _ ____  ____  _____ ____ _____ ___ ___  _   _",
"|  _ \\| ____/ ___|| | | |  _ \\|  _ \\| ____/ ___|_   _|_ _/ _ \\| \\ | |", 
"| |_) |  _| \\___ \\| | | | |_) | |_) |  _|| |     | |  | | | | |  \\| |",
"|  _ <| |___ ___) | |_| |  _ <|  _ <| |__| |___  | |  | | |_| | |\\  |",
"|_| \\_\\_____|____/ \\___/|_| \\_\\_| \\_\\_____\\____| |_| |___\\___/|_| \\_|"
};

_Bool utentePrivilegiato=false; //variabile globale

int menuIniziale(GameAudio* audio)
{
    WINDOW* wmenu=newwin(NLINES+HUDLINES, NCOLS, 0, 0);
    keypad(wmenu, TRUE); 
    int i=0; //corrisponde alla freccetta per selezionare l'opzione
    
    while(true)
    {
        _Bool flag=true; //flag per la gestione del loop 
        
        while(flag)
        {
            wclear(wmenu);
            box(wmenu, '#', '#');
            
            wattron(wmenu, COLOR_PAIR(COLORI_HUD));
            for(int j =0 ; j < ALTEZZA_MENU; j++)
            { 
                mvwprintw(wmenu, 2+j, NCOLS/2-LARGHEZZA_MENU/2, spriteMenu[j]); //stampo la sprite del menù
            }
            wattroff(wmenu, COLOR_PAIR(COLORI_HUD));

            //staampo le opzioni
            mvwprintw(wmenu, 10, NCOLS/2-LARGHEZZA_MENU/2, "Regolamento");
            mvwprintw(wmenu, 13, NCOLS/2-LARGHEZZA_MENU/2, "Selezione giocatore");
            mvwprintw(wmenu, 16, NCOLS/2-LARGHEZZA_MENU/2, "Esci");

            wattron(wmenu, COLOR_PAIR(COLORI_HUD));
            mvwprintw(wmenu, 10+i*3, (NCOLS/2-LARGHEZZA_MENU/2)-3, "-->"); //la freccia si sposta da un'opzione all'altra
            wattroff(wmenu, COLOR_PAIR(COLORI_HUD));

            wrefresh(wmenu);

            int c=(int)wgetch(wmenu);
            switch(c) //check sulla selezione
            {
                case KEY_UP: 
                    riproduciSuono(audio->freccia_menu);
                    i--;
                    if(i<0) i=2; //se sono sulla prima opzione e premo freccia su mi trovo sull'ultima opzione
                    break;
                case KEY_DOWN:
                    riproduciSuono(audio->freccia_menu);
                    i++;
                    break;
                case 10: //tasto invio
                    riproduciSuono(audio->freccia_menu);
                    flag=false; //esco dal loop
                    break;
            }
            i=i%3; //l'indice della freccia modulo tre per non farla scendere più in basso dell'ultima opzione

            usleep(2000);
        }
        //se si esce dal ciclo è stato premuto invio
        
        switch(i) //controllo che indice è stato selezionato
        {
            case 0:
                stampaRegolamento(wmenu);
                break;
            case 1:
                selezionaUtente(wmenu, audio);
                return 0; //gestito dal main
            case 2: //esci
                endwin();
                return 1; //gestito dal main
                break;
        }
    }
    
}

void stampaRegolamento(WINDOW* wmenu)
{
    wclear(wmenu);
    box(wmenu, '#', '#');
    mvwprintw(wmenu, 1, NCOLS/2 - 6 , "REGOLAMENTO:");

    mvwprintw(wmenu, 3, 3 , "1. L'obiettivo del gioco è chiudere tutte le tane saltando da un ");
    mvwprintw(wmenu, 4, 3 , "coccodrillo all'altro (frecce) senza cadere in acqua ");

    mvwprintw(wmenu, 6, 3 , "2. I coccodrilli sparano proiettili che possono colpire la rana ");
    mvwprintw(wmenu, 7, 3 , "facendole perdere una vita");

    mvwprintw(wmenu, 9, 3 , "3. Per evitare i proiettili la rana può schivarli oppure sparare ");
    mvwprintw(wmenu, 10, 3 , "delle granate (tasto spazio)");

    mvwprintw(wmenu, 12, 3 , "4. Il gioco è composto da più manche che si concludono quando la ");
    mvwprintw(wmenu, 13, 3 , "rana chiude una tana o perde una vita o allo scadere del tempo");

    mvwprintw(wmenu, 15, 3 , "5. La rana è al sicuro quando si trova sul marciapiede(parte ");
    mvwprintw(wmenu, 16, 3 , "inferiore dello schermo) o sulla sponda(parte sottostante alle tane) ");

    mvwprintw(wmenu, 18, 3 , "6. Il giocatore deve accumulare il massimo di punti possibile");

    mvwprintw(wmenu, 25, 6 , "Premere q per tornare al menu' principale");

    wrefresh(wmenu);

    char c;
    while(c!= 'q') //q per uscire
    {
        c=wgetch(wmenu);
    }
}

void selezionaUtente(WINDOW* wmenu, GameAudio* audio)
{
    _Bool flag=true; //flag per la gestione del loop
    int i=0; //indice della freccia

    while(flag)
    {
        wclear(wmenu);
        box(wmenu, '#', '#');

        mvwprintw(wmenu, 10, NCOLS/2-LARGHEZZA_MENU/2, "Banale utente");
        mvwprintw(wmenu, 13, NCOLS/2-LARGHEZZA_MENU/2, "Professore di SOPR");
        
        wattron(wmenu, COLOR_PAIR(COLORI_HUD));
        mvwprintw(wmenu, 10+i*3, (NCOLS/2-LARGHEZZA_MENU/2)-3, "-->"); //stampo la freccia
        wattroff(wmenu, COLOR_PAIR(COLORI_HUD));

        wrefresh(wmenu);

        int c=(int)wgetch(wmenu);
        switch(c)
        {
            case KEY_UP:
                riproduciSuono(audio->freccia_menu);
                i--;
                if(i<0) i=1;
                break;
            case KEY_DOWN:
                riproduciSuono(audio->freccia_menu);
                i++;
                break;
            case 10:
                riproduciSuono(audio->freccia_menu);
                flag=false;
                break;
        }
        i=i%2;
    }

    if(i==1) //check sull'utente selezionato
    {
        utentePrivilegiato=true;
    }
}

void menuFinale(Punteggio punti, int vite, GameAudio* audio)
{
    riproduciMusica(audio->musica_menu);
    clear();
    refresh();
    WINDOW* wmenu=newwin(NLINES+HUDLINES, NCOLS, 0, 0);

    if(vite > 0)
    {
        stampaWin(wmenu); //se si vince
    }
    else
    {
        stampaGO(wmenu); //se si perde
    }

    sleep(3);

    //riepilogo punti
    wclear(wmenu);
    box(wmenu, '#', '#');
    mvwprintw(wmenu, 3, 4, "PUNTI TOTALI: %d  (di  cui:)", totalePunti(punti)); 
    wrefresh(wmenu);
    usleep(350000);
    mvwprintw(wmenu, 5, 4, "--> tane: %d", punti.tane);
    wrefresh(wmenu);
    usleep(350000);
    if(utentePrivilegiato==true) 
    {
        punti.salti-=1000;
    }
    mvwprintw(wmenu, 7, 4, "--> salti: %d", punti.salti);
    wrefresh(wmenu);
    usleep(350000);
    mvwprintw(wmenu, 9, 4, "--> distruzione proiettili: %d", punti.proiettili);
    wrefresh(wmenu);
    usleep(350000);
    mvwprintw(wmenu, 11, 4, "--> tempo: %d", punti.tempo);
    wrefresh(wmenu);
    usleep(350000);
    mvwprintw(wmenu, 13, 4, "--> morte: -%d", punti.morte);
    wrefresh(wmenu);
    usleep(350000);
    if(utentePrivilegiato==true)
    {
        mvwprintw(wmenu, 15, 4, "--> BONUS!: 1000");
        wrefresh(wmenu);
        usleep(350000);
    }
    wattron(wmenu, COLOR_PAIR(COLORI_HUD));
    mvwprintw(wmenu, 17, 4, "Premi un tasto per continuare");
    wattroff(wmenu, COLOR_PAIR(COLORI_HUD));
    wrefresh(wmenu);
    wgetch(wmenu);
}

void stampaWin(WINDOW* w)
{
    wclear(w);
    for(int i=0; i < ALTEZZA_WIN; i++)
    {
        mvwprintw(w, 2+i, NCOLS/2-LARGHEZZA_WIN/2, spriteWin[i]); //stampo la sprite in caso di vittoria
    }
    wrefresh(w);
}

void stampaGO(WINDOW* w)
{
    wclear(w);
    for(int i=0; i < ALTEZZA_GO; i++)
    {
        mvwprintw(w, 10+i, NCOLS/2-LARGHEZZA_GO/2, spriteGameOver[i]); //stampo la sprite in caso di game over
    }
    wrefresh(w);
}

_Bool restart(GameAudio* audio)
{
    WINDOW* wmenu=newwin(NLINES+HUDLINES, NCOLS, 0, 0);
    keypad(wmenu, TRUE); 

    int i=0; //freccia
    _Bool flag=true; //gestione loop

    while(flag)
    {
        wclear(wmenu);
        box(wmenu, '#', '#');
        mvwprintw(wmenu, 7, NCOLS/2-LARGHEZZA_MENU/2, "Vuoi rigiocare?");
        mvwprintw(wmenu, 10, NCOLS/2-LARGHEZZA_MENU/2, "Sì");
        mvwprintw(wmenu, 13, NCOLS/2-LARGHEZZA_MENU/2, "No");
        wattron(wmenu, COLOR_PAIR(COLORI_HUD));
        mvwprintw(wmenu, 10+i*3, (NCOLS/2-LARGHEZZA_MENU/2)-3, "-->");
        wattroff(wmenu, COLOR_PAIR(COLORI_HUD));

        wrefresh(wmenu);
        int c=(int)wgetch(wmenu);
        
        switch(c)
        {
            case KEY_UP:
                riproduciSuono(audio->freccia_menu);
                i--;
                if(i<0) i=1;
                break;
            case KEY_DOWN:
                riproduciSuono(audio->freccia_menu);
                i++;
                break;
            case '\n':
            case KEY_ENTER:
                flag=false;
                break;
        }
        i=i%2;
    }
    delwin(wmenu);

    if(i==0) //restart nuova partita
    { 
        return true;
    }
    else return false;
}

void stampaNewManche()
{
    clear();
    attron(COLOR_PAIR(COLORI_HUD));
    for(int i=0; i < ALTEZZA_NEW; i++)
    {
        mvprintw(10+i, 5+NCOLS/2-LARGHEZZA_NEW/2, newMancheSprite[i]);
    }
    attroff(COLOR_PAIR(COLORI_HUD));
    refresh();
    sleep(1);
}

void stampaFrogger()
{
    WINDOW* wmenu=newwin(NLINES+HUDLINES, NCOLS, 0, 0);
    box(wmenu, '*', '*');

    wattron(wmenu, COLOR_PAIR(COLORI_HUD));
    for(int i=0; i < H_FROGGER; i++)
    {
        mvwprintw(wmenu, NLINES/2-H_FROGGER+i, 2, spriteFrogger[i]);
    }
    wattroff(wmenu, COLOR_PAIR(COLORI_HUD));
    wrefresh(wmenu);

    sleep(3);
    endwin();
}