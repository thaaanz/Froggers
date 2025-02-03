#include "rana.h"

const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA]={
    " 0-0 ",
    "(___)"
};

const Thread quit={0,{'q', 0, 0, NULL}};
const Thread spara= {0,{'g', 0, 0, NULL}};


Thread avviaRana (pthread_mutex_t* mutex) {
    pthread_t tid_rana;
    pthread_create(&tid_rana, NULL, &funzioneRana, mutex);
    Thread rana={pthread_self(),{'r', NLINES- ALTEZZA_RANA-1, NCOLS/2, NULL }}; 
    return rana;
}

void* funzioneRana(void * mutex)
{
    pthread_mutex_t* m=(pthread_mutex_t*) mutex;
    Thread rana;
    int c;

    _Bool f=true;
    while(true)
    {
        rana.item.id='r'; rana.item.y=0; rana.item.x=0; // scrive sulla pipe le coordinate relative
        c=(int)getch(); //lettura input tastiera
        //flash();
        switch(c)
        {
            case KEY_UP:
                rana.item.y= -ALTEZZA_FLUSSO;
                break;
            case KEY_DOWN:
                rana.item.y= +ALTEZZA_FLUSSO;
                break;
            case KEY_LEFT:
                rana.item.x= -1;
                break;
            case KEY_RIGHT:
                rana.item.x= +1;
                break;
            case 'q':
                //write(pipe_fd[1], &quit, sizeof(Processo));
                f=false;
                break;
            case 32:
                scrivi(m, spara);
                break;
        }

        if(f) scrivi(m, rana);
        usleep(DELAY_RANA);
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

void inizializzaGranate(Thread* granate)
{
    for(int i=0; i<N_GRANATE; i++)
    {
        granate[i].tid=-1;
        granate[i].item.id='o';
        granate[i].item.y=0;
        granate[i].item.x=0;
        granate[i].item.dir=0;
    }
}

void avviaGranate(Thread rana, pthread_mutex_t* mutex) {

    pthread_t tid_left, tid_right;
    static Params granataLeft = {0};
    static Params granataRight = {0};

    granataLeft.mutex = mutex;
    granataLeft.item = (Oggetto){'o', rana.item.y+1, rana.item.x-1, DIR_LEFT};

    granataRight.mutex = mutex;
    granataRight.item = (Oggetto){'o', rana.item.y+1, rana.item.x + LARGHEZZA_RANA +1, DIR_RIGHT};

    if(pthread_create(&tid_left, NULL, &granata, &granataLeft) != 0) {
        //gestione errore thread boh
    }
    if(pthread_create(&tid_right, NULL, &granata, &granataRight) != 0){
        //gestione errore
    }  
    
}

void* granata(void* params) 
{
    Params* p= ((Params*) params);
    pthread_mutex_t* m= p->mutex;
    Thread granata;
    granata.item=p->item;
    granata.tid = pthread_self();
  
    while(true) {
        granata.item.x += granata.item.dir;
        scrivi(m, granata);
        pthread_testcancel();
        usleep(DELAY_GRANATA);
    }
    return NULL;
}

void stampaGranata(Oggetto g, WINDOW* w)
{
    wattron(w, COLOR_PAIR(COLORI_PROIETTILI));
    wattron(w, A_BOLD); 

    mvwaddch(w, g.y, g.x, g.id);
    
    wattroff(w, COLOR_PAIR(COLORI_PROIETTILI));
    wattroff(w, A_BOLD); 
}