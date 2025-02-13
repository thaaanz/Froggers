#ifndef UTILITIES_H
#define UTILITIES_H

//librerie
#include <unistd.h>
#include <stdlib.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <stdbool.h>

//rana
#define ALTEZZA_RANA 2
#define LARGHEZZA_RANA 5

//coccodrilli
#define MAX_COCCODRILLI_PER_FLUSSO 4
#define MAX_SPEED 5
#define DIR_RIGHT 1
#define DIR_LEFT -1
#define ALTEZZA_COCCODRILLO 2
#define LARGHEZZA_COCCODRILLO 10

//grafica
#define NLINES 46
#define NCOLS 73
#define HUDLINES 4
#define TCOLS 4
#define NUMERO_FLUSSI 8
#define ALTEZZA_FLUSSO 4
#define ALTEZZA_SPONDA 4
#define ALTEZZA_TANE 4
#define LARGHEZZA_TANE 73
#define LARGHEZZA_TANA 5 //* larghezza tana singola (stessa della rana)
#define ALTEZZA_MARCIAPIEDE 6

//colori
#define COLORI_RANA 1
#define COLORI_COCCODRILLO 2
#define COLORI_HUD 3
#define COLORI_TANE 4
#define COLORI_TEMPO 5
#define COLORI_FIUME 6
#define COLORI_MARCIAPIEDE 7
#define COLORI_SPONDA 8
#define COLORI_GRANATE 9
#define COLORI_PROIETTILI 9
#define COLORI_MINE 11
#define GREEN_TEMPO 10
#define YELLOW_TEMPO 11
#define RED_TEMPO 12
#define COLOR_DARK_GREEN 8  // Usa un numero > 7 per colori custom
#define COLOR_GRAY 9
#define COLOR_DARK_BLUE 10
#define COLOR_BROWN 11

//menù
#define ALTEZZA_MENU 4
#define LARGHEZZA_MENU 31
#define ALTEZZA_GO 5
#define LARGHEZZA_GO 54
#define ALTEZZA_WIN 5
#define LARGHEZZA_WIN 49
#define ALTEZZA_NEW 5
#define LARGHEZZA_NEW 73
#define H_FROGGER 11
#define W_FROGGER 79

//altro
#define N_PROIETTILI 10//* max coccodrilli * numero flussi
#define MAX_INTERVALLO_PROIETTILI 10
#define N_GRANATE 20
#define N_MINE 3
#define DIST_MAX_MINE 30
#define DIST_MIN_MINE 14
#define NUMERO_TANE 5
#define MAX_VITE 3
#define MAX_TEMPO 60

//delay
#define DELAY_PROIETTILE 40000
#define DELAY_GRANATA 35000
#define DELAY_RANA 20000
#define DELAY_CONTROLLO 1100

//punti
#define PUNTI_TANA 200
#define PUNTI_SALTO_AVANTI 20
#define PUNTI_SALTO_INDIETRO -20
#define PUNTI_PROIETTILI 10
#define PUNTI_MORTE 100
#define PUNTI_PER_SEC 0.5

//errori
#define ERRORE_AVVIO_PIPE 1
#define ERRORE_FORK_RANA 2
#define ERRORE_FORK_COCCODRILLI 3 
#define ERRORE_FORK_PROIETTILI 4
#define ERRORE_FORK_GRANATE 5
#define ERRORE_KILL_RANA 6
#define ERRORE_KILL_COCCODRILLI 7
#define ERRORE_KILL_PROIETTILI 8
#define ERRORE_KILL_GRANATE 9
#define ERRORE_KILL_SPARO 10 //generico per la funzione posizionaSparo



//strutture
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

typedef struct{
    int x;
    int y;
    int width;
    int height;
} BoundingBox; //rappresenta il rettangolo occupato da ogni oggetto

typedef struct{
    int tempo;
    int salti;
    int proiettili;
    int tane;
    int morte;
} Punteggio;


#endif