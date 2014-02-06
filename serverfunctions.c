#include "serverfunctions.h"
#include "def.h"

extern unsigned int maxRooms;
extern unsigned int roomNumber;
extern Game* rooms[MAX_ROOM_NUMBER];
extern sfThread* threads[MAX_ROOM_NUMBER];
extern sfMutex* roomsMutex;
extern unsigned int playersPerRoom;

sfThread* findRoom() {
	unsigned int i;
	int emptySlot = -1;
	sfThread* result = NULL;

	sfMutex_Lock(roomsMutex);
	
	for(i=0;i < maxRooms && result == NULL;i++) {
		if(rooms[i] != NULL) {
			if(rooms[i]->playerNumber < playersPerRoom)
				result = threads[i];
		} else {
			if(emptySlot < 0) {
				emptySlot = i;
			}
		}
	}
	
	if(result == NULL && emptySlot >= 0) {
		rooms[emptySlot] = createRoom();
		if(threads[emptySlot] == NULL) {
			//threads[emptySlot] = sfThread_Create(handleGame(), rooms[emptySlot]);
			sfThread_Launch(threads[emptySlot]);
		}
		result = threads[emptySlot];
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
