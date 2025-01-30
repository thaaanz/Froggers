CC = gcc
CFLAGS = -c -Wall -g
LDFLAGS = -lncurses -lpthread
OBJS =rana.o main.o controllo.o buffer.o campo.o avvia.o

a.out: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o a.out

main.o: main.c rana.h  controllo.h buffer.h
	$(CC) $(CFLAGS) main.c -o main.o

rana.o: rana.c rana.h
	$(CC) $(CFLAGS) rana.c -o rana.o

controllo.o: controllo.c controllo.h rana.h 
	$(CC) $(CFLAGS) controllo.c -o controllo.o

buffer.o: buffer.c buffer.h 
	$(CC) $(CFLAGS) buffer.c -o buffer.o

campo.o: campo.c campo.h
	$(CC) $(CFLAGS) campo.c -o campo.o

avvia.o: avvia.c avvia.h 
	$(CC) $(CFLAGS) avvia.c -o avvia.o
clean:
	rm -f *.o a.out
