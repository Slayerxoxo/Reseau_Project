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
unsigned int roomNumber;							// Le nombre de parties en cours
Game* rooms[MAX_ROOM_NUMBER] = {NULL};				// Les parties
sfThread* threads[MAX_ROOM_NUMBER] = {NULL};		// Les threads gérant les parties
sfMutex* roomsMutex[MAX_ROOM_NUMBER];				// Les mutex utilisés pour modifier la éléments de la liste des parties
sfMutex* threadsMutex;								// Le mutex utilisé pour modifier la liste des threads
unsigned int playersPerRoom;						// Le nombre de joueurs par partie

//--------------------------------------------------
//		Fonction main
//--------------------------------------------------
int main(int argc, char **argv) {
	sfSocketUDP* socketListen = sfSocketUDP_Create();		// La socket écoutant les demandes de connexion
	char receptionBuffer[128];								// Le buffer réceptionnant les messages reçus
	size_t* received = NULL;								// La taille des messages reçus
	sfIPAddress* sender = NULL;								// L'adresse de l'émetteur des messages reçus
	unsigned short* port = NULL;							// Le port sur lequel le message reçu a été envoyé
	
	int givenRoom;											// L'indice de la partie attribuée au nouveau client
	
	sfSocketUDP* socketRespond = sfSocketUDP_Create();		// La socket utilisée pour répondre au client
	
	int i;													// Une variable utilisée pour les parcours de boucles
	
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
	if(!sfSocketUDP_Bind(socketListen,5000)) {
		perror("erreur : impossible d'affecter le port 5000 à la socket du serveur.\n");
		exit(1);
	} else {
		printf("Socket d'écoute des demandes de connexion du serveur liée au port 5000.\n");
	}

	roomNumber = 0;
	// Création des mutex
	for(i = 0; i< maxRooms; i++) {
		roomsMutex[i] = sfMutex_Create();
	}
	threadsMutex = sfMutex_Create();

	//-------------------------------------------------------------------------
	//					Boucle principale du programme
	//-------------------------------------------------------------------------
	for(;;) {	
		// Ecoute des demandes de connexion
		if(sfSocketUDP_Receive(socketListen, receptionBuffer, sizeof(receptionBuffer), received, sender, port) != sfSocketDone)
		{
			perror("erreur : impossible d'établir la connexion avec le client pour recevoir les messages.\n");
		}
		printf("----------Réception----------\n");
		printf("Message: %s\n",receptionBuffer);
		// Gestion du nouveau client => dans un autre thread ?
		givenRoom = findRoom();
		if(givenRoom == -1) {
			// Aucune partie n'est disponible
			if(sfSocketUDP_Send(socketRespond, "noRoomAvailable", sizeof("noRoomAvailable"), sfIPAddress_FromString("127.0.0.1"), 5100) != sfSocketDone)
			{
				perror("erreur : impossible d'établir la connexion avec le client pour envoyer la réponse.\n");
			} else {
				printf("Réponse : noRoomAvailable\n");
			}
		} else {
			// Une partie est disponible
			if(sfSocketUDP_Send(socketRespond, "roomAvailable", sizeof("roomAvailable"), sfIPAddress_FromString("127.0.0.1"), 5100) != sfSocketDone)
			{
				perror("erreur : impossible d'établir la connexion avec le client pour envoyer la réponse.\n");
				sfMutex_Unlock(roomsMutex[givenRoom]);
			} else {
				printf("Réponse : roomAvailable\n");
				addPlayer(givenRoom);
				sfMutex_Unlock(roomsMutex[givenRoom]);
			}
		}
	}

	// Fermeture de la socket d'écoute des demandes de connexion
	sfSocketUDP_Destroy(socketListen);
	
	// désallocations diverses
	
	return(EXIT_SUCCESS);
}
