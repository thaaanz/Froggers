#include "buffer.h"

// definizione buffer come risorsa globale
Buffer b = {.w = 0, .r = 0}; //w indice di scrittura, r indice di lettura

// lettura dal buffer
Thread leggi(pthread_mutex_t* mutex) {
    Thread obj = {0, {0}}; // inizializza thread vuoto

    pthread_mutex_lock(mutex);

    // wait quando il buffer è vuoto
    while (b.w == b.r) {
        pthread_mutex_unlock(mutex);
        //usleep(DELAY_MUTEX);
        pthread_mutex_lock(mutex);
    }

    // lettura dal buffer
    obj = b.buffer[b.r];
    b.r = (b.r + 1) % DIM_BUFFER; // incremento indice di lettura

    pthread_mutex_unlock(mutex);

    return obj;
}

// scrittura nel buffer
void scrivi(pthread_mutex_t* mutex, Thread obj) {
    pthread_mutex_lock(mutex);

    // Wait quando il buffer è pieno
    while ((b.w + 1) % DIM_BUFFER == b.r) {
        pthread_mutex_unlock(mutex);
        pthread_mutex_lock(mutex);
    }

    // scrittura nel buffer
    b.buffer[b.w] = obj;
    b.w = (b.w + 1) % DIM_BUFFER; // incremento indice di scrittura

    pthread_mutex_unlock(mutex);
}