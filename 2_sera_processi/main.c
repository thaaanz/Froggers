#include "utilities.h"
#include "avvia.h"
#include "controllo.h"
#include "menu.h"

int  main()
{
    avviancurses();
    if(menuIniziale()){ 
        return 0; // non si vuole giocare
    };

    
    do{
        controllo();
    }while(restart());


    endwin();
}
