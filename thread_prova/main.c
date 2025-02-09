#include "utilities.h"
#include "rana.h"
#include "controllo.h"
#include "avvia.h"
#include "menu.h"
#include "buffer.h"

int main()
{
    avviancurses();
    srand(time(NULL));

    stampaFrogger();

    GameAudio audio; //inizializzo la struttura contenente gli audio di gioco
    inizializzaAudio(&audio); //inizializzo l'audio

    riproduciMusica(audio.musica_menu);

    if (menuIniziale(&audio))
    {
        return 0; // non si vuole giocare
    };

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    sem_t sem_liberi, sem_occupati;
    sem_init(&sem_liberi, 0, DIM_BUFFER);
    sem_init(&sem_occupati, 0, 0);

    Semafori semafori={sem_liberi, sem_occupati, mutex};

    ParametriControllo parametri={semafori, audio};

    do
    {
        clear();
        refresh();
        pthread_t tid_controllo;
        pthread_create(&tid_controllo, NULL, &controllo, &parametri);
        pthread_join(tid_controllo, NULL);

    } while (restart(&audio));
    
    sem_destroy(&sem_occupati);
    sem_destroy(&sem_liberi);

    freeAudio(&audio);

    endwin();
}