#include "utilities.h"

const char spriteCoccodrillo[ALTEZZA_COCCODRILLO][LARGHEZZA_COCCODRILLO]={
    "---------<",
    "-W-W-W-W- "
};

void stampaCoccodrillo(Oggetto coccodrillo, WINDOW* window, int n) 
{ for (int i = 0; i < ALTEZZA_COCCODRILLO; i++) { 
    for (int j = 0; j < LARGHEZZA_COCCODRILLO; j++) {
        int y = coccodrillo.y + i; 
        int x = coccodrillo.x + j; 
        
        mvwaddch(window, y, x, spriteCoccodrillo[i][j]); 
    } 
} 
    wrefresh(window);
}

void stampaCoccodrilli(Processo * coccodrilli, WINDOW* window){
    

    for(int j=0; j < MAX_COCCODRILLI; j++)
    {
        if (coccodrilli[j].pid != 0)
            stampaCoccodrillo(coccodrilli[j].item, window, j);
        
    }

}

void avviaCoccodrilli(Flusso* fiume, Processo* coccodrilli, int* pipe_fd) {
    pid_t temp;

    while(1){
        int i=0;
        if(coccodrilli[i].pid == 0){
            temp = fork();
            if(temp < 0) {
                perror("fork cocco");
                exit(18);
            }
            else if(temp == 0) {
                // Processo figlio
                coccodrilli[i].pid = getpid();
                coccodrilli[i].item.y=fiume[i].y;
                coccodrillo(fiume[i % NUMERO_FLUSSI], pipe_fd, i);
                exit(1);
            }
        }
        i++;
        i=i%MAX_COCCODRILLI;
        sleep(3);
    }
}


void coccodrillo(Flusso flusso, int pipe_fd[], int j)
{
    close(pipe_fd[0]);
    Oggetto coccodrillo={'c', flusso.y+1, flusso.dir==DIR_RIGHT ? 1 : NCOLS};

    while(coccodrillo.x > 1- LARGHEZZA_COCCODRILLO && coccodrillo.x < NCOLS + LARGHEZZA_COCCODRILLO)
    {
        write(pipe_fd[1], &coccodrillo, sizeof(Oggetto));

        coccodrillo.x+=flusso.dir;
        
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