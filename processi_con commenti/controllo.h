#ifndef CONTROLLO_H
#define CONTROLLO_H

#include "utilities.h"
#include "audio.h"
#include "rana.h"
#include "avvia.h"
#include "coccodrilli.h"
#include "campo.h"
#include "punteggio.h"
#include "menu.h"

void controllo();
void posizionaSparo(Processo p, Processo* array, int dim_array);
BoundingBox createBoundingBox(int x, int y, int larghezza, int altezza);
_Bool checksovrapposizione(BoundingBox a, BoundingBox b);
int checkTaneCollision(int x, int y, GameAudio audio);
_Bool checkWin(_Bool* tane);
void handleMorteRana(Processo* rana, int* vite, Punteggio* punti, WINDOW* wgioco, GameAudio audio);
_Bool detectCollisione(Processo* rana, Processo* cricca, Processo* astuccio, Processo* granate, Oggetto* mine, _Bool* tane, Processo last, WINDOW* wgioco, int* vite, int secondi, Punteggio* punti, GameAudio audio);
void handleHud(WINDOW* whud, int vite, Punteggio punti);

#endif