a.out: main.o ok.o
	gcc main.o ok.o rana.o -lncurses

main.o: prova_header.c
	gcc -c prova_header.c -o main.o

rana.o: rana.c rana.h
	gcc -c rana.c -o rana.o

ok.o: ok.c ok.h
	gcc -c ok.c 

clean:
	rm -f *.o