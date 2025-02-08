#include "utilities.h"
#include "avvia.h"
#include "controllo.h"
#include "menu.h"

int main()
{
    avviancurses(); //inizializzo ncurses

    if(menuIniziale())
    { 
        return 0; //opzione di uscita dal gioco
    };

    do{
        controllo(); //avvio il controllo
    }while(restart()); //check per una nuova partita

    endwin();
}
