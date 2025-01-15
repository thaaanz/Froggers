#include "utilities.h"
#include "fiume.h"
#include "rana.h"
#include "controllo.h"
#include "avvia.h"
#include "coccodrilli.h"

void controllo(int* pipe_fd, int* pipe_inversa)
{
    WINDOW* wgioco=newwin(NLINES, NCOLS, 0, 0);
    Processo rana=avviaRana(pipe_fd, pipe_inversa);
    Processo temp;
    Processo granate[N_GRANATE];

    for(int i=0; i<N_GRANATE; i++)
    {
        granate[i].item.id=NULL;
    }

    Flusso* fiume=avviaFlussi();
    
    Processo  cricca[MAX_COCCODRILLI]; //cricca di coccodrilli
    for(int i=0; i < MAX_COCCODRILLI; i++) //inizializzo tutto a 0 sennò nemmeno crea i processi
    {
        cricca[i].pid=0; 
        cricca[i].item.x=1;
        cricca[i].item.y=1;
        cricca[i].item.dir=0;
    }

    
    avviaCoccodrilli(fiume, cricca, pipe_fd, wgioco);
    flash();
    for(int i=0; i < MAX_COCCODRILLI; i++) //inizializzo tutto a 0 sennò nemmeno crea i processi
    {
        mvprintw(wgioco, cricca[i].item.y, 2, "cock %d", i);
        //wrefresh(wgioco);
        //sleep(3);
    }
    

    close(pipe_fd[1]);
    close(pipe_inversa[0]);

    while(true)
    {
        wclear(wgioco);
        box(wgioco, ACS_VLINE, ACS_HLINE);
        //flash();
        
        stampaGranate(granate, wgioco);
        stampaCoccodrilli(cricca, wgioco);
        stampaRana(rana.item, wgioco);
        //flash();

        //check flussi
        for(int i=0; i<NUMERO_FLUSSI; i++)
        {
            mvwprintw(wgioco, fiume[i].y, 2, "Y:%d, dir:%d, speed:%d", fiume[i].y, fiume[i].dir, fiume[i].speed);
        }

        /*for(int j=0; j < N_GRANATE; j++){
                if(granate[j].item.id!=NULL){
                    //flash();
                    mvwprintw(wgioco, 1, 1, "granata. %d x %d y %d", granate[j].pid, granate[j].item.x, granate[j].item.y);
                    wrefresh(wgioco);
                    usleep(5000);
                }
        }*/

        wrefresh(wgioco);

        read(pipe_fd[0], &temp, sizeof(Processo));
        switch(temp.item.id)
        {
            case 'r':
                rana.item.x+=temp.item.x;
                rana.item.y+=temp.item.y;
                if(rana.item.x<=0) rana.item.x=1;
                if(rana.item.y<=0) rana.item.y=1;
                if(rana.item.x>=NCOLS-LARGHEZZA_RANA) rana.item.x=NCOLS-LARGHEZZA_RANA-1;
                if(rana.item.y>=NLINES-ALTEZZA_RANA) rana.item.y=NLINES-ALTEZZA_RANA-1;
                break;
            case 'q':
                cleanup(); break;
            case 'o':
                int i=(temp.item.y - ALTEZZA_MARCIAPIEDE)/ALTEZZA_FLUSSO;
                if(temp.item.dir==DIR_RIGHT) i*=2;
                granate[i]=temp;
                
                break;
            case 'g':
                
                write(pipe_inversa[1], &rana, sizeof(Processo));
                write(pipe_inversa[1], &rana, sizeof(Processo));
                break;
            case 'c':
                if(temp.item.dir >= 0 && temp.item.dir < MAX_COCCODRILLI) {
                    cricca[temp.item.dir]= temp;
                } //sennò mi dava segmentation fault, anche se mi fa pensare che non inizializzi bene tutti i processi
                else ; //flash();
                /*mvwprintw(wgioco, 10,10,"cod c%d", temp.item.dir); // zio pera gli passa codici a cazzo non lo so
                wrefresh(wgioco);
                sleep(2);*/
                break;

        }

        usleep(DELAY);
    }
}