#ifndef MENU_H
#define MENU_H

#include "utilities.h"
#include "punteggio.h"

#define ALTEZZA_MENU 4
#define LARGHEZZA_MENU 31
#define ALTEZZA_GO 4
#define LARGHEZZA_GO 54
#define ALTEZZA_WIN 5
#define LARGHEZZA_WIN 49

extern const char spriteMenu[ALTEZZA_MENU][LARGHEZZA_MENU];
extern const char spriteGameOver[ALTEZZA_GO][LARGHEZZA_GO];
extern const char spriteWin[ALTEZZA_WIN][LARGHEZZA_WIN];
extern _Bool utentePrivilegiato;

int menuIniziale();
void stampaRegolamento(WINDOW* wmenu);
void selezionaUtente(WINDOW* wmenu);

void menuFinale(Punteggio punti, int vite);
void stampaWin(WINDOW* w);
void stampaGO(WINDOW* w);

#endif