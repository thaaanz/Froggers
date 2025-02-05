#include "utilities.h"
#include "rana.h"
#include "controllo.h"
#include "avvia.h"
#include "menu.h"
#include "buffer.h"

// nella versione processi c'è avia controllo e menù
// io cambierei mettendo qua nel main la creazione dei thread
//+ nella versione processi non creiamo un processo controllo, qua sì?
int main()
{
    avviancurses();
    srand(time(NULL));

    if (menuIniziale())
    {
        return 0; // non si vuole giocare
    };

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    sem_t sem_liberi, sem_occupati;
    sem_init(&sem_liberi, 0, DIM_BUFFER);
    sem_init(&sem_occupati, 0, 0);

    Semafori semafori={sem_liberi, sem_occupati, mutex};

    do
    {
        clear();
        refresh();
        pthread_t tid_controllo;
        pthread_create(&tid_controllo, NULL, &controllo, &semafori);
        pthread_join(tid_controllo, NULL);

    } while (restart());
    
    //pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_occupati);
    sem_destroy(&sem_liberi);

    endwin();
}