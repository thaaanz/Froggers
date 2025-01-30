#include "buffer.h"

Buffer b = {.w = 0, .r = 0};

Oggetto leggi(pthread_mutex_t* mutex)
{
    Oggetto obj={0, 0, ' '};

    pthread_mutex_lock(mutex);

    if(b.w!=b.r)
    {
        obj=b.buffer[b.r];
        b.r=(b.r+1)%DIM_BUFFER;
    }

    pthread_mutex_unlock(mutex);

    return(obj);
}

void scrivi(pthread_mutex_t* mutex, Oggetto boh)
{
    pthread_mutex_lock(mutex);

    if((b.w+1)%DIM_BUFFER!=b.r) //se il buffer non è pieno
    {
        b.buffer[b.w]=boh; //scrivi
        b.w=(b.w+1)%DIM_BUFFER; //incrementa l'indice
    }
        
    pthread_mutex_unlock(mutex);

}