/*----------------------------------------------
Serveur à lancer avant le client
------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <SFML/Network.h>
#include <SFML/System.h>
#include "serverfunctions.h"
#include "types.h"
#include "def.h"

//--------------------------------------------------
//		Variabels globales
//--------------------------------------------------
unsigned int maxRooms;								// Le nombre maximum de parties simultanées
Game* rooms[MAX_ROOM_NUMBER] = {NULL};				// Les parties
sfMutex* roomsMutex;								// Le mutex utilisé pour modifier la liste des parties
unsigned int playersPerRoom;						// Le nombre de joueurs par partie

//--------------------------------------------------
//		Fonction main
//--------------------------------------------------
int main(int argc, char **argv) {
	sfSocketUDP* socket = sfSocketUDP_Create();		// La socket écoutant les demandes de connexion
	char receptionBuffer[128];						// Le buffer réceptionnant les messages reçus
	size_t* received = NULL;						// La taille des messages reçus
	sfIPAddress* sender = NULL;						// L'adresse de l'émetteur des messages reçus
	unsigned short* port = NULL;					// Le port sur lequel le message reçu a été envoyé
	
	// Traitement des paramètres passés par l'utilisateur
	if(argc != 3) {
		perror("usage : serveur <nombre-max-de-salles(1-100)> <nombre-de-joueurs-par-salle(2-4)>\n");
		exit(1);
    }
    maxRooms = atoi(argv[1]);
    playersPerRoom = atoi(argv[2]);
    if(maxRooms <= 0 || maxRooms > MAX_ROOM_NUMBER || playersPerRoom <= 1 || playersPerRoom > MAX_PLAYER_NUMBER) {
		perror("usage : serveur <nombre-max-de-salles(1-100)> <nombre-de-joueurs-par-salle(2-4)>\n");
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

	// Création du mutex utilisé pour gérer la liste des parties	
	roomsMutex = sfMutex_Create();

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
