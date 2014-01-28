#ifndef _TYPES_H_
#define _TYPES_H_

#include "def.h"

enum gameState_t {WAITING, PLAYING, FINISHED};
enum direction_t {LEFT, RIGHT, UP, DOWN};

typedef struct Player {
		int lives;
		enum direction_t looking;
	} Player;

typedef struct Game {
		enum gameState_t state;
		Player players[MAX_PLAYER_NUMBER];
		unsigned int playerNumber;
	} Game;

#endif		// _TYPES_H_
