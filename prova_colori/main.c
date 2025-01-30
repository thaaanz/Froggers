#include "utilities.h"
#include "avvia.h"
#include "controllo.h"
#include "menu.h"

int  main()
{
    avviancurses();
    if(menuIniziale()){
        return 0;
    };

    
    do{
        int pipe_fd[2];
        int pipe_inversa[2];
        avviaPipe(pipe_fd);
        avviaPipe(pipe_inversa);

        controllo(pipe_fd, pipe_inversa);
    }while(restart());


    endwin();
}
