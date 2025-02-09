#include "rana.h"

const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA]={
    " 0-0 ",
    "(___)"
};

const Thread quit={0,{'q', 0, 0, NULL}};
const Thread spara= {0,{'g', 0, 0, NULL}};


Thread avviaRana (Semafori* s) {
    pthread_t tid_rana;
    if(pthread_create(&tid_rana, NULL, &funzioneRana, s)!=0){ //avvio la rana
        perror("ERRORE CREAZIONE THREAD RANA");
        exit(ERRORE_THREAD_RANA);
    }
    Thread rana={tid_rana,{'r', NLINES- ALTEZZA_RANA-1, NCOLS/2, NULL }};  //inizializzo la rana con la sua posizione iniziale
    return rana;
}

void* funzioneRana(void * semafori)
{
    Semafori* s=(Semafori*) semafori;
    Thread rana;
    int c; //carattere di input per il movimento
    rana.tid=pthread_self(); //salvo il tid della rana
    _Bool f=true;
    while(true)
    {
        rana.item.id='r'; rana.item.y=0; rana.item.x=0; // inizializzo le coordinate
        c=(int)getch(); //lettura input tastiera
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
                scrivi((Semafori*) semafori, quit);
                break;
            case 32: //tasto spazio per sparare le granate
                scrivi(s, spara);
                continue;
        }

        scrivi(s, rana);
        usleep(DELAY_RANA);
    }
}

void stampaRana(Oggetto r, WINDOW* w){
    wattron(w, COLOR_PAIR(COLORI_RANA));
    for (int i = 0; i < ALTEZZA_RANA; i++) 
    {
        for( int j=0; j < LARGHEZZA_RANA; j++)
        {
            mvwaddch(w, r.y+i, r.x+j,  spriteRana[i][j]); //stampo la rana
        }
    }
    wattroff(w, COLOR_PAIR(COLORI_RANA));
}

//GESTIONE GRANATE

void inizializzaGranate(Thread* granate)//inizializzo l'array delle granate
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

void avviaGranate(Thread rana, Semafori* s) {

    pthread_t tid_left, tid_right;
    static Params granataLeft = {0};
    static Params granataRight = {0};

    //inizializzo le granate con le coordinate corrette in base a quelle della rana
    granataLeft.semafori = s;
    granataLeft.item = (Oggetto){'o', rana.item.y+1, rana.item.x-1, DIR_LEFT};

    granataRight.semafori = s;
    granataRight.item = (Oggetto){'o', rana.item.y+1, rana.item.x + LARGHEZZA_RANA +1, DIR_RIGHT};

    if(pthread_create(&tid_left, NULL, &granata, &granataLeft) != 0) {
        perror("ERRORE CREAZIONE THREAD GRANATA");
        exit(ERRORE_THREAD_GRANATE);
    }
    if(pthread_create(&tid_right, NULL, &granata, &granataRight) != 0){
        perror("ERRORE CREAZIONE THREAD GRANATA");
        exit(ERRORE_THREAD_GRANATE);
    }  
    
}

void* granata(void* params) 
{
    Params* p= ((Params*) params);
    Semafori* s= p->semafori;
    Thread granata;
    granata.item=p->item;
    granata.tid = pthread_self();
    int direzione=granata.item.dir;
    granata.item.dir=0;//uso una variabile direzione e non .item.dir perchè sfrutto quest'ultimo per fare in modo che il suono dello sparo venga riprodotto solo una volta e non ad ogni aggiornamento della posizione

  
    while(granata.item.x>=-1 && granata.item.x<=NCOLS+1) //finchè la granata è dentro lo schermo
    {
        granata.item.x += direzione; //aggiorno la x
        scrivi(s, granata);
        usleep(DELAY_GRANATA);
        granata.item.dir++;//incremento .dir in modo che il suono non venga più riprodotto (vedi check nel controllo)
    }
}

void stampaGranate(Thread* g, WINDOW* w)
{
    wattron(w, COLOR_PAIR(COLORI_GRANATE));
    wattron(w, A_BOLD); 

    for (int i = 0; i < N_GRANATE; i++)//scorro l'array
            {
                if (g[i].tid != -1)//se la cella contiene una granata
                {
                    mvwaddch(w, g[i].item.y, g[i].item.x, g[i].item.id);//la stampo
                }
            }
    
    
    wattroff(w, COLOR_PAIR(COLORI_GRANATE));
    wattroff(w, A_BOLD); 
}