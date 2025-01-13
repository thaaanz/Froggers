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

void stampaCoccodrillo(Oggetto coccodrillo, WINDOW* window, int n) 
{ for (int i = 0; i < ALTEZZA_COCCO; i++) { 
    for (int j = 0; j < LARGHEZZA_COCCO; j++) {
        int y = coccodrillo.y + i; 
        int x = coccodrillo.x + j; 
        
        mvwaddch(window, y, x, (char)n); 
        
    } 
} 
    wrefresh(window);
}

void stampaCoccodrilli(Processo * coccodrilli, WINDOW* window){
    
for(int i=0; i < NUMERO_FLUSSI; i++){
    for(int j=0; j < MAX_COCCODRILLI; j++)
    {
        stampaCoccodrillo(coccodrilli[i][j].item, window, i);
        
    }
}

}



/*void avviaCoccodrilli(Flusso* fiume, Processo* coccodrilli, int pipe_fd[])
{
    pid_t temp;
    for(int i=0; i < MAX_COCCODRILLI; i++)
    {
        temp=fork();
        if(temp<0)
        {
            perror("fork cocco");
            exit(18);
        }
        else if(temp==0)
        {
            coccodrilli[i].pid=getpid();
            coccodrillo(fiume[i], pipe_fd);
            exit(1);
        }
        usleep(20000);
    }
}*/

void avviaCoccodrilli(Flusso* fiume, Processo* coccodrilli[], int pipe_fd[], WINDOW * w) {
    pid_t temp;


    for(int i = 0; i < NUMERO_FLUSSI; i++) {
        for(int j=0; j< MAX_COCCODRILLI; j++){
            temp = fork();
            if(temp < 0) {
                perror("fork cocco");
                exit(18);
            }
            else if(temp == 0) {
                // Processo figlio
                coccodrilli[i][j].pid = getpid();
                coccodrillo(fiume[i], pipe_fd);
                exit(1);
            }
            
            usleep(rand() % 500000);
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
