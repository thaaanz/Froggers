#include "utilities.h"
#include "rana.h"

const char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA]={
    " 0-0 ",
    "(___)"
};

const Processo quit={NULL, {'q', 0, 0, NULL}};
const Processo spara={NULL, {'g', 0, 0, NULL}};

Processo avviaRana(int* pipe_fd, int* pipe_inversa)
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
    Processo rana={pid, {'r', NLINES- ALTEZZA_RANA-1, NCOLS/2, NULL }};
    return rana;
}

void funzioneRana(int* pipe_fd, int* pipe_inversa)
{
    close(pipe_fd[0]);
    Processo rana;
    int c;

    rana.pid=getpid();
    while(true)
    {
        rana.item.id='r'; rana.item.y=0; rana.item.x=0, rana.item.dir=NULL;
        c=(int)getch();

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
                break;
        }

        write(pipe_fd[1], &rana, sizeof(Processo));
        usleep(DELAY);

    }
}

void stampaRana(Oggetto r, WINDOW* w)
{
    for (int i = 0; i < ALTEZZA_RANA; i++) 
    {
        for( int j=0; j < LARGHEZZA_RANA; j++)
        {
            mvwaddch(w, r.y+i, r.x+j,  spriteRana[i][j]);
        }
    }
}

void avviaGranate(int* pipe_fd, int* pipe_inversa)
{
    pid_t pid_left=fork();
    if(pid_left<0)
    {
        perror("fork granata fallita");
    }
    else if(pid_left==0)
    {
        granata(DIR_LEFT, pipe_fd, pipe_inversa);
        exit(3);
    }

    pid_t pid_right=fork();
    if(pid_right<0)
    {
        perror("fork granata fallita");
    }
    else if(pid_right==0)
    {
        granata(DIR_RIGHT, pipe_fd, pipe_inversa);
        exit(3);
    }
}

void granata(int direzione, int* pipe_fd, int* pipe_inversa)
{
    close(pipe_inversa[1]);
    Processo granata, temp;
    read(pipe_inversa[0], &temp, sizeof(Processo));//coordinate della rana
    granata.item.id='o';
    granata.item.y=temp.item.y+(ALTEZZA_RANA/2);
    granata.item.x=temp.item.x+(direzione==DIR_RIGHT? LARGHEZZA_RANA : 0);

    while(granata.item.x>0 && granata.item.x<NCOLS)
    {
        granata.item.x+=direzione;
        write(pipe_fd[1], &granata, sizeof(Processo));
        usleep(DELAY);
    }
}

void stampaGranate(Processo* g, WINDOW* w)
{
    for(int i=0; i<N_GRANATE; i++)
    {
        if(g[i].item.id!=0)
        {
         mvwaddch(w, g[i].item.y, g[i].item.x, g[i].item.id);
        }
    }
}