SRC = $(wildcard src/*.c)
main: main.o debug.o chunk.o value.o
	gcc -o main main.o debug.o chunk.o value.o

main.o: 
	gcc -c main.c

debug.o:
	gcc -c debug.c

chunk.o:
	gcc -c chunk.c 
