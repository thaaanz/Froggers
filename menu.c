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
    " |___/  ",
};
const char spriteWin[ALTEZZA_WIN][LARGHEZZA_WIN]={
" _   _  ___  _   _  __      _____  _ __ | |",
"| | | |/ _ \\| | | | \\ \\ /\\ / / _ \\| '_ \\| |",
"| |_| | (_) | |_| |  \\ V  V / (_) | | | |_|",
" \\__, |\\___/ \\__,_|   \\_/\\_/ \\___/|_| |_(_)",
"|___/   "
};

_Bool utentePrivilegiato=false; // variabile globale

int menuIniziale(){
    WINDOW* wmenu=newwin(NLINES+HUDLINES, NCOLS, 0, 0);
    keypad(wmenu, TRUE); 
    int i=0;
    
    while(true){
        _Bool flag=true;
        
        while(flag){
            wclear(wmenu);
            box(wmenu, '#', '#');
            //stampa sprite menù
            wattron(wmenu, COLOR_PAIR(COLORI_HUD));
            for(int j =0 ; j < ALTEZZA_MENU; j++){ 
                mvwprintw(wmenu, 2+j, NCOLS/2-LARGHEZZA_MENU/2, spriteMenu[j]);
            }
            wattroff(wmenu, COLOR_PAIR(COLORI_HUD));

            mvwprintw(wmenu, 10, NCOLS/2-LARGHEZZA_MENU/2, "Regolamento");
            mvwprintw(wmenu, 13, NCOLS/2-LARGHEZZA_MENU/2, "Selezione giocatore");
            mvwprintw(wmenu, 16, NCOLS/2-LARGHEZZA_MENU/2, "Esci");

            wattron(wmenu, COLOR_PAIR(COLORI_HUD));
            mvwprintw(wmenu, 10+i*3, (NCOLS/2-LARGHEZZA_MENU/2)-3, "-->"); // freccia che si sposta in base a cosa è selezionato
            wattroff(wmenu, COLOR_PAIR(COLORI_HUD));

            wrefresh(wmenu);

            int c=(int)wgetch(wmenu);
            switch(c)
            {
                case KEY_UP:
                    i--;
                    if(i<0) i=2;
                    break;
                case KEY_DOWN:
                    i++;
                    break;
                case 10:
                    flag=false;
                    break;
            }
            i=i%3;

    
            usleep(2000);
        }
        //se si esce dal ciclo è stato premuto invio
        //controllo che indice è stato selezionato
        switch(i){
            case 0:
                stampaRegolamento(wmenu);
                break;
            case 1:
                selezionaUtente(wmenu);
                return 0; //gestito dal main
            case 2:
                endwin();
                return 1; // gestito dal main
                break;
        }
    }
    
}

void stampaRegolamento(WINDOW* wmenu){
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
    while(c!= 'q'){
        c=wgetch(wmenu);
    }
}

void selezionaUtente(WINDOW* wmenu){

    _Bool flag=true;
    int i=0;
    while(flag){
        wclear(wmenu);
            box(wmenu, '#', '#');

            mvwprintw(wmenu, 10, NCOLS/2-LARGHEZZA_MENU/2, "Banale utente");
            mvwprintw(wmenu, 13, NCOLS/2-LARGHEZZA_MENU/2, "Professore di SOPR");
            
            wattron(wmenu, COLOR_PAIR(COLORI_HUD));
            mvwprintw(wmenu, 10+i*3, (NCOLS/2-LARGHEZZA_MENU/2)-3, "-->");
            wattroff(wmenu, COLOR_PAIR(COLORI_HUD));

            wrefresh(wmenu);

        int c=(int)wgetch(wmenu);
        switch(c)
        {
            case KEY_UP:
                i--;
                if(i<0) i=1;
                break;
            case KEY_DOWN:
                i++;
                break;
            case 10:
                flag=false;
                break;
        }
        i=i%2;
    }
    if(i==1){
        utentePrivilegiato=true;
    }
}

void menuFinale(Punteggio punti, int vite){

    clear();
    refresh();
    WINDOW* wmenu=newwin(NLINES+HUDLINES, NCOLS, 0, 0);

    if(vite > 0){
        stampaWin(wmenu);
    }
    else{
        stampaGO(wmenu);
    }

    sleep(3);

    //riepilogo punti, premi un tasto qualsiasi per andare avanti
    wclear(wmenu);
    box(wmenu, '#', '#');
    mvwprintw(wmenu, 3, 4, "Hai totalizzato i seguenti punti: %d", totalePunti(punti)); 
    wrefresh(wmenu);
    usleep(350000);
    mvwprintw(wmenu, 5, 4, "Chiusura tane: %d", punti.tane);
    wrefresh(wmenu);
    usleep(350000);
    mvwprintw(wmenu, 7, 4, "Salti: %d", punti.tane);
    wrefresh(wmenu);
    usleep(350000);
    mvwprintw(wmenu, 9, 4, "Esplosione dei proiettili: %d", punti.proiettili);
    wrefresh(wmenu);
    usleep(350000);
    mvwprintw(wmenu, 11, 4, "Scorrere del tempo: %d", punti.tempo);
    wrefresh(wmenu);
    usleep(350000);
    mvwprintw(wmenu, 13, 4, "Morti: -%d", punti.morte);
    wrefresh(wmenu);
    usleep(350000);
    wattron(wmenu, COLOR_PAIR(COLORI_HUD));
    mvwprintw(wmenu, 15, 4, "Premi un tasto per continuare");
    wattroff(wmenu, COLOR_PAIR(COLORI_HUD));
    wrefresh(wmenu);
    wgetch(wmenu);
}

void stampaWin(WINDOW* w){
     wclear(w);
    for(int i=0; i < ALTEZZA_WIN; i++){
        flash();
        mvwprintw(w, 2+i, NCOLS/2-LARGHEZZA_WIN/2, spriteWin[i]);
    }
    wrefresh(w);
}

void stampaGO(WINDOW* w){
    wclear(w);
    for(int i=0; i < ALTEZZA_GO; i++){
        mvwprintw(w, 10+i, NCOLS/2-LARGHEZZA_GO/2, spriteGameOver[i]);
    }
    wrefresh(w);
}

_Bool restart(){
    WINDOW* wmenu=newwin(NLINES+HUDLINES, NCOLS, 0, 0);
    keypad(wmenu, TRUE); 
    int i=0;
    _Bool flag=true;
    while(flag){
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
                i--;
                if(i<0) i=1;
                break;
            case KEY_DOWN:
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
    if(i==0){ //restart nuova partita
        return true;
    }
    else return false;
}