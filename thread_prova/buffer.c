#include "buffer.h"

// definizione buffer come risorsa globale
Buffer b = {.w = 0, .r = 0}; //w indice di scrittura, r indice di lettura


Thread leggi(Semafori* semafori)
{
    Thread letto = {0, {0}};
    
    //attendo che ci siano elementi da leggere
    sem_wait(&semafori->sem_occupati);
    //ingresso sezione critica
    pthread_mutex_lock(&semafori->mutex);
    
    letto = b.buffer[b.r];  //lettura in base all'indice di lettura
    b.r = (b.r + 1) % DIM_BUFFER; //aggiorno l'indice di lettura
    
    //uscita sezione critica
    pthread_mutex_unlock(&semafori->mutex);
    //segnalo uno spazio libero
    sem_post(&semafori->sem_liberi);
    
    return letto;
}

void scrivi(Semafori* semafori, Thread daScrivere)
{
    //attendo che ci siano spazi liberi
    sem_wait(&semafori->sem_liberi);
    //ingresso sezione critica
    pthread_mutex_lock(&semafori->mutex);
    
    b.buffer[b.w] = daScrivere; //scrittura in base all'indice di scrittura
    b.w = (b.w + 1) % DIM_BUFFER; //aggiorno l'indice di scrittura
    
    //uscita sezione critica
    pthread_mutex_unlock(&semafori->mutex);
    //segnalo uno spazio occupato
    sem_post(&semafori->sem_occupati);
}