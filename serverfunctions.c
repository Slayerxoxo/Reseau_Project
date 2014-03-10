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
	if (number%2 != 0) {	// Les joueurs impairs regardent à droite
		result->looking = RIGHT;
	} else {				// Les joueurs pairs regardent à gauche
		result->looking = LEFT;
	}
	
	switch(number) {
		case 1:
			result->position.x = 0;
			result->position.y = 0;
			break;
		case 2:
			result->position.x = 8;
			result->position.y = 8;
			break;
		case 3:
			result->position.x = 0;
			result->position.y = 8;
			break;
		case 4:
			result->position.x = 8;
			result->position.y = 0;
		default:
			;
	}
	
	result->bombs[0].state = IDLE;
	
	return result;
}

void handleGame(void* roomIndex) {
	int gameIndex;								// L'indice de la partie à gérer dans le tableau des parties
	int activePlayer;							// L'indice du joueur dont c'est le tour
	int i,j;									// Des variables pour le parcours des boucles
	
	sfSocketUDP* socketReceive = sfSocketUDP_Create();		// Les sockets d'écoute des joueurs
	char receptionBuffer[128];								// Le buffer réceptionnant les messages reçus
	size_t* received = NULL;								// La taille des messages reçus
	sfIPAddress* sender = NULL;								// L'adresse de l'émetteur des messages reçus
	unsigned short* port = NULL;							// Le port sur lequel le message reçu a été envoyé
	
	char gameNumber[4];									// Le numéro de la partie à laquelle le joueur envoyant un message appartient
	char playerNumber[2];								// Le numéro du joueur ayant envoyé un message dans la partie
	char message[128-4];								// Le message envoyé par le joueur
	int validMove;										// Une variable pour indiquer si l'action effectuée par le joueur est possible ou non (0 = IMPOSSIBLE, 1 = POSSIBLE)
	
	char response[4096];								// La réponse à envoyer au joueur
	
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
					}
				}
				// Construction du message décrivant l'état du jeu
				sprintf(response,"%s",playerToString(*(rooms[gameIndex]->players[0])));
				
				for(i=1; i < rooms[gameIndex]->playerNumber; i++) {
					sprintf(response,"%s/%s",response, playerToString(*(rooms[gameIndex]->players[i])));
				}
				
				// Envoi de l'état du jeu aux joueurs
				for(i=0;i<rooms[gameIndex]->playerNumber;i++) {
					if(sfSocketUDP_Send(socketSend, response, sizeof(response), rooms[gameIndex]->players[i]->address, 5100+gameIndex*(playersPerRoom+1)+i+1) != sfSocketDone)
					{
						perror("erreur : impossible d'établir la connexion avec le client pour envoyer le message du jeu.\n");
					} else {
						printf("Envoi :\n     %s\n     au joueur %d\n", response, i+1);
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
						// récupération du message
						snprintf(message,128-2-strlen(gameNumber)-strlen(playerNumber),"%s", strchr(strchr(receptionBuffer, '/')+sizeof(char), '/')+sizeof(char));

						// traitement du message
						validMove = 0;
						if(strcmp(message,"up") == 0) {
							if(rooms[gameIndex]->players[activePlayer-1]->position.y != 0) {
								if((rooms[gameIndex]->players[activePlayer-1]->position.y)%2 == 0) {
									if((rooms[gameIndex]->players[activePlayer-1]->position.x)%2 == 0) {
										validMove = 1;
										//update
										rooms[gameIndex]->players[activePlayer-1]->position.y-=1;
										rooms[gameIndex]->players[activePlayer-1]->looking = UP;
									}
								} else {
									validMove = 1;
									//update
									rooms[gameIndex]->players[activePlayer-1]->position.y-=1;
									rooms[gameIndex]->players[activePlayer-1]->looking = UP;
								}
							}
						}
						if(strcmp(message,"down") == 0) {
							if(rooms[gameIndex]->players[activePlayer-1]->position.y != 8) {
								if((rooms[gameIndex]->players[activePlayer-1]->position.y)%2 == 0) {
									if((rooms[gameIndex]->players[activePlayer-1]->position.x)%2 == 0) {
										validMove = 1;
										//update
										rooms[gameIndex]->players[activePlayer-1]->position.y+=1;
										rooms[gameIndex]->players[activePlayer-1]->looking = DOWN;
									}
								} else {
									validMove = 1;
									//update
									rooms[gameIndex]->players[activePlayer-1]->position.y+=1;
									rooms[gameIndex]->players[activePlayer-1]->looking = DOWN;
								}
							}
						}
						if(strcmp(message,"left") == 0) {
							if(rooms[gameIndex]->players[activePlayer-1]->position.x != 0) {
								if((rooms[gameIndex]->players[activePlayer-1]->position.x)%2 == 0) {
									if((rooms[gameIndex]->players[activePlayer-1]->position.y)%2 == 0) {
										validMove = 1;
										//update
										rooms[gameIndex]->players[activePlayer-1]->position.x-=1;
										rooms[gameIndex]->players[activePlayer-1]->looking = LEFT;
									}
								} else {
									validMove = 1;
									//update
									rooms[gameIndex]->players[activePlayer-1]->position.x-=1;
									rooms[gameIndex]->players[activePlayer-1]->looking = LEFT;
								}
							}
						}
						if(strcmp(message,"right") == 0) {
							if(rooms[gameIndex]->players[activePlayer-1]->position.x != 8) {
								if((rooms[gameIndex]->players[activePlayer-1]->position.x)%2 == 0) {
									if((rooms[gameIndex]->players[activePlayer-1]->position.y)%2 == 0) {
										validMove = 1;
										//update
										rooms[gameIndex]->players[activePlayer-1]->position.x+=1;
										rooms[gameIndex]->players[activePlayer-1]->looking = RIGHT;
									}
								} else {
									validMove = 1;
									//update
									rooms[gameIndex]->players[activePlayer-1]->position.x+=1;
									rooms[gameIndex]->players[activePlayer-1]->looking = RIGHT;
								}
							}
						}
						if(strcmp(message,"bomb") == 0) {
							// test sur les bombes restantes
							for(i = 0; i < MAX_BOMB_NUMBER && validMove == 0;i++) {
								if(rooms[gameIndex]->players[activePlayer-1]->bombs[i].state == IDLE) {
									rooms[gameIndex]->players[activePlayer-1]->bombs[i].state = LAUNCHING;
									rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.x = rooms[gameIndex]->players[activePlayer-1]->position.x;
									rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.y = rooms[gameIndex]->players[activePlayer-1]->position.y;
									validMove = 1;
								}
							}
						}
						
						if(validMove == 0) {
							// Indication au joueur que son action est impossible
							if(sfSocketUDP_Send(socketSend, "fail", sizeof("fail"), rooms[gameIndex]->players[activePlayer-1]->address, 5100+gameIndex*(playersPerRoom+1)+activePlayer) != sfSocketDone)
							{
								perror("erreur : impossible d'établir la connexion avec le client pour envoyer le message du jeu.\n");
							} else {
								printf("Envoi : fail\n");
							}
						} else {
							// Evolution de l'état des bombes
							for(i = 0; i < MAX_BOMB_NUMBER; i++) {
								switch(rooms[gameIndex]->players[activePlayer-1]->bombs[i].state) {
									case LAUNCHING:
										rooms[gameIndex]->players[activePlayer-1]->bombs[i].state = COUNTING1;
										break;
									case COUNTING1:
										rooms[gameIndex]->players[activePlayer-1]->bombs[i].state = COUNTING2;
										break;
									case COUNTING2:
										rooms[gameIndex]->players[activePlayer-1]->bombs[i].state = COUNTING3;
										break;
									case COUNTING3:
										rooms[gameIndex]->players[activePlayer-1]->bombs[i].state = COUNTING4;
										break;
									case COUNTING4:
										rooms[gameIndex]->players[activePlayer-1]->bombs[i].state = RED;
										break;
									case RED:
										rooms[gameIndex]->players[activePlayer-1]->bombs[i].state = EXPLODING;
										break;
									case EXPLODING:
										rooms[gameIndex]->players[activePlayer-1]->bombs[i].state = IDLE;
										// calcul des dégats
										for(j=0;j<rooms[gameIndex]->playerNumber;j++){
											if(rooms[gameIndex]->players[j]->position.x == rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.x && rooms[gameIndex]->players[j]->position.y == rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.y){
												if(rooms[gameIndex]->players[j]->lives > 0){
													rooms[gameIndex]->players[j]->lives--;
												}
											}
											if((rooms[gameIndex]->players[j]->position.x -1) == rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.x && rooms[gameIndex]->players[j]->position.y == rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.y){
												if(rooms[gameIndex]->players[j]->lives > 0){
													rooms[gameIndex]->players[j]->lives--;
												}
											}
											if((rooms[gameIndex]->players[j]->position.x +1) == rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.x && rooms[gameIndex]->players[j]->position.y == rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.y){
												if(rooms[gameIndex]->players[j]->lives > 0){
													rooms[gameIndex]->players[j]->lives--;
												}
											}
											if(rooms[gameIndex]->players[j]->position.x == rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.x && (rooms[gameIndex]->players[j]->position.y +1) == rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.y){
												if(rooms[gameIndex]->players[j]->lives > 0){
													rooms[gameIndex]->players[j]->lives--;
												}
											}
											if(rooms[gameIndex]->players[j]->position.x == rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.x && (rooms[gameIndex]->players[j]->position.y -1) == rooms[gameIndex]->players[activePlayer-1]->bombs[i].position.y){
												if(rooms[gameIndex]->players[j]->lives > 0){
													rooms[gameIndex]->players[j]->lives--;
												}
											}
										}
										break;
									default:
										;
								}
							}
							// Construction du message décrivant l'état du jeu
							sprintf(response,"%s",playerToString(*(rooms[gameIndex]->players[0])));
							
							for(i=1; i < rooms[gameIndex]->playerNumber; i++) {
								sprintf(response,"%s/%s",response, playerToString(*(rooms[gameIndex]->players[i])));
							}
							
							// Envoi de l'état du jeu aux joueurs
							for(i=0;i<rooms[gameIndex]->playerNumber;i++) {
								if(sfSocketUDP_Send(socketSend, response, sizeof(response), rooms[gameIndex]->players[i]->address, 5100+gameIndex*(playersPerRoom+1)+i+1) != sfSocketDone)
								{
									perror("erreur : impossible d'établir la connexion avec le client pour envoyer le message du jeu.\n");
								} else {
									printf("Envoi :\n     %s\n     au joueur %d\n", response, i+1);
								}
							}
							// Indication au joueur suivant que c'est son tour
							do {
								if(activePlayer != rooms[gameIndex]->playerNumber) {
									activePlayer++;
								} else {
									activePlayer = 1;
								}
							} while(rooms[gameIndex]->players[activePlayer-1]->lives <= 0);
							if(sfSocketUDP_Send(socketSend, "play", sizeof("play"), rooms[gameIndex]->players[activePlayer-1]->address, 5100+gameIndex*(playersPerRoom+1)+activePlayer) != sfSocketDone)
							{
								perror("erreur : impossible d'établir la connexion avec le client pour envoyer le message du jeu.\n");
							} else {
								printf("Envoi : play\n");
							}
						}
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
	char* lives = malloc(sizeof(int));
	char* direction;
	char* position = malloc(2*sizeof(int)+sizeof(char));
	char* bombs = malloc(MAX_BOMB_NUMBER*(2*sizeof(char)+2*sizeof(char)+2*sizeof(int))+(MAX_BOMB_NUMBER-1)*sizeof(char));
	int i;
	
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
	
	for(i=0;i<MAX_BOMB_NUMBER;i++) {
		if(i != 0) {
			bombs = strcat(bombs,";");
		}
		switch(player.bombs[i].state) {
			case IDLE:
				bombs = strcat(bombs,"i;0;0");
				break;
			case COUNTING1:
				sprintf(position, "%d;%d", player.bombs[i].position.x, player.bombs[i].position.y);
				bombs = strcat(bombs,"c1;");	
				bombs = strcat(bombs, position);
				break;
			case COUNTING2:
				sprintf(position, "%d;%d", player.bombs[i].position.x, player.bombs[i].position.y);
				bombs = strcat(bombs,"c2;");	
				bombs = strcat(bombs, position);
				break;
			case COUNTING3:
				sprintf(position, "%d;%d", player.bombs[i].position.x, player.bombs[i].position.y);
				bombs = strcat(bombs,"c3;");	
				bombs = strcat(bombs, position);
				break;
			case COUNTING4:
				sprintf(position, "%d;%d", player.bombs[i].position.x, player.bombs[i].position.y);
				bombs = strcat(bombs,"c4;");	
				bombs = strcat(bombs, position);
				break;
			case RED:
				sprintf(position, "%d;%d", player.bombs[i].position.x, player.bombs[i].position.y);
				bombs = strcat(bombs,"r;");	
				bombs = strcat(bombs, position);
				break;
			case EXPLODING:
				sprintf(position, "%d;%d", player.bombs[i].position.x, player.bombs[i].position.y);
				bombs = strcat(bombs,"e;");
				bombs = strcat(bombs, position);
				break;
			default:
				;
		}
	}
	
	sprintf(position, "%d;%d", player.position.x, player.position.y);
	
	res = malloc(sizeof(lives)+sizeof(direction)+sizeof(position)+4*sizeof(char)+sizeof(bombs));
	sprintf(res, "%s;%s;%s;%s", lives, direction, position, bombs);
	
	return res;
}

char* gameToString(Game game) {
	return NULL;
}
