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
sfThread* threads[MAX_ROOM_NUMBER] = {NULL};		// Les threads gérant les parties
sfMutex* roomsMutex[MAX_ROOM_NUMBER];				// Les mutex utilisés pour modifier la éléments de la liste des parties
unsigned int playersPerRoom;						// Le nombre de joueurs par partie

//--------------------------------------------------
//		Fonction main
//--------------------------------------------------
int main(int argc, char **argv) {
	sfSocketUDP* socketListen = NULL;						// La socket écoutant les demandes de connexion
	char receptionBuffer[128];								// Le buffer réceptionnant les messages reçus
	size_t* received = NULL;								// La taille des messages reçus
	sfIPAddress sender;										// L'adresse de l'émetteur des messages reçus
	unsigned short* port = NULL;							// Le port sur lequel le message reçu a été envoyé
	
	int givenRoom;											// L'indice de la partie attribuée au nouveau client
	
	sfSocketUDP* socketRespond = NULL;						// La socket utilisée pour répondre au client
	char givenGame[4];										// Le numéro de la partie allouée au client
	char clientPseudo[7];									// Le pseudo du client
	char sendBuffer[512];									// Le buffer servant àrépondre au client
	
	int i;													// Une variable utilisée pour les parcours de boucles
	
	socketListen = sfSocketUDP_Create();
	socketRespond = sfSocketUDP_Create();
	
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

	// Création des mutex
	for(i = 0; i< maxRooms; i++) {
		roomsMutex[i] = sfMutex_Create();
	}

	//-------------------------------------------------------------------------
	//					Boucle principale du programme
	//-------------------------------------------------------------------------
	for(;;) {	
		// Ecoute des demandes de connexion
		if(sfSocketUDP_Receive(socketListen, receptionBuffer, sizeof(receptionBuffer), received, &sender, port) != sfSocketDone)
		{
			perror("erreur : impossible d'établir la connexion avec le client pour recevoir les demandes de connexion.\n");
		}
		printf("----------Réception----------\n");
		printf("Message: %s\n",receptionBuffer);
		// Gestion du nouveau client => dans un autre thread ?
		givenRoom = findRoom();
		if(givenRoom == -1) {
			// Aucune partie n'est disponible
			if(sfSocketUDP_Send(socketRespond, "noRoomAvailable", sizeof("noRoomAvailable"), sender, 5100) != sfSocketDone)
			{
				perror("erreur : impossible d'établir la connexion avec le client pour envoyer la réponse.\n");
			} else {
				printf("Réponse : noRoomAvailable\n");
			}
		} else {
			// Une partie est disponible
			snprintf(givenGame, 4, "%d", givenRoom);
			snprintf(clientPseudo, 7, "%s", receptionBuffer);
			snprintf(sendBuffer, 512, "%s/%s/%d", givenGame, clientPseudo, rooms[givenRoom]->playerNumber+1);
			if(sfSocketUDP_Send(socketRespond, sendBuffer, sizeof(sendBuffer), sender, 5100) != sfSocketDone)
			{
				perror("erreur : impossible d'établir la connexion avec le client pour envoyer la réponse.\n");
				sfMutex_Unlock(roomsMutex[givenRoom]);
			} else {
				printf("Réponse : %s\n", sendBuffer);
				addPlayer(givenRoom, sender);
				sfMutex_Unlock(roomsMutex[givenRoom]);
			}
		}
	}

	// Fermeture de la socket d'écoute des demandes de connexion
	sfSocketUDP_Destroy(socketListen);
	
	// désallocations diverses
	
	return(EXIT_SUCCESS);
}
