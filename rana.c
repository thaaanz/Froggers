// sinc non ho idea di cosa bisogna includere qua

int avviaRana(int pipe_fd[2]){
    pid_t pid;

    if( (pid=fork() == -1)){
        perror("fork rana fallita"); //ciao
        exit(1); //o _exit?
    }

    if(pid == 0){
        movimentoRana(pipe_fd);
        //exit(0); ????
    }
    
    return pid; // in modo che poi si possa fare la wait, da capire dove lo salviamo
}

void movimentoRana(int pipe[2]){

    close(pipe[0]);

    Pos ogg;
    ogg.x=X_INIZIALE_RANA;
    ogg.y=Y_INIZIALE_RANA;
    ogg.tipo=TIPO_RANA; //qua io definirei delle costanti in qualche header però si può anche fare senza
    
    write(pipe[1], &ogg, sizeof(ogg)); // manda posizione iniziale
    _Bool cambio=false; // per non mandare 300mila robe inutili alla pipe sennò rip la mia cpu
    char c;
    //loop principale per lettura input e l'invio dei movimenti
    while(1){
        cambio=false;
        c=getch();
        ogg.tipo=TIPO_RANA;
        //qua cè da decidere se memorizzare la posizione della rana in questo processo e mandarla ogni volta al controllo
        //oppure mandare solo la variazione e far gestire la variazione all'altro processo
        switch (c) {
        	case KEY_UP:
				ogg.y-=DIM_Y_RANA; 
            	cambio=true;
            break;
          case KEY_DOWN:
            	ogg.y+=DIM_Y_RANA;
            	cambio=true;
            break;
          case KEY_LEFT:
            	ogg.x-=DIM_X_RANA;
            	cambio=true;  
            break;
          case KEY_RIGHT:
            	ogg.x+=DIM_X_RANA;
            	cambio=true;
            break;
          case 32: // spazio
          	    ogg.tipo=TIPO_PROIETTILE; //da mandare al controllo così crea il processo per i proiettili
                cambio=true;
            break;
          default:
          	break;
        }

        if (cambio){
            write(pipe[1], &ogg, sizeof(ogg));
            //a seconda di come gestiamo le coordinate bisogna resettare x e y di ogg 
            usleep(); // da decidere il delay
        }
    }
}
