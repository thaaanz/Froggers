#ifndef CONTROLLO_H
#define CONTROLLO_H

void controllo(int* pipe_fd, int* pipe_inversa);
//_Bool detectCollisione (Processo* rana, Processo* cricca, Processo* granate, Processo last, int * vite, WINDOW* debug);
_Bool detectCollisione(Processo*rana, Processo *cricca, Processo *granate, Processo last, int *vite,  WINDOW* debug, _Bool * tane);

#endif