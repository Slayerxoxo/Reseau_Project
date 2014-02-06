#include "serverfunctions.h"
#include "def.h"

extern unsigned int maxRooms;
extern unsigned int roomNumber;
extern Game* rooms[MAX_ROOM_NUMBER];
extern sfThread* threads[MAX_ROOM_NUMBER];
extern sfMutex* roomsMutex;
extern unsigned int playersPerRoom;

int findRoom() {
	unsigned int i;
	int emptySlot = -1;
	int result = -1;

	sfMutex_Lock(roomsMutex);
	
	for(i=0;i < maxRooms && result == -1;i++) {
		if(rooms[i] != NULL) {
			if(rooms[i]->playerNumber < playersPerRoom)
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
			threads[emptySlot] = sfThread_Create(&handleGame, rooms[emptySlot]);
			sfThread_Launch(threads[emptySlot]);
		}
		result = emptySlot;
	}
	
	sfMutex_Unlock(roomsMutex);
	
	return result;
}

Game* createRoom() {
	Game* result = (Game*)malloc(sizeof(Game));

	result->state = WAITING;
	result->playerNumber = 0;

	roomNumber++;
	
	return result;
}

void handleNewPlayer(void* arg) {

}

void handleGame(void* arg) {
	int gameIndex;		// L'indice de la partie à gérer dans le tableau des parties
	
	// récupération des arguments
	
	// boucle d'exécution de la partie
	while(rooms[gameIndex]->state != FINISHED) {
		switch(rooms[gameIndex]->state) {
			case WAITING:
				if(rooms[gameIndex]->playerNumber == playersPerRoom) {
					rooms[gameIndex]->state = STARTING;
				}
				break;
			case STARTING:
				// ouverture des threads/sockets/... pour écoute des joueurs
				break;
			case PLAYING:
				break;
			default:
				;
		}
	}
	
	// gestion de fin de partie
		// annonces aux joueurs
		// désallocations
		// ...
}
