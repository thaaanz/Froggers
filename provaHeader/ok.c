#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <stdbool.h>

#define MAX_VITE 3
#define NUMERO_FLUSSI 8
#define ALTEZZA_RANA 2
#define LARGHEZZA_RANA 5
#define ALTEZZA_COCCO 2
#define LARGHEZZA_COCCO 10
#define ALTEZZA_TANE 5
#define MAX_INTERVALLO_PROIETTILI 5

#define MAX_SPEED 5
#define DIR_RIGHT 1
#define DIR_LEFT -1
#define NLINES 30
#define NCOLS 50
#define MAX_PROIETTILI 2


typedef struct{
    char id;
    int y, x;
}Oggetto;

typedef struct {
    pid_t pid;
    bool attivo;
} ProcessoProiettile;

typedef struct {
    int speed;
    int direzione;
    int y;
}Flusso;

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

void avviaRana(int* pipe_fd) {
    pid_t pid;
    pid = fork();
    if (pid == -1) 
    {
        endwin();
        perror("Fork rana fallita");
        exit(1);
    }
    if (pid == 0) {
        rana(pipe_fd);
        exit(0);
    }
}

Flusso* avviaFlussi(void) 
{
    Flusso* fiume = malloc(NUMERO_FLUSSI * sizeof(Flusso));
    if (fiume == NULL) {
        endwin();
        perror("Errore allocazione memoria");
        exit(1);
    }

    srand(time(NULL));  // Inizializza il generatore di numeri casuali
    int dir = -1 + (rand() % 3);
    int y_pos = 2;  // Inizia da una posizione più in alto

    for(int i = 0; i < NUMERO_FLUSSI; i++) {
        fiume[i].direzione = dir;
        fiume[i].speed = 1 + (rand() % MAX_SPEED);  // Velocità minima 1
        fiume[i].y = y_pos;
        y_pos += 2;  // Spazia i flussi verticalmente
        dir = -dir;  // Alterna la direzione
    }
    return fiume;
}