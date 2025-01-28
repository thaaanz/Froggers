#include "menu.h"


const char spriteMenu[ALTEZZA_MENU][LARGHEZZA_MENU]={
    " _ __ ___   ___ _ __  _   _ ",
    "| '_ ` _ \\ / _ \\ '_ \\| | | |",
    "| | | | | |  __/ | | | |_| |",
    "|_| |_| |_|\\___|_| |_|\\__,_|"
};

const char spriteGameOver[ALTEZZA_GO][LARGHEZZA_GO]={
    "/ _` |/ _` | '_ ` _ \\ / _ \\  / _ \\ \\ / / _ \\ '__|",
    "| (_| | (_| | | | | | |  __/ | (_) \\ V /  __/ |  ", 
    "\\__, |\\__,_|_| |_| |_|\\___|  \\___/ \\_/ \\___|_|   ",
    "|___/  ",
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
    //werase(wmenu);
    int i=0;
    
    while(1){
        _Bool flag=true;
        
        while(flag){
            wclear(wmenu);
            box(wmenu, '#', '#');
            for(int j =0 ; j < ALTEZZA_MENU; j++){
                mvwprintw(wmenu, 2+j, NCOLS/2-LARGHEZZA_MENU/2, spriteMenu[j]);
            }

            mvwprintw(wmenu, 10, NCOLS/2-LARGHEZZA_MENU/2, "Regolamento");
            mvwprintw(wmenu, 13, NCOLS/2-LARGHEZZA_MENU/2, "Selezione giocatore");
            mvwprintw(wmenu, 16, NCOLS/2-LARGHEZZA_MENU/2, "Esci");
            mvwprintw(wmenu, 10+i*3, (NCOLS/2-LARGHEZZA_MENU/2)-3, "-->");

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
        switch(i){
            case 0:
                stampaRegolamento(wmenu);
                break;
            case 1:
                selezionaUtente(wmenu);
                return 0;
            case 2:
                endwin();
                return 1;
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
    flash();
    _Bool flag=true;
    int i=0;
    while(flag){
        wclear(wmenu);
            box(wmenu, '#', '#');

            mvwprintw(wmenu, 10, NCOLS/2-LARGHEZZA_MENU/2, "Banale utente");
            mvwprintw(wmenu, 13, NCOLS/2-LARGHEZZA_MENU/2, "Professore di SOPR");
            mvwprintw(wmenu, 10+i*3, (NCOLS/2-LARGHEZZA_MENU/2)-3, "-->");

            wrefresh(wmenu);
        int c=(int)wgetch(wmenu);
        switch(c)
        {
            case KEY_UP:
                i--;
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
    //endwin();
    WINDOW* wmenu=newwin(NLINES+HUDLINES, NCOLS, 0, 0);
    if(vite > 0){
        stampaWin(wmenu);
    }
    else{
        stampaGO(wmenu);
    }

    sleep(5);
}

void stampaWin(WINDOW* w){
    for(int i=0; i < ALTEZZA_WIN; i++){
        flash();
        mvwprintw(w, 2+i, NCOLS/2-LARGHEZZA_WIN/2, spriteWin[i]);
    }
    wclear(w);
    wrefresh(w);
}

void stampaGO(WINDOW* w){
    flash();
    sleep(3);
    wclear(w);

    for(int i=0; i < ALTEZZA_GO; i++){
        flash();
        mvwprintw(w, 10+i, NCOLS/2-LARGHEZZA_GO/2, spriteGameOver[i]);
    }
    wrefresh(w);
}