CC = gcc
CFLAGS = -c -Wall
LDFLAGS = -lncurses
OBJS = rana.o main.o controllo.o fiume.o avvia.o

a.out: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o a.out

main.o: main.c rana.h avvia.h controllo.h fiume.h
	$(CC) $(CFLAGS) main.c -o main.o

rana.o: rana.c rana.h
	$(CC) $(CFLAGS) rana.c -o rana.o

controllo.o: controllo.c controllo.h rana.h
	$(CC) $(CFLAGS) controllo.c -o controllo.o

fiume.o: fiume.c fiume.h rana.h
	$(CC) $(CFLAGS) fiume.c -o fiume.o

avvia.o: avvia.c avvia.h rana.h
	$(CC) $(CFLAGS) avvia.c -o avvia.o

clean:
	rm -f *.o a.out
