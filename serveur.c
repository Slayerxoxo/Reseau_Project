/*----------------------------------------------
Serveur à lancer avant le client
------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <SFML/Network.h>

int main(int argc, char **argv) {
	sfSocketUDP* socket = sfSocketUDP_Create();
	char receptionBuffer[128];
	size_t* received = NULL;
	sfIPAddress* sender = NULL;
	unsigned short* port = NULL;
	
	if(!sfSocketUDP_Bind(socket,5000)) {
		perror("erreur : impossible d'affecter le port 5000 à la socket du serveur.\n");
		exit(1);
	} else {
		printf("Socket d'écoute des connexions du serveur liée au port 5000\n");
	}

	
	if (sfSocketUDP_Receive(socket, receptionBuffer, sizeof(receptionBuffer), received, sender, port) != sfSocketDone)
	{
		perror("erreur : impossible d'établir la connexion avec le client.\n");
		exit(1);
	}

	sfSocketUDP_Destroy(socket);
	
	return(EXIT_SUCCESS);
}
