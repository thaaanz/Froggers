#include "utilities.h"
#include "fiume.h"
#include "rana.h"
#include "controllo.h"
#include "avvia.h"
#include "coccodrilli.h"

void controllo(int* pipe_fd, int* pipe_inversa)
{
    WINDOW* wgioco=newwin(NLINES, NCOLS, HUDLINES, 0);
    WINDOW* whud=newwin(HUDLINES, NCOLS, 0, 0);
    WINDOW* debug=newwin(20, 20, 0, NCOLS);

    Flusso* fiume=avviaFlussi();
    Processo rana=avviaRana(pipe_fd, pipe_inversa);
    Processo cricca[NUMERO_FLUSSI*MAX_COCCODRILLI];//cricca di coccodrilli
    Processo granate[N_GRANATE];
    Processo astuccio[N_PROIETTILI]; //astuccio dei proiettili
    Processo temp;
    //_Bool tane[NUMERO_TANE]={0};

    temp.pid=0;

    avviaCoccodrilli(pipe_fd, fiume, cricca); 
    avviaProiettili(astuccio);

    for(int i=0; i<N_GRANATE; i++)
    {
        granate[i].item.id=NULL;
    }

    close(pipe_fd[1]);
    close(pipe_inversa[0]);

    int vite=MAX_VITE;
    time_t start=time(NULL);    

    while(vite>0)
    {
        //?PERCHÈ È BOOL????
        //detectCollisione(&rana, cricca, granate, temp, &vite, debug, tane);
        
        werase(wgioco);
        box(wgioco, ACS_VLINE, ACS_HLINE);
        box(whud, ACS_VLINE, ACS_HLINE);

        mvwprintw(whud, 1, 1, "time: %d - vite: %d", time(NULL)-start, vite);

        //stampaTane(wgioco, tane);
        //stampaMarciapiede(wgioco);
        stampaCoccodrilli(cricca, wgioco);
        stampaRana(rana.item, wgioco);

        for(int i=0; i<N_GRANATE; i++)
        {
            if(granate[i].item.id!=0) //* meglio fare il controllo sul pid ma dovremmo memorizzarlo
            {
                stampaGranata(granate[i].item, wgioco);
            }
        }

        for(int i=0; i<N_PROIETTILI; i++)
        {
            if(astuccio[i].pid!=-1)
            {
                stampaProiettile(astuccio[i].item, wgioco);
            }
        }

        //check flussi
        for(int i=0; i<NUMERO_FLUSSI; i++)
        {
            mvwprintw(wgioco, fiume[i].y, 2, "Y:%d, dir:%d, speed:%d", fiume[i].y, fiume[i].dir, fiume[i].speed);
        }

        int c=0;
        for(int i=0; i<N_PROIETTILI; i++)
        {
            if(astuccio[i].pid!=-1) c++;
        }
        mvwprintw(debug, 1, 1, "validi: %d", c);

        wnoutrefresh(wgioco);
        wnoutrefresh(debug);
        wnoutrefresh(whud);
        doupdate();

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
            case 'g': //scrive due volte perchè sono due granate
                write(pipe_inversa[1], &rana, sizeof(Processo));
                write(pipe_inversa[1], &rana, sizeof(Processo));
                break;
            case 'c':
                for(int i=0; i<NUMERO_FLUSSI*MAX_COCCODRILLI; i++)
                {
                    if(cricca[i].pid==temp.pid)
                    {
                        cricca[i]=temp;
                    }
                }
                break;
            case '-':
                if(temp.item.dir==0)
                {
                    for(int i = 0; i < N_PROIETTILI; i++)
                    {
                        if(astuccio[i].pid==temp.pid)
                        {
                            kill(astuccio[i].pid, 9);
                            astuccio[i].pid=-1;
                            break;
                        }
                    }
                }
                _Bool trovato = false;
                for(int i = 0; i < N_PROIETTILI; i++) 
                {
                    if(astuccio[i].pid == temp.pid) {
                        astuccio[i] = temp;
                        trovato = 1;
                        break;
                    }
                }
                if(!trovato) 
                {
                    for(int i = 0; i < N_PROIETTILI; i++) 
                    {
                        if(astuccio[i].pid<0) {  // slot libero
                            astuccio[i] = temp;
                            trovato=true;
                            break;
                        }
                    }
                }
                if(!trovato)
                {
                    kill(temp.pid, 9);
                }
                break;
        }

        usleep(DELAY/100);
    }
}
