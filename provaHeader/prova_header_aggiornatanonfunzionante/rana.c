#include <curses.h>
#include "ok.h"
#include "rana.h"
#define ALTEZZA_RANA 2
#define LARGHEZZA_RANA 5
#define ALTEZZA_FLUSSO 4
#define DELAY 20000




const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA]={
    " 0-0 ",
    "(___)"
};

const Oggetto quit={'q', 0, 0};

void rana(int pipe_fd[])
{
    close(pipe_fd[0]);
    Oggetto rana;
    int c;
    _Bool change=false;
    while(1)
    {
        rana.id='r'; rana.y=0; rana.x=0; 
        change=false;
        c=(int)getch();
        switch(c)
        {
            case KEY_UP:
                rana.y= -ALTEZZA_FLUSSO;
                change=true;
                break;
            case KEY_DOWN: 
                rana.y= +ALTEZZA_FLUSSO;
                change=true;
                break;
            case KEY_LEFT:
                rana.x= -1;
                change=true;
                break;
            case KEY_RIGHT:
                rana.x= +1;
                change=true;
                break;
            case 'q':
                write(pipe_fd[1], &quit, sizeof(Oggetto));
                return;
            /*case 32:
                rana.id='g';
                change=true;
                break;*/
        }
        
        if(change) write(pipe_fd[1], &rana, sizeof(Oggetto));
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