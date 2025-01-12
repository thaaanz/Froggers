#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <stdbool.h>
#include "ok.h"
#include "coccodrillo.h"

#include "utilities.h"

const char spriteCoccodrillo[ALTEZZA_COCCO][LARGHEZZA_COCCO]={
    "---------<",
    "-W-W-W-W- "
};

/*void stampaCoccodrillo(Oggetto coccodrillo, WINDOW* window)
{
    for (int i = 0; i < ALTEZZA_COCCO; i++) {
        for( int j=0; j < LARGHEZZA_COCCO; j++){
            mvwaddch(window, coccodrillo.y +i, coccodrillo.x+j,  spriteCoccodrillo[i][j]);
        }
    }
}*/
void stampaCoccodrilli(Processo * coccodrilli, WINDOW* window){

    for(int v=0; v < MAX_COCCODRILLI; v++){
        for (int i = 0; i < ALTEZZA_COCCO; i++) { 
            for (int j = 0; j < LARGHEZZA_COCCO; j++) { 
                int y = coccodrilli[v].item.y + i; 
                int x = coccodrilli[v].item.x + j; // Add debug statements 
                mvwprintw(window, 0, 0, "Drawing at: y=%d x=%d", y, x); 

                if (y >= 0 && y < getmaxy(window) && x >= 0 && x < getmaxx(window)) {
                    mvwaddch(window, y, x, spriteCoccodrillo[i][j]); 
                } 
            } 
        } 
    }
wrefresh(window);
}


void avviaCoccodrilli(Flusso* fiume, Processo* coccodrilli, int pipe_fd[])
{
    for(int i=0; i < MAX_COCCODRILLI; i++)
    {
        pid_t temp=fork();
        if(temp<0)
        {
            perror("fork cocco");
            exit(18);
        }
        else if(temp==0)
        {
            coccodrillo(fiume[i], pipe_fd);
            exit(1);
        }
        else{
            coccodrilli[i].pid=temp;
        }
    }
}

void coccodrillo(Flusso flusso, int pipe_fd[])
{
    close(pipe_fd[0]);
    Oggetto coccodrillo={'c', flusso.y+1, flusso.direzione==DIR_RIGHT ? 1 : NCOLS};

    while(coccodrillo.x > 1- LARGHEZZA_COCCO && coccodrillo.x < NCOLS + LARGHEZZA_COCCO)
    {
        write(pipe_fd[1], &coccodrillo, sizeof(Oggetto));

        coccodrillo.x+=flusso.direzione;
        
        int delay=fdelay(flusso.speed);

        sleep(1);
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
