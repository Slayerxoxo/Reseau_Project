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
#include "def.h"

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
	char sendBuffer[] = "PlayerPseudo";					// mess à envoyer
	char receptionBuffer[128];							// Le buffer réceptionnant les messages reçus
	size_t* received = NULL;							// La taille des messages reçus
	sfIPAddress* sender = NULL;							// L'adresse de l'émetteur des messages reçus
	unsigned short* port = NULL;						// Le port sur lequel le message reçu a été envoyé

	// Emission d'un message
	if(sfSocketUDP_Send(socket, sendBuffer, sizeof(sendBuffer), sfIPAddress_FromString("127.0.0.1"), 5000) != sfSocketDone)
	{
		perror("erreur : impossible d'établir la connexion avec le serveur.\n");
		exit(1);
	}
	
	// Attente d'une réponse
	if(sfSocketUDP_Receive(socket, receptionBuffer, sizeof(receptionBuffer), received, sender, port) != sfSocketDone)
	{
		perror("erreur : impossible d'établir la connexion avec le serveur.\n");
		exit(1);
	}
	
	// Gestion de la réponse du serveur
	
	// Lancement de la boucle principale
		// écoute des entrées
		// affichage de l'état actuel du jeu
		// récupération des messages serveur
			// pb : fonction d'écoute bloquante
				// fork ?
				// thread ?
 
	fenetre = creationFenetre();
    sleep(3);
    
       
    exit(0);   
}
