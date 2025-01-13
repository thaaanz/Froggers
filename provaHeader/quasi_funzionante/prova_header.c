
#include "ok.h"
#include "rana.h"
#include "coccodrillo.h"
#include "utilities.h"

#define NLINES 50
#define NCOLS 60

WINDOW* wgioco;

void controllo(int pipe_fd[]);

void cleanupGame( Processo* coccodrilli);

int main()
{
    int pipe_fd[2];
    avviaPipe(pipe_fd);
    avviancurses();

    avviaRana(pipe_fd);
    

    controllo(pipe_fd);

    endwin();
}



void controllo(int pipe_fd[]) 
{
    int* open_pipe=pipe_fd;
    

    wgioco=newwin(NLINES, NCOLS, 0, 0);

    Oggetto ranocchia={'r', NLINES-ALTEZZA_RANA-1, NCOLS/2, NULL};
    Oggetto temp, proiettile;

    Flusso* fiume=avviaFlussi();
    Processo coccodrilli[MAX_COCCODRILLI];


    
    mvwprintw(wgioco, NLINES-1, 0, "Cocco pos");
    wrefresh(wgioco);
    sleep(2);


    
        for(int j=0; j < MAX_COCCODRILLI; j++){
            coccodrilli[j].pid=0;
            coccodrilli[j].item.id='c';
            coccodrilli[j].item.y=0;
            coccodrilli[j].item.x=0;
        }


    avviaCoccodrilli(fiume, coccodrilli, pipe_fd);
    close(pipe_fd[1]);

    while(true)
    {
        wclear(wgioco);
        box(wgioco, ACS_VLINE, ACS_HLINE);

        stampaRana(ranocchia, wgioco);
        stampaCoccodrilli(coccodrilli, wgioco);

        for(int i=0; i<NUMERO_FLUSSI; i++)
        {
            mvwprintw(wgioco, fiume[i].y, 2, "%d  direzione: %d velocità: %d",fiume[i].y, fiume[i].direzione, fiume[i].speed);
        }

        wrefresh(wgioco);
        
        if(read(pipe_fd[0], &temp, sizeof(Oggetto)) == -1){
            perror("lettura fallita");
            exit(89);
        }
        switch(temp.id)
        {
            case 'r':
                ranocchia.x+=temp.x;
                ranocchia.y+=temp.y;
                if(ranocchia.x<=0) ranocchia.x=1;
                if(ranocchia.y<=0) ranocchia.y=1;
                if(ranocchia.x>=NCOLS-LARGHEZZA_RANA) ranocchia.x=NCOLS-LARGHEZZA_RANA - 1;
                if(ranocchia.y>=NLINES-ALTEZZA_RANA) ranocchia.y=NLINES-ALTEZZA_RANA -1 ;
                break;
            case 'q':
                cleanupGame( coccodrilli); 
                
                break;
            case 'c':
                int i=(temp.y-fiume[0].y)/4;
                if(i >=0 && i < MAX_COCCODRILLI)
                    coccodrilli[temp.cod].item=temp;
                mvwprintw(wgioco, NLINES-1, 0, "Cocco pos: %d,%d", temp.x, temp.y);
                
                break;
        }
        
        usleep(DELAY);
    }
}



void cleanupGame(  Processo* coccodrilli)
{
    endwin();
    for (int i=0; i < MAX_COCCODRILLI; i++)
        kill(coccodrilli[i].pid,9);
    

}
