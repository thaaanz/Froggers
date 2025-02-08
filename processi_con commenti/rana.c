#include "rana.h"

//GESTIONE RANA

const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA]={
    " 0-0 ",
    "(___)"
};

const Processo quit={NULL, {'q', 0, 0, NULL}};
const Processo spara={NULL, {'g', 0, 0, NULL}};

Processo avviaRana(int* pipe_fd, int* pipe_inversa) 
{
    pid_t pid;
    pid = fork();
    if (pid == -1) 
    {
        perror("Fork rana fallita");
        exit(ERRORE_FORK_RANA);
    }
    if (pid == 0) 
    {
        funzioneRana(pipe_fd, pipe_inversa); //avvio la rana
        exit(0);
    }
    Processo rana={pid, {'r', NLINES- ALTEZZA_RANA-1, NCOLS/2, NULL }}; //inizializzo la rana con la sua posizione iniziale
    return rana;
}

void funzioneRana(int* pipe_fd, int* pipe_inversa)
{
    close(pipe_fd[0]); //chiudo la pipe in lettura

    Processo rana;
    int c; //carattere di input per il movimento

    rana.pid=getpid(); //salvo il pid della rana

    while(true)
    {
        rana.item.id='r'; rana.item.y=0; rana.item.x=0, rana.item.dir=NULL;  //inizializzo le coordinate
        c=(int)getch(); //leggo l'input da tastiera

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
                write(pipe_fd[1], &quit, sizeof(Processo)); //per interrompere il gioco
                break;
            case 32: //tasto spazio, per sparare le granate
                write(pipe_fd[1], &spara, sizeof(Processo));
                avviaGranate(pipe_fd, pipe_inversa); 
                //il controllo ha il file descriptor in scrittura chiuso quindi non si possono avviare là i processi granata
                continue;
        }

        write(pipe_fd[1], &rana, sizeof(Processo)); //scrivo nella pipe le coordinate relative
        usleep(DELAY_RANA);
    }
}

void stampaRana(Oggetto r, WINDOW* w)
{
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

void inizializzaGranate(Processo* granate)
{
    for(int i=0; i<N_GRANATE; i++) //inizializzo l'array delle granate
    {
        granate[i].pid=-1;
        granate[i].item.id='o';
        granate[i].item.y=0;
        granate[i].item.x=0;
        granate[i].item.dir=0;
    }
}

void avviaGranate(int* pipe_fd, int* pipe_inversa) 
{
    Processo rana;
    read(pipe_inversa[0], &rana, sizeof(Processo)); // Leggo le coordinate della rana mandate dal controllo
    
    pid_t pid_left = fork();
    if(pid_left < 0) 
    {
        perror("fork granata fallita");
        exit(ERRORE_FORK_GRANATE);
    }
    else if(pid_left == 0) 
    {
        granata(DIR_LEFT, pipe_fd, &rana); //avvio la granata di sinistra
    }
    
    pid_t pid_right = fork();
    if(pid_right < 0) 
    {
        perror("fork granata fallita");
        exit(ERRORE_FORK_GRANATE);
    }
    else if(pid_right == 0) 
    {
        granata(DIR_RIGHT, pipe_fd, &rana); //avvio la granata di destra
    }
}

void granata(int direzione, int* pipe_fd, Processo* rana) 
{
    Processo granata; //inizializzo la granata con le coordinate corrette in base a quelle della rana
    granata.pid = getpid();
    granata.item.id = 'o';
    granata.item.y = rana->item.y + (ALTEZZA_RANA/2);
    granata.item.x = rana->item.x + (direzione == DIR_RIGHT ? LARGHEZZA_RANA : 0); //uso l'operatore ternario per stabilire da quale parte della rana partire in base alla direzione
    granata.item.dir=0; //uso una variabile direzione e non .item.dir perchè sfrutto quest'ultimo per fare in modo che il suono dello sparo venga riprodotto solo una volta e non ad ogni aggiornamento della posizione
    
    while(true) 
    {
        granata.item.x += direzione; //aggiorno la x
        write(pipe_fd[1], &granata, sizeof(Processo)); //scrivo sulla pipe
        usleep(DELAY_GRANATA);
        granata.item.dir++; //incremento .dir in modo che il suono non venga più riprodotto (vedi check nel controllo)
    }
}

void stampaGranate(Processo* g, WINDOW* w)
{
    wattron(w, COLOR_PAIR(COLORI_GRANATE));
    wattron(w, A_BOLD); 

    for (int i = 0; i < N_GRANATE; i++) //scorro l'array
            {
                if (g[i].pid != -1) //se la cella contiene una granata
                {
                    mvwaddch(w, g[i].item.y, g[i].item.x, g[i].item.id); //la stampo
                }
            }
    
    
    wattroff(w, COLOR_PAIR(COLORI_GRANATE));
    wattroff(w, A_BOLD); 
}