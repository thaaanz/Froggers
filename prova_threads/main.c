#include "utilities.h"
#include "controllo.h"
#include "avvia.h"

int main()
{
    avviancurses();
    srand(time(NULL));

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    pthread_t tid_controllo;
    pthread_create(&tid_controllo, NULL, &controllo, &mutex);

    //pthread_join(tid_rana, NULL);
    pthread_join(tid_controllo, NULL);

    pthread_mutex_destroy(&mutex);

    flash();
    endwin();
}