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



//coccodrilli
#define MAX_COCCODRILLI 4 //*per riga
#define MAX_SPEED 5
#define DIR_RIGHT 1
#define DIR_LEFT -1

//grafica
#define NLINES 46
#define NCOLS 73
#define NUMERO_FLUSSI 8
#define ALTEZZA_FLUSSO 4
#define ALTEZZA_RANA 2
#define LARGHEZZA_RANA 5
#define ALTEZZA_COCCODRILLO 2
#define LARGHEZZA_COCCODRILLO 10
#define ALTEZZA_SPONDA 4
#define ALTEZZA_TANE 4
#define LARGHEZZA_TANE 71
#define LARGHEZZA_TANA 5 //* larghezza tana singola (stessa della rana)
#define ALTEZZA_MARCIAPIEDE 6
#define HUDLINES 4

//altro
#define NUMERO_TANE 5
#define MAX_VITE 3
#define MAX_INTERVALLO_PROIETTILI 5
#define DELAY 20000
#define MAX_PROIETTILI 2
#define N_GRANATE 16
#define MAX_TEMPO 50



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