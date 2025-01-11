#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <time.h>

#define MAX_VITE 3
#define FLUSSI 8
#define I_SPRITE_RANA 2
#define J_SPRITE_RANA 5
#define I_SPRITE_COCCO 2
#define J_SPRITE_COCCO 10
#define ALTEZZA_TANE 5
#define MAX_INTERVALLO_PROIETTILI 5
#define VEL_GRANATA 1000 //provvisorio


char spriteRana[I_SPRITE_RANA][J_SPRITE_RANA]={
    " 0-0 "
    "(___)"
};

char spriteCoccodrillo[I_SPRITE_COCCO][J_SPRITE_COCCO]={
    "---------<"
    "-W-W-W-W- "
};

typedef struct{
    char id;
    int y, x;
}Oggetto;

void avviaPipe(int pipe_fd[])
{
    if(pipe(pipe_fd)==-1)
    {
        perror("pipe fallita");
        exit(1);
    }
}

void avviancurses()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE); 
}

//controllo
void avviaGioco(int pipe_fd[]) //contollo
{
    close(pipe_fd[1]);
    avviaRana(pipe_fd);

    int vite=MAX_VITE;
    _Bool flagCollisione=FALSE;
    Oggetto temp;

    //loop principale
    while(vite > 0){ //da capire dove salvare queste vite, idealmente in una strttura
        read(pipe_fd[0], &temp, sizeof(Oggetto)); // anche qua bisogna vedere in che strttura salvare le cose lette dalla pipe
        avviaCock(); //controlla se sono morti coccodrilli e se c'è spazio ne spawna altri, memorizza id velocità etc nella struttura principale(?)

        detectCollsione(flagCollisione); //= true se trova collisioni

        if(flagCollisione){
            gestisciCollisione(); // da passare la matrice in modo che gestisca
        }

        aggiornaSchermo(); //stampa tutto

        stampaInfoGame(); //stampa di vite tempo etc, da aggiornare a ogni ciclo oppure solo quando cambia?(una volta al secondo)

        gestioneTana(); // se si è arrivati alla tana bisogna cancellare tutto lo schermo e far ripartire i processi
    
    }

}

//rana
void avviaRana(int pipe_fd[]) //passiamo la pipe
{
    pid_t pid;
    if((pid=fork())<0)
    {
        perror("fork rana fallita");
    }
    else if(pid==0) //figlio
    {
        gestioneRana(pipe_fd);
    }
}

Oggetto inizializzaRana()
{
    Oggetto rana={'r', LINES-2, COLS/2};
    return rana;
}

void stampaOggetto(Oggetto daStampare)
{
    // Controllo bordi verticali
    if (daStampare.y < 0 || daStampare.y >= LINES) return;

    if (daStampare.id == 'r') {
        // Controllo bordi orizzontali per la rana
        if (daStampare.x + J_SPRITE_RANA > COLS) return;
        
        for (int i = 0; i < I_SPRITE_RANA && daStampare.y + i < LINES; i++) {
            mvprintw(daStampare.y + i, daStampare.x, "%s", spriteRana[i]);
        }
    }
    else if (daStampare.id == 'c') {
        // Controllo bordi orizzontali per il coccodrillo
        if (daStampare.x + J_SPRITE_COCCO > COLS) return;
        
        for (int i = 0; i < I_SPRITE_COCCO && daStampare.y + i < LINES; i++) {
            mvprintw(daStampare.y + i, daStampare.x, "%s", spriteCoccodrillo[i]);
        }
    }
    else {
        mvaddch(daStampare.y, daStampare.x, '*');
    }
}

void gestioneRana(int pipe_fd[])
{
    close(pipe_fd[0]);
    Oggetto rana=inizializzaRana();
    int c;

    while(1)
    {
        c=(int)getch();
        switch(c)
        {
            case KEY_UP:
                if(rana.y>0) rana.y--; 
                break;
            case KEY_DOWN: //non è lines-2 perchè c'è roba sotto, da modificare
                if((rana.y+I_SPRITE_RANA-1)<=LINES-2) rana.y++;
                break;
            case KEY_LEFT:
                if(rana.x>0) rana.x--;
                break;
            case KEY_RIGHT:
                if((rana.x+J_SPRITE_RANA-1)<=COLS-2) rana.x++;
                break;
            case 32:
                sparaProiettile(pipe_fd, 0, VEL_GRANATA, rana);
                break;
        }
        write(pipe_fd[1], &rana, sizeof(Oggetto));
    }
}

//coccodrilli
void avviaCock()
{
    for(int i=0; i<FLUSSI; i++)
    {
        if(spazioCock()) //passiamo il numero di riga e qualcosa per il numero di coccodrilli
        {
            pid_t pid;
            if((pid=fork())<0)
            {
                perror("fork coccodrillo fallita");
            }
            else if(pid==0) //figlio
            {
                //gestioneCock();
            }
        }
    }  
}

void gestioneCock(int dir, int vel, int * pipe, int flusso) // 
{ //cacca
    close(pipe[0]);
    //dir=1 va a destra
    Oggetto cock; 
    cock.id='c';
    if(dir==1){
        cock.x=0; //spawn tutto a sinistra
    }
    else{
        cock.x=COLS; //spawn tutto a destra
    }

    cock.y= flusso*I_SPRITE_RANA+ ALTEZZA_TANE; //offset dato dal marciapiede

    int delay=vel; //dipende dalla velocità è da decidere in base a cosa, qualche costante?
    int random;
    while(1)
    {
        random=rand();
        cock.x+=dir;
        write(pipe[1], &cock, sizeof(Oggetto));
        usleep(delay);
    
        if((random % MAX_INTERVALLO_PROIETTILI)==0){ // costante da creare
            sparaProiettile(pipe, dir, vel, cock);
        }
    }
}

void sparaProiettile( int* pipe, int dir, int vel, Oggetto pistolero){ //bisogna capire dove mettere i pid per poi fare la kill nel momento in cui escono dallo schermo
    if(pistolero.id == "c")
        {
            pid_t pid=fork();
            if(pid < 0)
            {
                perror("fork proiettile fallita");
            }
            if (pid==0)
                {
                    movimentoProiettile( pipe, dir, vel, pistolero.x, pistolero.y); //ne crea solo uno con quella determinata direzione
                    kill(pid);
                }
        }
        // gli passo la velocità del flusso per gestire la velocità del proiettile?
        // o ogni proiettile ha sempre la stessa?
    if (pistolero.id == "r")
        {
            pid_t pid=fork();
            if(pid < 0)
            {
                perror("fork proiettile fallita");
            }
            if (pid==0)
                {
                movimentoProiettile( pipe, dir, vel, pistolero.x, pistolero.y);
                kill(pid);
                }
            else
            {
                pid=fork();
                if(pid < 0)
                {
                    perror("fork proiettile fallita");
                }
                if (pid==0)
                {
                    int opp=-dir;
                    movimentoProiettile( pipe, opp, vel, pistolero.x, pistolero.y);
                    kill(pid);
                    //else è il padre e continua nel while di gestioneCock
                }
            }
        }

}

int main()
{
    int pipe_fd[2];
    avviaPipe(pipe_fd);
    avviancurses();
    avviaGioco(pipe_fd);
    return 0;
}


/*
- funzioni per le collisioni
- funzioni per la stampa
- funzioni per la gestione di rana e coccodrillo
- tutti i pid nella struttura principale
- funzione che controlla se c'è spazio per i coccodrilli

cacca
*/
