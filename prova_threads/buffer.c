#include "buffer.h"

Buffer b=
{   
    .w = 0,  //indice di scrittura (next write)
    .r = 0  //indice di lettura (next read)
};

Thread leggi(pthread_mutex_t* mutex)
{
    Thread temp={0, {' ', 0, 0, 0}};
    pthread_mutex_lock(mutex); //blocco il mutex

    if(b.w!=b.r) //se il buffer non è vuoto
    {
        temp=b.buffer[b.r]; //leggo l'elemento in indice r
        b.r=(b.r+1)%DIM_BUFFER; //incrementa r di uno (%DIM_BUFFER perchè è circolare)
    }

    pthread_mutex_unlock(mutex); //sblocco il mutex
    return(temp); //restituisco l'ultimo elemento letto
}

void scrivi(pthread_mutex_t* mutex, Thread daScrivere)
{
    pthread_mutex_lock(mutex); //blocco il mutex

    if((b.w+1)%DIM_BUFFER!=b.r) //se il buffer non è pieno
    {
        b.buffer[b.w]=daScrivere; //scrivo l'elemento
        b.w=(b.w+1)%DIM_BUFFER; //incremento l'indice w
    }
        
    pthread_mutex_unlock(mutex); //sblocco il mutex
}