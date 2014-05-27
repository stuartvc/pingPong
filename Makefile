all : start display player

start : start.o
	gcc -o start start.o

start.o : start.c
	gcc -c start.c

display : display.o
	gcc -o display display.o

display.o : display.c
	gcc -c display.c

player : player.o
	gcc -o player player.o

player.o : player.c
	gcc -c player.c

clean : 
	rm -Rf *.o
	rm start display player
