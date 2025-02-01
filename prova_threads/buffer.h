#ifndef BUFFER_H
#define BUFFER_H

#include "utilities.h"

typedef struct{
    int w;
    int r;
    Thread buffer[DIM_BUFFER];
} Buffer;


Thread leggi(pthread_mutex_t* mutex);
void scrivi(pthread_mutex_t* mutex, Thread daScrivere);

#endif 