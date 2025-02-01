CC = gcc
CFLAGS = -c -Wall -g
LDFLAGS = -lncurses -lpthread
OBJS = main.o controllo.o buffer.o avvia.o coccodrilli.o sfondo.o

a.out: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o a.out

main.o: main.c controllo.h avvia.h
	$(CC) $(CFLAGS) main.c -o main.o

controllo.o: controllo.c controllo.h 
	$(CC) $(CFLAGS) controllo.c -o controllo.o

buffer.o: buffer.c buffer.h 
	$(CC) $(CFLAGS) buffer.c -o buffer.o

avvia.o: avvia.c avvia.h 
	$(CC) $(CFLAGS) avvia.c -o avvia.o

coccodrilli.o: coccodrilli.c coccodrilli.h
	$(CC) $(CFLAGS) coccodrilli.c -o coccodrilli.o

sfondo.o: sfondo.c sfondo.h
	$(CC) $(CFLAGS) sfondo.c -o sfondo.o

clean:
	rm -f *.o a.out