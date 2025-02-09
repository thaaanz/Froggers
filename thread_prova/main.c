#include "utilities.h"
#include "rana.h"
#include "controllo.h"
#include "avvia.h"
#include "menu.h"
#include "buffer.h"

int main()
{
    avviancurses(); //inizializzazione ncurses e colori
    srand(time(NULL));

    stampaFrogger(); //stampa grafica iniziale

    GameAudio audio; //inizializzo la struttura contenente gli audio di gioco
    inizializzaAudio(&audio); //inizializzo l'audio

    riproduciMusica(audio.musica_menu);

    if (menuIniziale(&audio)) //display del menu inziale
    {
        return 0; // non si vuole giocare
    };

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL); //creazione del mutex

    sem_t sem_liberi, sem_occupati; 
    sem_init(&sem_liberi, 0, DIM_BUFFER); //creazione semaforo per i posti liberi, parte da DIM_BUFFER
    sem_init(&sem_occupati, 0, 0); //creazione semaforo per i posti occupati, parte da 0

    Semafori semafori={sem_liberi, sem_occupati, mutex}; //inizializzazione struttura contenente i semafori

    ParametriControllo parametri={semafori, audio}; //parametri da passare al controllo

    do
    {
        clear(); //pulizia dello schermo
        refresh();
        pthread_t tid_controllo;
        pthread_create(&tid_controllo, NULL, &controllo, &parametri); //creazione thread di controllo
        pthread_join(tid_controllo, NULL); //aspetto il controllo termini

    } while (restart(&audio)); //controllo se si vuole ricominciare
    
    //distruzione semafori e mutex
    sem_destroy(&sem_occupati);
    sem_destroy(&sem_liberi);
    pthread_mutex_destroy(&mutex);

    freeAudio(&audio);

    endwin();
}