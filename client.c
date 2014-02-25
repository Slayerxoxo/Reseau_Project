/*-----------------------------------------------------------
Client a lancer apres le serveur avec la commande :
client <adresse-serveur> <message-a-transmettre>
------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "graphic.h"
#include "clientfunctions.h"
#include "def.h"
#include "types.h"

#include <SFML/Network.h>
#include <SFML/System.h>

/* Déclarations */
sfRenderWindow* fenetre;

typedef struct sockaddr 	sockaddr;
typedef struct sockaddr_in 	sockaddr_in;
typedef struct hostent 		hostent;
typedef struct servent 		servent;

int main(int argc, char **argv) {

	sfSocketUDP* socket = sfSocketUDP_Create();			// création de la socket
	char sendBuffer[7];									// mess à envoyer
	char receptionBuffer[128];							// Le buffer réceptionnant les messages reçus
	size_t* received = NULL;							// La taille des messages reçus
	sfIPAddress* sender = NULL;							// L'adresse de l'émetteur des messages reçus
	unsigned short* port = NULL;						// Le port sur lequel le message reçu a été envoyé
	
	char pseudo[7];										// Le pseudo du joueur
	char gameNumber[4];									// Le numéro de la partie à laquelle on appartient
	char playerNumber[2];								// Le numéro du joueur dans la partie

	sfSocketUDP* socketReception = sfSocketUDP_Create();	// Socket utilisée pour écouter les messages du serveur
//====================================================================================================================================== configuration du client
	// Traitement des paramètres passés par l'utilisateur
	if(argc != 2) {
		perror("usage : client <pseudo(6 caractères max)>\n");
		exit(1);
    }
    snprintf(sendBuffer,7,"%s",argv[1]);
    snprintf(pseudo,7,"%s",argv[1]);

	// Liaison de la socket d'écoute des réponses au port 5100
	if(!sfSocketUDP_Bind(socketReception,5100)) {
		perror("erreur : impossible d'affecter le port 5100 à la socket d'écoute des réponses du serveur.\n");
		exit(1);
	} else {
		printf("Socket d'écoute des réponses du serveur liée au port 5100.\n");
	}
//====================================================================================================================================== demande de partie au serveur
	// Emission d'un message
	if(sfSocketUDP_Send(socket, sendBuffer, sizeof(sendBuffer), sfIPAddress_FromString("127.0.0.1"), 5000) != sfSocketDone)
	{
		perror("erreur : impossible d'établir la connexion avec le serveur pour demander une partie.\n");
		exit(1);
	}

	// Attente d'une réponse
	if(sfSocketUDP_Receive(socketReception, receptionBuffer, sizeof(receptionBuffer), received, sender, port) != sfSocketDone)
	{
		perror("erreur : impossible d'établir la connexion avec le serveur pour recevoir la réponse à la demande de partie.\n");
		exit(1);
	}
//====================================================================================================================================== traitement de la réponse
	// Gestion de la réponse du serveur
	printf("Réponse reçue: %s\n",receptionBuffer);
	
	if(strncmp(receptionBuffer, "noRoomAvailable", sizeof(receptionBuffer)) == 0) {
		printf("Aucune partie n'est disponible actuellement. Veuillez réessayer plus tard.\n");
		exit(0);
	}
	
	// Récupération du numéro de partie
	if((strlen(receptionBuffer)-strlen(strchr(receptionBuffer, '/'))) <= 3)
		snprintf(gameNumber, strlen(receptionBuffer)-strlen(strchr(receptionBuffer, '/'))+1, "%s", receptionBuffer);
	else
		snprintf(gameNumber, 4 , "%s",receptionBuffer);
	
	printf("%s\n", gameNumber);
	
	// Récupération du numéro de joueur
	snprintf(playerNumber, 2 , "%s", strchr(receptionBuffer, '/')+sizeof(char));
	
	printf("%s\n", playerNumber);
	
	
	// Liaison de la socket d'écoute du serveur au port de gestion de partie
	if(!sfSocketUDP_Bind(socketReception,5200)) {
		perror("erreur : impossible d'affecter le port 5200 à la socket d'écoute des réponses du serveur pour la partie.\n");
		exit(1);
	} else {
		printf("Socket d'écoute des réponses du serveur liée au port 5200.\n");
	}
	
	// Lancement de la boucle principale
		// écoute des entrées
		// affichage de l'état actuel du jeu
		// récupération des messages serveur
			// pb : fonction d'écoute bloquante
				// fork ?
				// thread ?
 
	fenetre = creationFenetre();

    exit(0);   
}
