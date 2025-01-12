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
#define NCOLS 50
#define MAX_PROIETTILI 2
#define MAX_COCCODRILLI 6 //per flusso

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

void avviaPipe(int pipe_fd[]);
void avviancurses();
int fdelay(int speed);
void coccodrillo(Flusso f, int pipe_fd[]);
void stampaCoccodrillo(Oggetto coccodrillo);
void avviaCoccodrilli(Flusso* fiume, pid_t* pidCoccodrilli, int pipe_fd[]);
Flusso* avviaFlussi();
void cleanupGame(Flusso* fiume);



int main()
{
    int pipe_fd[2];
    avviaPipe(pipe_fd);
    avviancurses();

    Flusso* fiume=avviaFlussi();
    pid_t pidCoccodrilli[NUMERO_FLUSSI];

    avviaCoccodrilli(fiume, pidCoccodrilli, pipe_fd);
    Oggetto coccodrillo;
    Oggetto coccodrilli[8];
    int c=0;
    while(c<80)
    {
        clear();
        read(pipe_fd[0], &coccodrillo, sizeof(Oggetto));
        mvprintw(LINES/2, COLS/2, "%d", coccodrillo.y);
        switch(coccodrillo.y){
            case 3:
                coccodrilli[0]=coccodrillo;
                break;
            case 7:
                coccodrilli[1]=coccodrillo;
                break;
            case 11:
                coccodrilli[2]=coccodrillo;
                break;
            case 15:
                coccodrilli[3]=coccodrillo;
                break;
            case 19:
                coccodrilli[4]=coccodrillo;
                break;
            case 23:
                coccodrilli[5]=coccodrillo;
                break;
            case 27:
                coccodrilli[6]=coccodrillo;
                break;
            case 31:
                coccodrilli[7]=coccodrillo;
                break;
        }
        for(int i=0; i< 8; i++)
            stampaCoccodrillo(coccodrilli[i]);
            //stampaCoccodrillo(coccodrilli[1]);
        refresh();
        usleep(50000);
        c++;
    }

    cleanupGame(fiume);
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

void coccodrillo(Flusso f, int pipe_fd[])
{
    close(pipe_fd[0]);
    Oggetto coccodrillo={'c', f.y+1, f.direzione==DIR_RIGHT ? 1 : NCOLS};

    while(true)
    {
        write(pipe_fd[1], &coccodrillo, sizeof(Oggetto));

        coccodrillo.x+=f.direzione;
        
        int delay=fdelay(f.speed);

        usleep(delay);
    }
}

int fdelay(int speed)
{
    switch(speed)
    {
        case 1: return 500000; break;
        case 2: return 400000; break;
        case 3: return 300000; break;
        case 4: return 200000; break;
        case 5: return 100000; break;
    }
}

void stampaCoccodrillo(Oggetto coccodrillo)
{
    for (int i = 0; i < ALTEZZA_COCCO; i++) {
        for( int j=0; j < LARGHEZZA_COCCO; j++){
            mvaddch(coccodrillo.y +i, coccodrillo.x+j,  spriteCoccodrillo[i][j]);
        }
    }
}

void avviaCoccodrilli(Flusso* fiume, pid_t* pidCoccodrilli, int pipe_fd[])
{
    for(int i=0; i<NUMERO_FLUSSI; i++)
    {
        pid_t temp=fork();
        if(temp<0)
        {
            perror("fork cocco");
            exit(18);
        }
        else if(temp==0)
        {
            pidCoccodrilli[i]=temp;
            coccodrillo(fiume[i], pipe_fd);
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
        y_pos += 4;  // Spazia i flussi verticalmente
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