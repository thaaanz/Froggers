#ifndef UTILITIES_H
#define UTILITIES_H
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
#define ALTEZZA_COCCODRILLO 2
#define LARGHEZZA_COCCODRILLO 10
#define MAX_COCCODRILLI 4 //*per riga
#define ALTEZZA_TANE 5
#define MAX_INTERVALLO_PROIETTILI 5
#define DELAY 20000
#define ALTEZZA_FLUSSO 4
#define MAX_SPEED 5
#define DIR_RIGHT 1
#define DIR_LEFT -1
#define NLINES 40
#define NCOLS 100
#define MAX_PROIETTILI 2
#define ALTEZZA_RANA 2
#define LARGHEZZA_RANA 5
#define N_GRANATE 16
#define ALTEZZA_MARCIAPIEDE 4
#define N_PROIETTILI 32 //*numero di flussi*numero di coccodrilli
#define HUDLINES 3
#define MAX_BULLETS_PER_COCCO 2

typedef struct{
    char id;
    int y, x;
    int dir;
}Oggetto;

typedef struct{
    int speed;
    int dir;
    int y;
}Flusso;

typedef struct{
    pid_t pid;
    Oggetto item;
}Processo;

#endif