#include "serverfunctions.h"
#include "def.h"

extern unsigned int maxRooms;
extern unsigned int roomNumber;
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
			if(rooms[i]->playerNumber < playersPerRoom)		// Remplacer par un test sur state == WAITING ???
				result = i;
		} else {
			if(emptySlot < 0) {
				emptySlot = i;
			}
		}
	}
	
	if(result == -1 && emptySlot >= 0) {
		rooms[emptySlot] = createRoom();
		if(threads[emptySlot] == NULL) {
			threads[emptySlot] = sfThread_Create(&handleGame, &emptySlot);
			sfThread_Launch(threads[emptySlot]);
		}
		result = emptySlot;
	}
	
	sfMutex_Unlock(roomsMutex[i-1]);
	
	return result;
}

Game* createRoom() {
	Game* result = (Game*)malloc(sizeof(Game));

	result->state = WAITING;
	result->playerNumber = 0;

	roomNumber++;
	
	return result;
}

Player* createPlayer(int number) {
	Player* result = (Player*)malloc(sizeof(Player));
	
	result->lives = MAX_PLAYER_LIVES;
	if (number%2 == 0) {	// Les joueurs pairs regardent à droite
		result->looking = RIGHT;
	} else {				// Les joueurs impairs regardent à gauche
		result->looking = LEFT;
	}
	
	return result;
}

void handleNewPlayer(void* arg) {

}

void handleGame(void* roomIndex) {
	int gameIndex;		// L'indice de la partie à gérer dans le tableau des parties
	
	gameIndex = *((int*)roomIndex);
	
	// boucle d'exécution de la partie	
	while(rooms[gameIndex]->state != FINISHED) {
		sfMutex_Lock(roomsMutex[gameIndex]);
	
		switch(rooms[gameIndex]->state) {
			case WAITING:	// En attente de tous les joueurs
				if(rooms[gameIndex]->playerNumber == playersPerRoom) {
					rooms[gameIndex]->state = STARTING;
				}
				break;
			case STARTING:	// Le dernier joueur attendu vient d'être ajouté
				// ouverture des threads/sockets/... pour écoute des joueurs
				break;
			case PLAYING:	// La partie est en cours
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
}

void addPlayer(int roomIndex) {
	rooms[roomIndex]->players[rooms[roomIndex]->playerNumber] = createPlayer(rooms[roomIndex]->playerNumber+1);
	rooms[roomIndex]->playerNumber++;
}
