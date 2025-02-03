CC = gcc
CFLAGS = -c -Wall -g
LDFLAGS = -lncurses -lpthread
OBJS =rana.o main.o controllo.o buffer.o campo.o avvia.o coccodrilli.o punteggio.o menu.o

a.out: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o a.out

main.o: main.c 
	$(CC) $(CFLAGS) main.c -o main.o

rana.o: rana.c rana.h
	$(CC) $(CFLAGS) rana.c -o rana.o

controllo.o: controllo.c controllo.h  
	$(CC) $(CFLAGS) controllo.c -o controllo.o

buffer.o: buffer.c buffer.h 
	$(CC) $(CFLAGS) buffer.c -o buffer.o

campo.o: campo.c campo.h
	$(CC) $(CFLAGS) campo.c -o campo.o

avvia.o: avvia.c avvia.h 
	$(CC) $(CFLAGS) avvia.c -o avvia.o

coccodrilli.o: coccodrilli.c coccodrilli.h
	$(CC) $(CFLAGS) coccodrilli.c -o coccodrilli.o

punteggio.o: punteggio.c punteggio.h
	$(CC) $(CFLAGS) punteggio.c -o punteggio.o

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) menu.c -o menu.o
clean:
	rm -f *.o a.out
