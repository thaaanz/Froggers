#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <stdbool.h>

#define MAX_VITE 3
#define NUMERO_FLUSSI 8
#define ALTEZZA_RANA 2
#define LARGHEZZA_RANA 5
#define ALTEZZA_COCCO 2
#define LARGHEZZA_COCCO 10
#define ALTEZZA_TANE 5
#define MAX_INTERVALLO_PROIETTILI 5
#define DELAY 20000
#define ALTEZZA_FLUSSO 4
#define MAX_SPEED 5
#define DIR_RIGHT 1
#define DIR_LEFT -1
#define NLINES 30
#define NCOLS 20

const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA]={
    " 0-0 ",
    "(___)"
};

const char spriteCoccodrillo[ALTEZZA_COCCO][LARGHEZZA_COCCO]={
    "---------<",
    "-W-W-W-W- "
};

typedef struct{
    char id;
    int y, x;
}Oggetto;

typedef struct {
    int speed;
    int direzione;
    int y;
}Flusso;

WINDOW* wgioco;

const Oggetto quit={'q', 0, 0};

void avviaPipe(int pipe_fd[]);
void avviancurses();
void avviaRana(int* pipe_fd);
void rana(int pipe_fd[]);
void stampaRana(Oggetto pos, WINDOW* wgioco);
void controllo(int pipe_fd[]);
void sparaGranate(Oggetto obj, int pipe_fd[]);
void proiettile(Oggetto obj, int dir, int pipe_fd[]);
Flusso* avviaFlussi();
//void generaCoccodrilli(int pipe_fd[]);
void cleanupGame(Flusso* fiume);

int main()
{
    int pipe_fd[2];
    avviaPipe(pipe_fd);
    avviancurses();

    avviaRana(pipe_fd);

    controllo(pipe_fd);

    endwin();
}

void avviaPipe(int pipe_fd[])
{
    if(pipe(pipe_fd)==-1)
    {
        perror("pipe fallita");
        exit(1);
    }
}

void avviancurses()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE); 
    //srand(time(NULL));
}

void avviaRana(int* pipe_fd) {
    pid_t pid;
    pid = fork();
    if (pid == -1) 
    {
        endwin();
        perror("Fork rana fallita");
        exit(1);
    }
    if (pid == 0) {
        rana(pipe_fd);
        exit(0);
    }
}

void rana(int pipe_fd[])
{
    close(pipe_fd[0]);
    Oggetto rana;
    int c;

    while(1)
    {
        rana.id='r'; rana.y=0; rana.x=0; 

        c=(int)getch();
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
                write(pipe_fd[1], &quit, sizeof(Oggetto));
                return;
            case 32:
                rana.id='g';
                break;
        }
        
        write(pipe_fd[1], &rana, sizeof(Oggetto));
        usleep(DELAY);
    }
}

void controllo(int pipe_fd[]) 
{
    close(pipe_fd[1]);

    wgioco=newwin(NLINES, NCOLS, 0, 0);

    Oggetto rana={'r', NLINES-ALTEZZA_RANA-1, NCOLS/2};
    Oggetto temp, proiettile;

    Flusso* fiume=avviaFlussi();

    while(true)
    {
        wclear(wgioco);
        box(wgioco, ACS_VLINE, ACS_HLINE);
        stampaRana(rana, wgioco);

        for(int i=0; i<NUMERO_FLUSSI; i++)
        {
            mvwprintw(wgioco, fiume[i].y, 2, "direziione: %d - velocita' %d", fiume[i].direzione, fiume[i].speed);
        }

        wrefresh(wgioco);
        
        read(pipe_fd[0], &temp, sizeof(Oggetto));
        switch(temp.id)
        {
            case 'r':
                rana.x+=temp.x;
                rana.y+=temp.y;
                if(rana.x<0) rana.x=0;
                if(rana.y<0) rana.y=0;
                if(rana.x>NCOLS-LARGHEZZA_RANA) rana.x=NCOLS-LARGHEZZA_RANA;
                if(rana.x>NLINES-ALTEZZA_RANA) rana.y=NLINES-ALTEZZA_RANA;
                break;
            case 'q':
                cleanupGame(fiume); 
                break;
            case 'g': 
                sparaGranate(rana, pipe_fd);
                break;
            case 'o':
                proiettile=temp;
                mvwaddch(wgioco, proiettile.y, proiettile.x, proiettile.id);
                break;
        }

        usleep(DELAY);
    }
}

void stampaRana(Oggetto pos, WINDOW* wgioco) 
{
    for (int i = 0; i < ALTEZZA_RANA; i++) {
        for( int j=0; j < LARGHEZZA_RANA; j++){
            mvwaddch(wgioco, pos.y +i, pos.x+j,  spriteRana[i][j]);
        }
    }
}

Flusso* avviaFlussi(void) 
{
    Flusso* fiume = malloc(NUMERO_FLUSSI * sizeof(Flusso));
    if (fiume == NULL) {
        endwin();
        perror("Errore allocazione memoria");
        exit(1);
    }

    srand(time(NULL));  // Inizializza il generatore di numeri casuali
    int dir = -1 + (rand() % 3);
    int y_pos = 2;  // Inizia da una posizione più in alto

    for(int i = 0; i < NUMERO_FLUSSI; i++) {
        fiume[i].direzione = dir;
        fiume[i].speed = 1 + (rand() % MAX_SPEED);  // Velocità minima 1
        fiume[i].y = y_pos;
        y_pos += 2;  // Spazia i flussi verticalmente
        dir = -dir;  // Alterna la direzione
    }
    return fiume;
}

void liberaFlussi(Flusso* fiume) 
{
    if (fiume != NULL) {
        free(fiume);
    }
}

void cleanupGame(Flusso* fiume)
{
    endwin();
    liberaFlussi(fiume);
}

void sparaGranate(Oggetto obj, int pipe_fd[])
{
    pid_t pid=fork();
    if(pid<0)
    {
        perror("fork call");
        _exit(12);
    }
    else
    {
        proiettile(obj, DIR_RIGHT, pipe_fd);
    }

    pid_t pidd=fork();
    if(pid<0)
    {
        perror("fork call");
        _exit(13);
    }
    else
    {
        proiettile(obj, DIR_LEFT, pipe_fd);
    }
}

void proiettile(Oggetto obj, int dir, int pipe_fd[])
{
    Oggetto proiettile={'o', obj.x, obj.y};

    while(proiettile.y>0 && proiettile.y<COLS-2)
    {
        write(pipe_fd[1], &proiettile, sizeof(Oggetto));
        proiettile.y+=dir;

        usleep(DELAY/2);
    }

}