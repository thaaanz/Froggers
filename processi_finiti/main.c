#include "utilities.h"
#include "avvia.h"
#include "controllo.h"
#include "menu.h"

int main()
{
    avviancurses(); //inizializzo ncurses
    stampaFrogger(); //stampo la scritta iniziale

    GameAudio audio; //inizializzo la struttura contenente gli audio di gioco
    inizializzaAudio(&audio); //inizializzo l'audio

    riproduciMusica(audio.musica_menu);

    if(menuIniziale(&audio))
    { 
        return 0; //opzione di uscita dal gioco
    };

    do{
        controllo(audio); //avvio il controllo
    }while(restart(&audio)); //check per una nuova partita

    freeAudio(&audio);

    endwin();
}
