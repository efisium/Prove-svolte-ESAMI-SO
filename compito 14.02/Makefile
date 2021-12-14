
all: exe

exe:main.o procedure.o semafori.o
	gcc -o exe main.o procedure.o semafori.o

main.o:main.c header.h
	gcc -c main.c

procedure.o:procedure.c header.h semafori.h
	gcc -c procedure.c

semafori.o:semafori.c semafori.h
	gcc -c semafori.c

clean:
	rm -f *.o
	rm -f *~
	rm -f ./start
