

all: main server_exe client_exe

main:main.o 
	gcc -o main main.o 

main.o:main.c header.h
	gcc -c main.c

server_exe:server.o 
	gcc -o server_exe server.o 

server.o:server.c header.h
	gcc -c server.c

client_exe:client.o 
	gcc -o client_exe client.o 

client.o:client.c header.h
	gcc -c client.c

clean:
	rm -f *.o
	rm -f *~

