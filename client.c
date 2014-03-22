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

extern Player* playersTab[MAX_PLAYER_NUMBER];

int main(int argc, char **argv) {

	sfSocketUDP* socket = sfSocketUDP_Create();			// création de la socket
	char sendBuffer[10];								// mess à envoyer
	char receptionBuffer[4096];							// Le buffer réceptionnant les messages reçus
	size_t* received = NULL;							// La taille des messages reçus
	sfIPAddress* sender = NULL;							// L'adresse de l'émetteur des messages reçus
	unsigned short* port = NULL;						// Le port sur lequel le message reçu a été envoyé
	
	char pseudo[10];									// Le pseudo du joueur
	char gameNumber[4];									// Le numéro de la partie à laquelle on appartient
	char playerNumber[2];								// Le numéro du joueur dans la partie
	int myTurn = 0;										// Indique si c'est notre tour de jouer ou non (0 = NON, 1 = OUI)
	int played = 0;										// Indique si un coup a été joué ou non (0 = NON, 1 = OUI)
	
	sfSocketStatus receivedStatus;						// Le status émis par une socket après un appel à sfSocketUDP_Receive
	
	int gameEnded = 0;									// Indique si la partie est finie ou non (0 = NON, 1 = OUI)
	int msgReceived;									// Indique si un message a été reçu du serveur (0 = NON, 1 = OUI)
	int timeoutCount = 0;								// Compte le nombre de fois où un timeout apparait dans les communications avec le serveur
	
	int playersInMessage = 0;								// Le nombre de joueurs décrits par un message du serveur

	sfSocketUDP* socketReception = sfSocketUDP_Create();	// Socket utilisée pour écouter les messages du serveur
//====================================================================================================================================== configuration du client
	// Traitement des paramètres passés par l'utilisateur
	if(argc != 3) {
		perror("usage : client <pseudo(9 caractères max)> <ip serveur>\n");
		exit(1);
    }
    snprintf(sendBuffer,10,"%s",argv[1]);
    snprintf(pseudo,10,"%s",argv[1]);

	// Liaison de la socket d'écoute des réponses au port 5001
	if(!sfSocketUDP_Bind(socketReception,5001)) {
		perror("erreur : impossible d'affecter le port 5001 à la socket d'écoute des réponses du serveur.\n");
		exit(1);
	} else {
		printf("Socket d'écoute des réponses du serveur liée au port 5001.\n");
	}
//====================================================================================================================================== demande de partie au serveur
	// Emission d'un message
	if(sfSocketUDP_Send(socket, sendBuffer, sizeof(sendBuffer), sfIPAddress_FromString(argv[2]), 5000) != sfSocketDone)
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
	playersInMessage = atoi(playerNumber);	
	
	// Liaison de la socket d'écoute du serveur au port de gestion de partie
	if(!sfSocketUDP_Bind(socketReception,5100+atoi(gameNumber)*(MAX_PLAYER_NUMBER+1)+atoi(playerNumber))) {
		perror("erreur : impossible d'affecter le port 5200 à la socket d'écoute des réponses du serveur pour la partie.\n");
		exit(1);
	} else {
		printf("Socket d'écoute des réponses du serveur liée au port %d.\n",5100+atoi(gameNumber)*(MAX_PLAYER_NUMBER+1)+atoi(playerNumber));
	}
	sfSocketUDP_SetBlocking(socketReception, sfFalse);
	
	fenetre = creationFenetre();
	
	//Affichage
	sfRenderWindow_Clear(fenetre, sfBlack);			//Remplissage de l'écran par un fond noir
	creationBackground(fenetre, LARGEUR, HAUTEUR);	//Création de la carte
	
	initialisation();								//initialisation de la position du joueur principal
    sfRenderWindow_Display(fenetre);
	
	/* Gestion de la partie */

	sfClock* roundClock = sfClock_Create();
	int secondsLeft = 30;

	while(sfRenderWindow_IsOpened(fenetre) && gameEnded == 0 && timeoutCount < 5){
		sfEvent Event;

		//Affichage
		sfRenderWindow_Clear(fenetre, sfBlack);			//Remplissage de l'écran par un fond noir
		creationBackground(fenetre, LARGEUR, HAUTEUR);	//Création de la carte
		
		refreshJoueur(playersTab, playersInMessage);
	    sfRenderWindow_Display(fenetre);

		if (myTurn == 1) {	// C'est à notre tour de jouer
	    
			// affichage du temps restant
			if(secondsLeft != 30-(int)sfClock_GetTime(roundClock)) {
				secondsLeft = 30-(int)sfClock_GetTime(roundClock);
				printf("Il vous reste %d secondes.\n",secondsLeft);
			}
		
			// Ecoute des touches
  			while (sfRenderWindow_GetEvent(fenetre, &Event)){

				//Fermeture de la fenêtre
		        if (Event.Type == sfEvtClosed){
		            sfRenderWindow_Close(fenetre);
				}
				if ((Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyEscape)){
		            sfRenderWindow_Close(fenetre);
				}
				//Déplacement
				if ((Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyUp)){
					//fonction d'envoie au serveur la demande du déplacement
					snprintf(sendBuffer,sizeof(sendBuffer),"%s/%s/up",gameNumber, playerNumber);
					played = 1;
				}
				if ((Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyDown)){
					//fonction d'envoie au serveur la demande du déplacement
					snprintf(sendBuffer,sizeof(sendBuffer),"%s/%s/down",gameNumber, playerNumber);
					played = 1;
				}
				if ((Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyLeft)){
					//fonction d'envoie au serveur la demande du déplacement
					snprintf(sendBuffer,sizeof(sendBuffer),"%s/%s/left",gameNumber, playerNumber);
					played = 1;
				}
				if ((Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyRight)){
					//fonction d'envoie au serveur la demande du déplacement
					snprintf(sendBuffer,sizeof(sendBuffer),"%s/%s/right",gameNumber, playerNumber);
					played = 1;
				}
				if ((Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeySpace)){
					//fonction d'envoie au serveur la demande du déplacement
					snprintf(sendBuffer,sizeof(sendBuffer),"%s/%s/bomb",gameNumber, playerNumber);
					played = 1;
				}
			}
			// Envoi du coup joué
			if(played == 1) {
				msgReceived = 0;
				while(msgReceived == 0 && timeoutCount < 5) {
					if(sfSocketUDP_Send(socket, sendBuffer, sizeof(sendBuffer), sfIPAddress_FromString(argv[2]), 5100+atoi(gameNumber)*(MAX_PLAYER_NUMBER+1)) != sfSocketDone)
					{
						perror("erreur : impossible d'établir la connexion avec le serveur pour envoyer le coup joué.\n");
						exit(1);
					}
					played = 0;

					// Attente d'une réponse
					sfClock* timeoutClock = sfClock_Create();
					while(sfClock_GetTime(timeoutClock) < 3.0 && msgReceived == 0) {
						receivedStatus = sfSocketUDP_Receive(socketReception, receptionBuffer, sizeof(receptionBuffer), received, sender, port);
						if(receivedStatus == sfSocketDone)
						{
							msgReceived = 1;
						}
					}
					if(msgReceived == 1) {
						// Traitement du message
						timeoutCount = 0;
						if (strcmp(receptionBuffer, "fail") != 0){
							printf("%s\n",receptionBuffer);
							myTurn = 0;	
							printf("\n Veuillez patienter \n");
							//maj des joueurs (nouvel état après mon mouvement)
							playersInMessage = playersInString(receptionBuffer);
							stringToPlayers(receptionBuffer, playersTab, playersInMessage);
						}
					} else {
						// gestion du timeout
						timeoutCount++;
					}
					sfClock_Destroy(timeoutClock);
				}
			}
			if (secondsLeft <= 0) {
				// fin du tour
				printf("Temps écoulé! Vous perdez votre tour!\n");
				myTurn = 0;
				printf("Veuillez patienter\n");
				// envoi de l'info au serveur
			}
		} else {	// En attente de notre tour
			msgReceived = 0;
			// Attente d'une réponse
			while(msgReceived == 0 && sfRenderWindow_IsOpened(fenetre)) {
				// Ecoute des touches
				sfEvent Event;
				while (sfRenderWindow_GetEvent(fenetre, &Event)){

					//Fermeture de la fenêtre
					if (Event.Type == sfEvtClosed){
						sfRenderWindow_Close(fenetre);
					}
					if ((Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyEscape)){
						sfRenderWindow_Close(fenetre);
					}
				}
				// Ecoute du serveur
				receivedStatus = sfSocketUDP_Receive(socketReception, receptionBuffer, sizeof(receptionBuffer), received, sender, port);
				if(receivedStatus == sfSocketDone)
				{
					msgReceived = 1;
					
					// Traitement du message
					if(strcmp(receptionBuffer, "play") == 0){
						myTurn = 1;
						secondsLeft = 30;
						printf("\n A votre tour !!! \n");
						// vidage des événements de la fenêtre
						while (sfRenderWindow_GetEvent(fenetre, &Event)) {
				
						}
						sfClock_Reset(roundClock);
					}
					else {
						if(strcmp(receptionBuffer, "start") == 0) {
				
						}
						else {
							if(strcmp(receptionBuffer, "finish") == 0) {
								gameEnded = 1;
							}
							else {
								printf("%s\n",receptionBuffer);
								//maj des joueurs
								playersInMessage = playersInString(receptionBuffer);
								stringToPlayers(receptionBuffer, playersTab, playersInMessage);
							}
						}
					}
				}
			}
		}
	}
	// Partie terminée
	if(gameEnded == 1) {
		printf("#=============================#\n");
		printf("#       Partie terminée       #\n");
		printf("#-----------------------------#\n");
		if(playersTab[atoi(playerNumber)-1]->lives > 0)
			printf("# Vous gagnez, félicitations! #\n");
		else
			printf("#    Vous perdez, dommage!    #\n");
		printf("#=============================#\n");
		printf("------Appuyez sur ECHAP ou fermez la fenêtre pour quitter------\n");
		while(sfRenderWindow_IsOpened(fenetre)) {
			sfEvent Event;
			while (sfRenderWindow_GetEvent(fenetre, &Event)){

				//Fermeture de la fenêtre
				if (Event.Type == sfEvtClosed){
					sfRenderWindow_Close(fenetre);
				}
				if ((Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyEscape)){
					sfRenderWindow_Close(fenetre);
				}
			}
		}
	} else {
		// Timeouts trop nombreux
		if(timeoutCount >= 5) {
			printf("Le serveur ne répond plus, la partie est interrompue!\n");
			printf("Merci d'avoir joué.\n");
			printf("------Appuyez sur ECHAP ou fermez la fenêtre pour quitter------\n");
			while(sfRenderWindow_IsOpened(fenetre)) {
				sfEvent Event;
				while (sfRenderWindow_GetEvent(fenetre, &Event)){

					//Fermeture de la fenêtre
					if (Event.Type == sfEvtClosed){
						sfRenderWindow_Close(fenetre);
					}
					if ((Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyEscape)){
						sfRenderWindow_Close(fenetre);
					}
				}
			}
		} else {
			// La fenêtre a été fermée
			if(!sfRenderWindow_IsOpened(fenetre)) {
				printf("Vous avez quitté la partie en cours de jeu.\n");
				printf("A bientôt!\n");
			}
		}
	}
	
    exit(0);   
}
