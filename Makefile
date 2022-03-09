# -*- MakeFile -*-

main: theater.o library.o
	gcc theater.o library.o -o main

theater.o: theater.c
	gcc -c theater.c

library.o: library.c library.h
	gcc -c library.c

