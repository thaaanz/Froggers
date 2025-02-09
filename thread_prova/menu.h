#ifndef MENU_H
#define MENU_H

#include "utilities.h"
#include "punteggio.h"
#include "audio.h"

extern const char spriteMenu[ALTEZZA_MENU][LARGHEZZA_MENU];
extern const char spriteGameOver[ALTEZZA_GO][LARGHEZZA_GO];
extern const char spriteWin[ALTEZZA_WIN][LARGHEZZA_WIN];
extern const char spriteWin[ALTEZZA_WIN][LARGHEZZA_WIN];
extern const char spriteFrogger[H_FROGGER][W_FROGGER];
extern _Bool utentePrivilegiato;

int menuIniziale(GameAudio* audio);
void stampaRegolamento(WINDOW* wmenu);
void selezionaUtente(WINDOW* wmenu, GameAudio* audio);
void menuFinale(Punteggio punti, int vite, GameAudio* audio);
_Bool restart(GameAudio* audio);
void stampaWin(WINDOW* w);
void stampaGO(WINDOW* w);
void stampaNewManche();
void stampaFrogger();

#endif
