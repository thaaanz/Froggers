#include "utilities.h"
#include "rana.h"
#include "controllo.h"
#include "avvia.h"

// nella versione processi c'è avia controllo e menù
// io cambierei mettendo qua nel main la creazione dei thread
//+ nella versione processi non creiamo un processo controllo, qua sì?
int main()
{
    avviancurses();
    srand(time(NULL));

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    pthread_t tid_rana, tid_consumatore;
    pthread_create(&tid_rana, NULL, &funzioneRana, &mutex);
    pthread_create(&tid_consumatore, NULL, &controllo, &mutex);

    pthread_join(tid_rana, NULL);
    pthread_join(tid_consumatore, NULL);

    pthread_mutex_destroy(&mutex);

    flash();
    endwin();
}