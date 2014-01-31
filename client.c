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

#include <SFML/Network.h>
#include <SFML/System.h>


/* Déclarations */
sfRenderWindow* fenetre;


typedef struct sockaddr 	sockaddr;
typedef struct sockaddr_in 	sockaddr_in;
typedef struct hostent 		hostent;
typedef struct servent 		servent;

int main(int argc, char **argv) {
  
	sfSocketUDP* socket = sfSocketUDP_Create();					// création de la socket
	char sendBuffer[] = "Hi guys !";		// mess à envoyer

	

	// Emission d'un message
	if(sfSocketUDP_Send(socket, sendBuffer, sizeof(sendBuffer), sfIPAddress_FromString("127.0.0.1"), 5000) != sfSocketDone)
	{
		perror("erreur : impossible d'établir la connexion avec le client.\n");
		exit(1);
	}
 
	fenetre = creationFenetre();
    sleep(3);
    
       
    exit(0);   
}
