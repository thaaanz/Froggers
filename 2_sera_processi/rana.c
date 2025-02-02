#include "utilities.h"
#include "rana.h"

const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA]={
    " 0-0 ",
    "(___)"
};

const Processo quit={NULL, {'q', 0, 0, NULL}};
const Processo spara={NULL, {'g', 0, 0, NULL}};

Processo avviaRana(int* pipe_fd, int* pipe_inversa) // crea processo rana
{
    
    pid_t pid;
    pid = fork();
    if (pid == -1) 
    {
        endwin();
        perror("Fork rana fallita");
        exit(1);
    }
    if (pid == 0) 
    {
        funzioneRana(pipe_fd, pipe_inversa);
        exit(0);
    }
    Processo rana={pid, {'r', NLINES- ALTEZZA_RANA-1, NCOLS/2, NULL }}; //dà la posizione iniziale e la passa al controllo
    return rana;
}

void funzioneRana(int* pipe_fd, int* pipe_inversa)
{
    close(pipe_fd[0]); //chiusura pipe in lettura
    Processo rana;
    int c;

    rana.pid=getpid(); //!è davvero necessario?
    while(true)
    {
        rana.item.id='r'; rana.item.y=0; rana.item.x=0, rana.item.dir=NULL; // scrive sulla pipe le coordinate relative
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
                write(pipe_fd[1], &quit, sizeof(Processo));
                break;
            case 32:
                write(pipe_fd[1], &spara, sizeof(Processo));
                avviaGranate(pipe_fd, pipe_inversa); 
                //il controllo ha il file descriptor in scrittura chiuso quindi non si possono avviare là i processi granata
                break;
        }

        write(pipe_fd[1], &rana, sizeof(Processo));
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
            mvwaddch(w, r.y+i, r.x+j,  spriteRana[i][j]);
        }
    }
    wattroff(w, COLOR_PAIR(COLORI_RANA));
}

void inizializzaGranate(Processo* granate)
{
    for(int i=0; i<N_GRANATE; i++)
    {
        granate[i].pid=-1;
        granate[i].item.id='o';
        granate[i].item.y=0;
        granate[i].item.x=0;
        granate[i].item.dir=0;
    }
}

void avviaGranate(int* pipe_fd, int* pipe_inversa) {
    Processo rana;
    read(pipe_inversa[0], &rana, sizeof(Processo));    // Legge prima le coordinate della rana mandate dal controllo
    
    pid_t pid_left = fork();
    if(pid_left < 0) {
        perror("fork granata fallita");
    }
    else if(pid_left == 0) {
        granata(DIR_LEFT, pipe_fd, &rana); 
        exit(3);
    }
    
    pid_t pid_right = fork();
    if(pid_right < 0) {
        perror("fork granata fallita");
    }
    else if(pid_right == 0) {
        granata(DIR_RIGHT, pipe_fd, &rana); 
        exit(3);
    }
}

void granata(int direzione, int* pipe_fd, Processo* rana) 
{
    Processo granata;
    granata.pid = getpid();
    granata.item.id = 'o';
    granata.item.y = rana->item.y + (ALTEZZA_RANA/2);
    // a seconda della direzione parte da due punti diversi
    granata.item.x = rana->item.x + (direzione == DIR_RIGHT ? LARGHEZZA_RANA : 0);
    
    while(true) {
        granata.item.x += direzione;
        write(pipe_fd[1], &granata, sizeof(Processo));
        usleep(DELAY_GRANATA);
    }
}


void stampaGranata(Oggetto g, WINDOW* w)
{
    wattron(w, COLOR_PAIR(COLORI_PROIETTILI));
    wattron(w, A_BOLD); 

    mvwaddch(w, g.y, g.x, g.id);
    
    wattroff(w, COLOR_PAIR(COLORI_PROIETTILI));
    wattroff(w, A_BOLD); 
}