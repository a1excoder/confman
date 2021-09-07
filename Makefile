MAINFDIR = ./src/main.c
ENDDIR = ./example/main

build:
	gcc $(MAINFDIR) -o $(ENDDIR)