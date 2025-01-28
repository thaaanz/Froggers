#ifndef MENU_H
#define MENU_H

#include "utilities.h"

#define ALTEZZA_MENU 4
#define LARGHEZZA_MENU 31

extern const char spriteMenu[ALTEZZA_MENU][LARGHEZZA_MENU];
extern _Bool utentePrivilegiato;

int menuIniziale();
void stampaRegolamento(WINDOW* wmenu);
void selezionaUtente(WINDOW* wmenu);

#endif