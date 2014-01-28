all: client serveur

client: client.c
	gcc -Wall -o client $^ -lcsfml-graphics -lcsfml-window -lcsfml-system

serveur: serveur.c serverfunctions.c serverfunctions.h types.h
	gcc -Wall -o serveur $^ -lcsfml-network -lcsfml-system
