#include "buffer.h"

// definizione buffer come risorsa globale
Buffer b = {.w = 0, .r = 0}; //w indice di scrittura, r indice di lettura


Thread leggi(Semafori* semafori) //lettura dal buffer
{
    Thread letto={0, {0}};

    if(b.r!=b.w) //mentre il buffer non è vuoto DA RIVEDERE
    {
        sem_wait(&semafori->sem_occupati);
        pthread_mutex_lock(&semafori->mutex);

        letto=b.buffer[b.r];
        b.r = (b.r + 1) % DIM_BUFFER;

        pthread_mutex_unlock(&semafori->mutex);
        sem_post(&semafori->sem_liberi);
    }
       
    return letto;
}


void scrivi(Semafori* semafori, Thread daScrivere)
{
    if(b.w<DIM_BUFFER)
    {
        sem_wait(&semafori->sem_liberi);
        pthread_mutex_lock(&semafori->mutex);

        b.buffer[b.w]=daScrivere;
        b.w = (b.w + 1) % DIM_BUFFER;

        pthread_mutex_unlock(&semafori->mutex);
        sem_post(&semafori->sem_occupati);
    }
}
