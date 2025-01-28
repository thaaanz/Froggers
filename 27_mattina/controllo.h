#ifndef CONTROLLO_H
#define CONTROLLO_H

// strttura che rappresenta il rettangolo occupato da ogni oggetto
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
void handleMorteRana(Processo* rana, int* vite, WINDOW* wgioco);
_Bool detectCollisione(Processo* rana, Processo* cricca, Processo* astuccio, Processo* granate, _Bool* tane, Processo last, WINDOW* wgioco, WINDOW* debug, int* vite, int secondi, int* punti);
void handleHud(WINDOW* whud, int vite, int start);

#endif