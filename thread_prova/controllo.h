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

void *controllo(void *parametri);
BoundingBox createBoundingBox(int x, int y, int larghezza, int altezza);
_Bool checksovrapposizione(BoundingBox a, BoundingBox b);
int checkTaneCollision(int x, int y, GameAudio audio);
_Bool checkWin(_Bool* tane);
void handleMorteRana(Thread* rana, int* vite, Punteggio* punti, WINDOW* wgioco, GameAudio audio);
_Bool detectCollisione(Thread* rana, Thread* cricca, Thread* astuccio, Thread* granate, Oggetto* mine, _Bool* tane, Thread last,  WINDOW* debug, int* vite, int secondi, Punteggio* punti, GameAudio audio);
void handleHud(WINDOW* whud, int vite, Punteggio punti);
void handleTempo(WINDOW *wtempo, int start);
void posizionaSparo(Thread p, Thread* array, int dim_array);

#endif