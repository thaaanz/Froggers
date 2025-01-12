#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <stdbool.h>
#include "ok.h"
#include "rana.h"

WINDOW* wgioco;

const char spriteCoccodrillo[ALTEZZA_COCCO][LARGHEZZA_COCCO]={
    "---------<",
    "-W-W-W-W- "
};

void controllo(int pipe_fd[]);
void sparaGranate(Oggetto obj, int pipe_fd[], pid_t * pid);
void proiettile(Oggetto obj, int dir, int pipe_fd[]);

//void generaCoccodrilli(int pipe_fd[]);
void cleanupGame(Flusso* fiume, pid_t * granate);

int main()
{
    int pipe_fd[2];
    avviaPipe(pipe_fd);
    avviancurses();

    avviaRana(pipe_fd);

    controllo(pipe_fd);

    endwin();
}



void controllo(int pipe_fd[]) 
{
    close(pipe_fd[1]);

    wgioco=newwin(NLINES, NCOLS, 0, 0);

    Oggetto rana={'r', NLINES-ALTEZZA_RANA-1, NCOLS/2};
    Oggetto temp, proiettile;

    pid_t pidgranate[2];
    Flusso* fiume=avviaFlussi();

    while(true)
    {
        wclear(wgioco);
        box(wgioco, ACS_VLINE, ACS_HLINE);
        stampaRana(rana, wgioco);

        for(int i=0; i<NUMERO_FLUSSI; i++)
        {
            mvwprintw(wgioco, fiume[i].y, 2, "direziione: %d - velocita' %d", fiume[i].direzione, fiume[i].speed);
        }

        wrefresh(wgioco);
        
        read(pipe_fd[0], &temp, sizeof(Oggetto));
        switch(temp.id)
        {
            case 'r':
                rana.x+=temp.x;
                rana.y+=temp.y;
                if(rana.x<=0) rana.x=1;
                if(rana.y<=0) rana.y=1;
                if(rana.x>=NCOLS-LARGHEZZA_RANA) rana.x=NCOLS-LARGHEZZA_RANA - 1;
                if(rana.y>=NLINES-ALTEZZA_RANA) rana.y=NLINES-ALTEZZA_RANA -1 ;
                break;
            case 'q':
                cleanupGame(fiume, pidgranate); 
                break;
            case 'g': 
                sparaGranate(rana, pipe_fd, pidgranate);
                //mvwprintw(wgioco, 3,3, "sono nello switch");
                wrefresh(wgioco);
                
                break;
            case 'o':
                flash();
                proiettile=temp;
                mvwaddch(wgioco, proiettile.y, proiettile.x, proiettile.id);
                break;
        }

        usleep(DELAY);
    }
}





void liberaFlussi(Flusso* fiume) 
{
    if (fiume != NULL) {
        free(fiume);
    }
}

void cleanupGame(Flusso* fiume, pid_t * granate)
{
    endwin();
    liberaFlussi(fiume);
    kill(granate[1], 9);
    kill(granate[0], 9);
}

void sparaGranate(Oggetto obj, int pipe_fd[], pid_t * pid)
{
    // Inizializza il tracciamento dei processi
    
    static ProcessoProiettile proiettili[MAX_PROIETTILI];
    static bool inizializzato = false;
    
    if (!inizializzato) {
        for (int i = 0; i < MAX_PROIETTILI; i++) {
            proiettili[i].pid = -1;
            proiettili[i].attivo = false;
        }
        inizializzato = true;
    }

  
    // Trova spazi liberi per i nuovi proiettili
    int slotSinistro = -1, slotDestro = -1;
    for (int i = 0; i < MAX_PROIETTILI; i++) {
        if (!proiettili[i].attivo) {
            if (slotSinistro == -1) {
                slotSinistro = i;
            } else if (slotDestro == -1) {
                slotDestro = i;
                break;
            }
        }
    }

    // Lancia proiettile sinistro
    if (slotSinistro != -1) {
        pid_t pid_sinistro = fork();
        if (pid_sinistro < 0) {
            perror("fork proiettile sinistro fallita");
        } else if (pid_sinistro == 0) {
            proiettile(obj, DIR_LEFT, pipe_fd);
            _exit(0);
        } else {
            proiettili[slotSinistro].pid = pid_sinistro;
            proiettili[slotSinistro].attivo = true;
            pid[0] = pid_sinistro;
        }
    }

    //claudio ha detto di provare a mettere un delay
    usleep(2000);

    // Lancia proiettile destro
    if (slotDestro != -1) {
        pid_t pid_destro = fork();
        if (pid_destro < 0) {
            perror("fork proiettile destro fallita");
        } else if (pid_destro == 0) {
            proiettile(obj, DIR_RIGHT, pipe_fd);
            _exit(0);
        } else {
            proiettili[slotDestro].pid = pid_destro;
            proiettili[slotDestro].attivo = true;
            pid[1] = pid_destro;
        }
    }

    // Pulisci i processi dei proiettili completati
    for (int i = 0; i < MAX_PROIETTILI; i++) {
        if (proiettili[i].attivo) {
            int status;
            pid_t risultato = waitpid(proiettili[i].pid, &status, WNOHANG);
            if (risultato > 0) {
                proiettili[i].attivo = false;
                proiettili[i].pid = -1;
            }
        }
    }
}

void proiettile(Oggetto obj, int dir, int pipe_fd[])
{
    close(pipe_fd[0]);  // Chiudi l'estremità di lettura nel processo figlio
    
    Oggetto proiettile;
    proiettile.id='o';
    proiettile.y=obj.y + (ALTEZZA_RANA / 2);  // Parti dal centro della rana
    proiettile.x = obj.x + (dir == DIR_RIGHT ? LARGHEZZA_RANA : 0);  // Parti dal lato appropriato

    
    // Ciclo di movimento
    while (proiettile.x > 1 && proiettile.x < NCOLS-2) {
        // Invia posizione del proiettile
        
        // Muovi il proiettile
        proiettile.x += dir;
        write(pipe_fd[1], &proiettile, sizeof(Oggetto));
        usleep(DELAY);  // Controlla la velocità del proiettile
    }

}