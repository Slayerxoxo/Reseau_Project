#include "serverfunctions.h"
#include "def.h"
#include <stdio.h>
#include <string.h>
#include <SFML/Network.h>

extern unsigned int maxRooms;
extern Game* rooms[MAX_ROOM_NUMBER];
extern sfThread* threads[MAX_ROOM_NUMBER];
extern sfMutex* roomsMutex[MAX_ROOM_NUMBER];
extern unsigned int playersPerRoom;

int findRoom() {
	unsigned int i;
	int emptySlot = -1;
	int result = -1;
	
	for(i=0;i < maxRooms && result == -1;i++) {
		if(i!=0) {
			sfMutex_Unlock(roomsMutex[i-1]);
		}
		sfMutex_Lock(roomsMutex[i]);
		if(rooms[i] != NULL) {
			if(rooms[i]->state == RESET) {
				if(emptySlot < 0) {
					emptySlot = i;
				}
			} else {
				if(rooms[i]->playerNumber < playersPerRoom)		// Remplacer par un test sur state == WAITING ???
					result = i;
			}
		} else {
			if(emptySlot < 0) {
				emptySlot = i;
			}
		}
	}
	
	if(result == -1 && emptySlot >= 0) {
		rooms[emptySlot] = createRoom();
		if(threads[emptySlot] != NULL) {
			sfThread_Destroy(threads[emptySlot]);
		}
		threads[emptySlot] = sfThread_Create(&handleGame, &emptySlot);
		sfThread_Launch(threads[emptySlot]);

		result = emptySlot;
	}
	
	sfMutex_Unlock(roomsMutex[i-1]);
	
	return result;
}

Game* createRoom() {
	Game* result = (Game*)malloc(sizeof(Game));

	result->state = WAITING;
	result->playerNumber = 0;
	
	return result;
}

void resetRoom(int roomIndex) {
	int i;
	
	for(i=0; i<rooms[roomIndex]->playerNumber; i++) {
		free(rooms[roomIndex]->players[i]);
		rooms[roomIndex]->playerNumber--;
	}
	
	rooms[roomIndex]->state = RESET;
}

Player* createPlayer(int number) {
	Player* result = (Player*)malloc(sizeof(Player));
	
	result->lives = MAX_PLAYER_LIVES;
	if (number%2 == 0) {	// Les joueurs pairs regardent à droite
		result->looking = RIGHT;
	} else {				// Les joueurs impairs regardent à gauche
		result->looking = LEFT;
	}
	
	result->bombs[0].state = IDLE;
	
	return result;
}

void handleGame(void* roomIndex) {
	int gameIndex;								// L'indice de la partie à gérer dans le tableau des parties
	int activePlayer;							// L'indice du joueur dont c'est le tour
	int i;										// Une variable pour les boucles
	
	sfSocketUDP* socketReceive = sfSocketUDP_Create();		// Les sockets d'écoute des joueurs
	char receptionBuffer[128];								// Le buffer réceptionnant les messages reçus
	size_t* received = NULL;								// La taille des messages reçus
	sfIPAddress* sender = NULL;								// L'adresse de l'émetteur des messages reçus
	unsigned short* port = NULL;							// Le port sur lequel le message reçu a été envoyé
	
	char gameNumber[4];									// Le numéro de la partie à laquelle le joueur envoyant un message appartient
	char playerNumber[2];								// Le numéro du joueur ayant envoyé un message dans la partie
	
	sfSocketUDP* socketSend = sfSocketUDP_Create();
	
	gameIndex = *((int*)roomIndex);
	
	// boucle d'attente des joueurs
	while(rooms[gameIndex]->state == WAITING) {
		if(rooms[gameIndex]->playerNumber == playersPerRoom) {
			rooms[gameIndex]->state = STARTING;
		}
	}
	
	// boucle d'exécution de la partie	
	while(rooms[gameIndex]->state != FINISHED) {
		sfMutex_Lock(roomsMutex[gameIndex]);
	
		switch(rooms[gameIndex]->state) {
			case STARTING:	// Le dernier joueur attendu vient d'être ajouté
				// ouverture de la socket pour l'écoute des joueurs
				sfSocketUDP_Bind(socketReceive,5100+gameIndex*(playersPerRoom+1));
				
				// Envoi du message de début de partie à tout le monde
				for(i=0;i<rooms[gameIndex]->playerNumber;i++) {
					if(sfSocketUDP_Send(socketSend, "start", sizeof("start"), rooms[gameIndex]->players[i]->address, 5100+gameIndex*(playersPerRoom+1)+i+1) != sfSocketDone)
					{
						perror("erreur : impossible d'établir la connexion avec le client pour envoyer le message du jeu.\n");
					} else {
						printf("Envoi : start au joueur %d\n",i+1);
						sleep(1);
					}
				}
				// Indication au premier joueur que c'est son tour
				activePlayer = 1;
				if(sfSocketUDP_Send(socketSend, "play", sizeof("play"), rooms[gameIndex]->players[0]->address, 5100+gameIndex*(playersPerRoom+1)+1) != sfSocketDone)
					{
						perror("erreur : impossible d'établir la connexion avec le client pour envoyer le message du jeu.\n");
					} else {
						printf("Envoi : play\n");
					}
				rooms[gameIndex]->state = PLAYING;
				break;
			case PLAYING:	// La partie est en cours
				// écoute du joueur courant
				if(sfSocketUDP_Receive(socketReceive, receptionBuffer, sizeof(receptionBuffer), received, sender, port) != sfSocketDone)
				{
					perror("erreur : impossible d'établir la connexion avec le client pour recevoir les messages du jeu.\n");
				} else {
					printf("%s\n",receptionBuffer);
				}
				// test du message
					// Récupération du numéro de partie
					if((strlen(receptionBuffer)-strlen(strchr(receptionBuffer, '/'))) <= 3)
						snprintf(gameNumber, strlen(receptionBuffer)-strlen(strchr(receptionBuffer, '/'))+1, "%s", receptionBuffer);
					else
						snprintf(gameNumber, 4 , "%s",receptionBuffer);
	
					// Récupération du numéro de joueur
					snprintf(playerNumber, 2 , "%s", strchr(receptionBuffer, '/')+sizeof(char));
					
					if((atoi(gameNumber) == gameIndex) && (atoi(playerNumber) == activePlayer)) {
						// récupération du message et traitement en conséquence
					}
				break;
			default:
				;
		}

		sfMutex_Unlock(roomsMutex[gameIndex]);
	}
	
	// gestion de fin de partie
		// annonces aux joueurs
		// désallocations
		// ...
	sfMutex_Lock(roomsMutex[gameIndex]);
	
	resetRoom(gameIndex);
	
	sfMutex_Unlock(roomsMutex[gameIndex]);
}

void addPlayer(int roomIndex, sfIPAddress clientAdr) {
	rooms[roomIndex]->players[rooms[roomIndex]->playerNumber] = createPlayer(rooms[roomIndex]->playerNumber+1);
	rooms[roomIndex]->players[rooms[roomIndex]->playerNumber]->address = clientAdr;
	rooms[roomIndex]->playerNumber++;
	
	if(rooms[roomIndex]->state == RESET)
		rooms[roomIndex]->state = WAITING;
}

char* playerToString(Player player) {
	char* lives = malloc(16);
	char* direction;
	char* position = malloc(32+sizeof(char));
	
	char* res;
	
	sprintf(lives, "%d", player.lives);
	switch(player.looking) {
		case LEFT:
			direction = malloc(5*sizeof(char));
			sprintf(direction, "LEFT");
			break;
		case RIGHT:
			direction = malloc(5*sizeof(char));
			sprintf(direction, "RIGHT");
			break;
		case UP:
			direction = malloc(3*sizeof(char));
			sprintf(direction, "UP");
			break;
		case DOWN:
			direction = malloc(5*sizeof(char));
			sprintf(direction, "DOWN");
			break;
	}
	sprintf(position, "%d;%d", player.position.x, player.position.y);
	
	res = malloc(sizeof(lives)+sizeof(direction)+sizeof(position)+3*sizeof(char));
	sprintf(res, "%s;%s;%s", lives, direction, position);
	
	return res;
}

char* gameToString(Game game) {
	return NULL;
}
