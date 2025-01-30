#include "rana.h"

void* funzioneRana(void * mutex);
void* consumatore(void* mutex);
Oggetto leggi(pthread_mutex_t* mutex);
void scrivi(pthread_mutex_t* mutex, Oggetto boh);

const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA]={
    " 0-0 ",
    "(___)"
};

const Oggetto quit={'q', 0, 0, NULL};
const Oggetto spara= {'g', 0, 0, NULL};



void* funzioneRana(void * mutex)
{
    pthread_mutex_t* m=(pthread_mutex_t*) mutex;
    Oggetto rana;
    int c;

    while(true)
    {
        rana.id='r'; rana.y=0; rana.x=0; // scrive sulla pipe le coordinate relative
        c=(int)getch(); //lettura input tastiera

        switch(c)
        {
            case KEY_UP:
                rana.y= -ALTEZZA_FLUSSO;
                break;
            case KEY_DOWN:
                rana.y= +ALTEZZA_FLUSSO;
                break;
            case KEY_LEFT:
                rana.x= -1;
                break;
            case KEY_RIGHT:
                rana.x= +1;
                break;
            case 'q':
                //write(pipe_fd[1], &quit, sizeof(Processo));
                break;
            case 32:
                scrivi(mutex, spara);
                break;
        }

        scrivi(mutex, rana);
        usleep(DELAY);

    }
}

void stampaRana(Oggetto r, WINDOW* w){
    wattron(w, COLOR_PAIR(COLORI_RANA));
    for (int i = 0; i < ALTEZZA_RANA; i++) 
    {
        for( int j=0; j < LARGHEZZA_RANA; j++)
        {
            mvwaddch(w, r.y+i, r.x+j,  spriteRana[i][j]);
        }
    }
    wattroff(w, COLOR_PAIR(COLORI_RANA));
}