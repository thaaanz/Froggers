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
#define DELAY 20000
#define ALTEZZA_FLUSSO 4
#define MAX_SPEED 5
#define DIR_RIGHT 1
#define DIR_LEFT -1
#define NLINES 30
#define NCOLS 50
#define MAX_PROIETTILI 2

const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA]={
    " 0-0 ",
    "(___)"
};

const char spriteCoccodrillo[ALTEZZA_COCCO][LARGHEZZA_COCCO]={
    "---------<",
    "-W-W-W-W- "
};

typedef struct{
    char id;
    int y, x;
}Oggetto;

typedef struct {
    int speed;
    int direzione;
    int y;
}Flusso;

typedef struct {
    pid_t pid;
    bool attivo;
} ProcessoProiettile;

void avviaPipe(int pipe_fd[]);
void avviancurses();