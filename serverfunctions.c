#include "serverfunctions.h"
#include "def.h"

extern unsigned int maxRooms;
extern Game* rooms[MAX_ROOM_NUMBER];
extern sfMutex* roomsMutex;
extern unsigned int playersPerRoom;

Game* findRoom() {
	unsigned int i;
	int emptySlot = -1;
	Game* result = NULL;

	sfMutex_Lock(roomsMutex);
	
	for(i=0;i < maxRooms || result != NULL;i++) {
		if(rooms[i] != NULL) {
			if(rooms[i]->playerNumber < playersPerRoom)
				result = rooms[i];
		} else {
			if(emptySlot < 0) {
				emptySlot = i;
			}
		}
	}
	
	if(result == NULL && emptySlot >= 0) {
		rooms[emptySlot] = createRoom();
		result = rooms[emptySlot];
	}
	
	sfMutex_Unlock(roomsMutex);
	
	return result;
}

Game* createRoom() {
	Game* result = (Game*)malloc(sizeof(Game));

	result->state = WAITING;
	result->playerNumber = 0;
	
	return result;
}
