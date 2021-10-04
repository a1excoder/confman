MAINFDIR = ./src/main.c
ENDDIR = ./example/main


buildlib:
	gcc -c ./lib/confman.c
	ar r ./lib/libconfman.a confman.o
	rm confman.o

buildmain:
	gcc -c ./src/main.c
	gcc -o ./example/main main.o -L./lib -lconfman