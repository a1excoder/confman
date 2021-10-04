MAINFDIR = ./src/main.c
ENDDIR = ./example/main


buildlib:
	gcc -c ./lib/confman.c
	ar r ./lib/libconfman.a confman.o
	rm confman.o

buildmain:
	gcc -c $(MAINFDIR)
	gcc -o $(ENDDIR) main.o -L./lib -lconfman
