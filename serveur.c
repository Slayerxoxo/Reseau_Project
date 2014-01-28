/*----------------------------------------------
Serveur à lancer avant le client
------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <SFML/Network.h>

#define MAX_ROOM_NUMBER 100

int main(int argc, char **argv) {
	unsigned int maxRooms;
	unsigned int playersPerRoom;

	sfSocketUDP* socket = sfSocketUDP_Create();
	char receptionBuffer[128];
	size_t* received = NULL;
	sfIPAddress* sender = NULL;
	unsigned short* port = NULL;
	
	// Traitement des paramètres passés par l'utilisateur
	if(argc != 3) {
		perror("usage : serveur <nombre-max-de-salles(0-100)> <nombre-de-joueurs-par-salle(1-4)>\n");
		exit(1);
    }
    maxRooms = atoi(argv[1]);
    playersPerRoom = atoi(argv[2]);
    if(maxRooms <= 0 || maxRooms > MAX_ROOM_NUMBER || playersPerRoom <= 0 || playersPerRoom > 4) {
		perror("usage : serveur <nombre-max-de-salles(0-100)> <nombre-de-joueurs-par-salle(1-4)>\n");
		exit(1);
    } else {
    	printf("Ce serveur gère au maximum %i parties simultanées, avec %i joueurs par partie.\n",maxRooms, playersPerRoom);
	}
	
	// Liaison de la socket d'écoute des demandes de connexion au port 5000
	if(!sfSocketUDP_Bind(socket,5000)) {
		perror("erreur : impossible d'affecter le port 5000 à la socket du serveur.\n");
		exit(1);
	} else {
		printf("Socket d'écoute des demandes de connexion du serveur liée au port 5000.\n");
	}

	//-------------------------------------------------------------------------
	//					Boucle principale du programme
	//-------------------------------------------------------------------------
	for(;;) {
		// Ecoute des demandes de connexion
		if(sfSocketUDP_Receive(socket, receptionBuffer, sizeof(receptionBuffer), received, sender, port) != sfSocketDone)
		{
			perror("erreur : impossible d'établir la connexion avec le client.\n");
			exit(1);
		}
		
		
	}

	// Fermeture de la socket d'écoute des demandes de connexion
	sfSocketUDP_Destroy(socket);
	
	return(EXIT_SUCCESS);
}
