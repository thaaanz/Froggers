#ifndef CONTROLLO_H
#define CONTROLLO_H


#include "utilities.h"
#include "buffer.h"
#include "punteggio.h"
#include "campo.h"
#include "rana.h"
#include "coccodrilli.h"
#include "avvia.h"
#include "audio.h"


// struttura che rappresenta il rettangolo occupato da ogni oggetto
typedef struct {
    int x;
    int y;
    int width;
    int height;
} BoundingBox;

void* controllo(void* semafori);
BoundingBox createBoundingBox(int x, int y, int larghezza, int altezza);
_Bool checksovrapposizione(BoundingBox a, BoundingBox b);
int checkTaneCollision(int x, int y, GameAudio audio);
_Bool checkWin(_Bool* tane);
void handleMorteRana(Thread* rana, int* vite, Punteggio* punti, WINDOW* wgioco, GameAudio audio);
_Bool detectCollisione(Thread* rana, Thread* cricca, Thread* astuccio, Thread* granate, _Bool* tane, Thread last,  WINDOW* debug, int* vite, int secondi, Punteggio* punti, GameAudio audio);
void handleHud(WINDOW* whud, int vite, Punteggio punti);

#endif