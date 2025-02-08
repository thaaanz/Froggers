#ifndef MENU_H
#define MENU_H

#include "utilities.h"
#include "punteggio.h"

extern const char spriteMenu[ALTEZZA_MENU][LARGHEZZA_MENU];
extern const char spriteGameOver[ALTEZZA_GO][LARGHEZZA_GO];
extern const char spriteWin[ALTEZZA_WIN][LARGHEZZA_WIN];
extern const char spriteWin[ALTEZZA_WIN][LARGHEZZA_WIN];
extern _Bool utentePrivilegiato;

int menuIniziale();
void stampaRegolamento(WINDOW* wmenu);
void selezionaUtente(WINDOW* wmenu);

void menuFinale(Punteggio punti, int vite);
void stampaWin(WINDOW* w);
void stampaGO(WINDOW* w);
void stampaNewManche(WINDOW* w);

#endif
