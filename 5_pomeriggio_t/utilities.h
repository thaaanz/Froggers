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
//agiunte dei thread
#include <pthread.h>
#include <semaphore.h>




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
#define LARGHEZZA_TANE 73
#define LARGHEZZA_TANA 5 //* larghezza tana singola (stessa della rana)
#define ALTEZZA_MARCIAPIEDE 6
#define HUDLINES 4
#define TCOLS 4

//colori
#define COLORI_RANA 1
#define COLORI_COCCODRILLO 2
#define COLORI_HUD 3
#define COLORI_TANE 4
#define COLORI_TEMPO 5
#define COLORI_FIUME 6
#define COLORI_MARCIAPIEDE 7
#define COLORI_SPONDA 8
#define COLORI_PROIETTILI 9
#define GREEN_TEMPO 10
#define YELLOW_TEMPO 11
#define RED_TEMPO 12
#define COLOR_DARK_GREEN 8  // Usa un numero > 7 per colori custom
#define COLOR_GRAY 9
#define COLOR_DARK_BLUE 10
#define COLOR_BROWN 11

//altro
#define NUMERO_TANE 5
#define MAX_VITE 3
#define MAX_INTERVALLO_PROIETTILI 5
#define DELAY 20000
#define DELAY_CONTROLLO 1100
#define DELAY_RANA 20000
#define DELAY_GRANATA 35000
#define DELAY_PROIETTILE 40000
#define DELAY_MUTEX 1000
#define N_GRANATE 20
#define MAX_TEMPO 60
#define N_PROIETTILI 32//* max coccodrilli * numero flussi

//aggiunte dei thread 
#define DIM_BUFFER 500


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
    pthread_t tid;
    Oggetto item;
}Thread;

typedef struct{
    int w;
    int r;
    Thread buffer[DIM_BUFFER];
} Buffer;

typedef struct{
    sem_t sem_liberi;
    sem_t sem_occupati;
    pthread_mutex_t mutex;
} Semafori;

typedef struct{
    Oggetto item;
    Semafori* semafori;
} Params;

#endif