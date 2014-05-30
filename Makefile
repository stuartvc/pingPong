all : start display player

start : start.o
	gcc -o start start.o

start.o : start.c
	gcc -c start.c

display : display.o message.o
	gcc -o display display.o message.o -lncurses

display.o : display.c
	gcc -c display.c

player : player.o message.o
	gcc -o player player.o message.o

player.o : player.c
	gcc -c player.c

message.o : message.c message.h
	gcc -c message.c

clean : 
	rm -Rf *.o
	rm start display player
