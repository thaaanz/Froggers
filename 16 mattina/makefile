CC = gcc
CFLAGS = -c -Wall -g
LDFLAGS = -lncurses
OBJS = rana.o main.o controllo.o fiume.o avvia.o coccodrilli.o

a.out: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o a.out

main.o: main.c rana.h avvia.h controllo.h fiume.h coccodrilli.h
	$(CC) $(CFLAGS) main.c -o main.o

rana.o: rana.c rana.h
	$(CC) $(CFLAGS) rana.c -o rana.o

controllo.o: controllo.c controllo.h rana.h coccodrilli.h
	$(CC) $(CFLAGS) controllo.c -o controllo.o

fiume.o: fiume.c fiume.h rana.h
	$(CC) $(CFLAGS) fiume.c -o fiume.o

avvia.o: avvia.c avvia.h rana.h
	$(CC) $(CFLAGS) avvia.c -o avvia.o

coccodrilli.o: coccodrilli.c coccodrilli.h
	$(CC) $(CFLAGS) coccodrilli.c -o coccodrilli.o

clean:
	rm -f *.o a.out
