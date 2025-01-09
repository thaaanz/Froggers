#include "main.c"

void avviaPipe(int pipe_fd[2]){

    if (pipe(pipe_fd) == -1){
		perror("Pipe fallita");
		exit(1);
	}
}

void avviancurses(){
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
}

void avviaGioco(int pipe_fd[2]){

    close(pipe_fd[1]);
    avviaRana(pipe_fd);

    //loop principale
    while(vite > 0){ //da capire dove salvare queste vite, idealmente in una strttura
        lettura(pipe_fd[0]); // anche qua bisogna vedere in che strttura salvare le cose lette dalla pipe

        detectCollsione(flagCollisione); //= true se trova collisioni

        if(flagCollisione){
            gestisciCollisione(); // da passare la matrice in modo che gestisca
        }
        else{
            aggiornaSchermo();
        }

        stampaInfoGame(); //stampa di vite tempo etc, da aggiornare a ogni ciclo oppure solo quando cambia?(una volta al secondo)

        gestioneTana(); // se si è arrivati alla tana bisogna cancellare tutto lo schermo e far ripartire i processi
    
        gestioneCock(); //controlla se sono morti coccodrilli e se c'è spazio ne spawna altri, memorizza id velocità etc nella strtuura principale(?)
    }

}

void avviaRana(int pipe[2]){
    pid_t pid;
    pid=fork();
    if (pid < 0){
        perror("fork fallita");
    }
    else if(pid == 0){
        movimentoRana(pipe);
    }
    return;
}