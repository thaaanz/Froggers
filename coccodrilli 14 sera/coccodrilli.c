#include "utilities.h"
#include <ctype.h>

const char spriteCoccodrillo[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO] = {
    "---------<",
    "-W-W-W-W--"
};


void stampaCoccodrillo(Oggetto coccodrillo, WINDOW* window, int n)
{ 
    for (int i = 0; i < ALTEZZA_COCCODRILLO; i++) { 
        for (int j = 0; j < LARGHEZZA_COCCODRILLO; j++) {
            int y = coccodrillo.y + i; 
            int x = coccodrillo.x + j; 
            mvwaddch(window, y, x, spriteCoccodrillo[i][j]); 
        } 
    } 
}

void stampaCoccodrilli(Processo* coccodrilli, WINDOW* window) {
    
    //  stampa tutti i coccodrilli attivi
    for (int j = 0; j < MAX_COCCODRILLI*NUMERO_FLUSSI; j++) {
        if (coccodrilli[j].pid != 0) {
            stampaCoccodrillo(coccodrilli[j].item, window, j);
        }
    }
}

void avviaCoccodrilli(Flusso* fiume, Processo* coccodrilli, int* pipe_fd, WINDOW* wgioco) {
    pid_t temp;
    // Inizializza il generatore di numeri casuali
    srand(time(NULL));
    flash();
    mvwprintw(wgioco, NLINES/2, NCOLS/2, "Avvio...");
    wrefresh(wgioco);
    // Itera su ogni flusso
    for (int j = 0; j < NUMERO_FLUSSI; j++) {
        // E per ogni coccodrillo nel flusso
        for (int i = 0; i < MAX_COCCODRILLI; i++) {
            int index = j *(MAX_COCCODRILLI) + i;
            //flash();
            if (coccodrilli[index].pid == 0) {
                temp = fork();
                
                if (temp < 0) {
                    perror("fork cocco");
                    exit(18);
                }
                else if (temp == 0) {
                    // Processo figlio
                    coccodrilli[index].pid = getpid();
                    coccodrilli[index].item.y = fiume[j].y;
                    coccodrilli[index].item.id = 'c';
                    coccodrilli[index].item.dir = index;
                    
                    // Debug print
                    /*mvwprintw(wgioco, 2, 0, "Cocco %d: flusso %d, y=%d", index, j, fiume[j].y);
                    wrefresh(wgioco); */

                    // Ritardo iniziale casuale
                    usleep(fiume[j].speed * 1000000 + (rand() % 5000000)*3);
                    coccodrillo(fiume[j], pipe_fd, index, wgioco);
                    exit(1);
                }
                usleep(200); // Piccolo ritardo tra la creazione dei coccodrilli
            }
        }
    }
}

void coccodrillo(Flusso flusso, int pipe_fd[], int cod, WINDOW* window) {
    close(pipe_fd[0]); // Chiudi l'estremità di lettura
    
    // Inizializza la struttura del coccodrillo
    Processo coccodrillo = {
        getpid(),
        {
            'c',
            flusso.y + 1,
            flusso.dir == DIR_RIGHT ? -LARGHEZZA_COCCODRILLO : NCOLS,
            cod
        }
    };
    
    
    srand(getpid());

    if(cod >0 && cod <= MAX_COCCODRILLI){
        //usleep(10000);
    }
    else if(cod >MAX_COCCODRILLI && cod <= MAX_COCCODRILLI*2){
        usleep(3000000 + rand()%500000);
    }
    else {
        usleep(6000000+ rand()%500000);
    }

    

    // Loop infinito per il movimento
    while (1) {
        //check se c'è un altro carattere coccodrillo e nel caso wait 1 sec e riprova
        while(1){
                    chtype c = mvwinch(window, coccodrillo.item.y, 1 );
                    char carattere=(char)(c );
            
                    _Bool flag=false;
                    for(int i=0; i < ALTEZZA_COCCODRILLO; i++){
                        for( int j=0 ; j <LARGHEZZA_COCCODRILLO; j ++) {
                            if (c==spriteCoccodrillo[i][j]){
                                flash();
                                flag=true;
                            }
                        }
                    }
                    
                    if(!flag){
                        break;
                    }
                    else{
                        //flash();
                        sleep(1);
                    }
                }
        // Gestisci il wrapping dello schermo
        if (flusso.dir == DIR_RIGHT) {
            if (coccodrillo.item.x > NCOLS) {
                coccodrillo.item.x = -LARGHEZZA_COCCODRILLO;
                usleep(rand() % 1000000); // Ritardo casuale prima di riapparire
                
                }
        } else {
            if (coccodrillo.item.x < -LARGHEZZA_COCCODRILLO) {
                coccodrillo.item.x = NCOLS;
                usleep(rand() % 1000000); // Ritardo casuale prima di riapparire
            }
        }
        
        // Invia la posizione attraverso la pipe
        write(pipe_fd[1], &coccodrillo, sizeof(Processo));
        
        // Aggiorna la posizione
        coccodrillo.item.x += flusso.dir;
        
        // Attendi in base alla velocità
        usleep(fdelay(flusso.speed));
    }
}

int fdelay(int speed) {
    switch (speed) {
        case 1: return 300000;
        case 2: return 250000;
        case 3: return 200000;
        case 4: return 150000;
        case 5: return 100000;
        default: return 500000;
    }
}

_Bool isCarattereCoccodrillo(char c){
    for(int i=0; i < ALTEZZA_COCCODRILLO; i++){
        for( int j=0 ;j <LARGHEZZA_COCCODRILLO; j ++) {
            if (c==spriteCoccodrillo[i][j]){
                return 1;
            }
        }
    }
    return 0;
}