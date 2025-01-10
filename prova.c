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

typedef struct{
    char id;
    int vary, varx;
}Spostamento;

void avviaPipe(int* fds)
{
    if(pipe(fds)==-1)
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

void avviaGioco(int pipe_fd[2]) //contollo
{
    close(pipe_fd[1]);
    avviaRana(pipe_fd);

    int vite=MAX_VITE;
    _Bool flagCollisione=FALSE;
    Spostamento temp;

    //loop principale
    while(vite > 0){ //da capire dove salvare queste vite, idealmente in una strttura
        read(pipe_fd[0], &temp, sizeof(Spostamento)); // anche qua bisogna vedere in che strttura salvare le cose lette dalla pipe
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

void avviaRana() //pasiamo la pipe
{
    pid_t pid;
    if((pid=fork())<0)
    {
        perror("fork rana fallita");
    }
    else if(pid==0) //figlio
    {
        gestioneRana();
    }
}

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
                gestioneCock();
            }
        }
    }  
}

int main()
{
    int pipe_fd[2];
    avviaPipe(pipe_fd);
    avviancurses();

    pid_t pid;

    pid=fork();
    if(pid<0)
    {
        perror("fork fallita");
    }
    else if(pid==0) //primo figlio
    {

    }
    else //padre
    {
        pid=fork();
        if(pid<0)
        {
            perror("fork fallita");
        }
        else if(pid==0) //secondo figlio
        {

        }
        else //padre
        {

        }
    }


    avviaGioco(pipe_fd);
    return 0;
}


/*
- funzioni per le collisioni
- funzioni per la stampa
- funzioni per la gestione di rana e coccodrillo
- tutti i pid nella struttura principale
- funzione che controlla se c'è spazio per i coccodrilli
*/