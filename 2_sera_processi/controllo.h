#ifndef CONTROLLO_H
#define CONTROLLO_H

#include "utilities.h"
//#include "fiume.h"
#include "rana.h"
#include "avvia.h"
#include "coccodrilli.h"
#include "campo.h"
#include "punteggio.h"
#include "menu.h"

// struttura che rappresenta il rettangolo occupato da ogni oggetto
typedef struct {
    int x;
    int y;
    int width;
    int height;
} BoundingBox;

void controllo(int* pipe_fd, int* pipe_inversa);
BoundingBox createBoundingBox(int x, int y, int larghezza, int altezza);
_Bool checksovrapposizione(BoundingBox a, BoundingBox b);
int checkTaneCollision(int x, int y);
_Bool checkWin(_Bool* tane);
void handleMorteRana(Processo* rana, int* vite, Punteggio* punti, WINDOW* wgioco);
_Bool detectCollisione(Processo* rana, Processo* cricca, Processo* astuccio, Processo* granate, _Bool* tane, Processo last, WINDOW* wgioco, int* vite, int secondi, Punteggio* punti);
void handleHud(WINDOW* whud, int vite, Punteggio punti);

#endif