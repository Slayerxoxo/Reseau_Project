#ifndef _TYPES_H_
#define _TYPES_H_

#include "def.h"

enum gameState_t {WAITING, PLAYING, FINISHED};			// Etat d'une partie: en attente de joueurs pour démarrer - en cours - terminée
enum direction_t {LEFT, RIGHT, UP, DOWN};				// Direction
enum bombState_t {IDLE, COUNTING, EXPLODING};		// Etat d'une bombe: en attente d'être posée - posée prête à exploser - en train d'exploser

typedef struct Position {								// Une position dans un plan
		unsigned int x;										// Son abscisse
		unsigned int y;										// Son ordonnée
	} Position;

typedef struct Bomb {									// Une bombe
		enum bombState_t state;								// Son état
		Position position;									// Sa position
	} Bomb;

typedef struct Player {									// Un joueur
		int lives;											// Ses vies restantes
		enum direction_t looking;							// La direction de son regard
		Bomb bombs[MAX_BOMB_NUMBER];						// Ses bombes
		Position position;									// Sa position
	} Player;

typedef struct Game {									// Une partie
		enum gameState_t state;								// Son état
		Player players[MAX_PLAYER_NUMBER];					// Les joueurs
		unsigned int playerNumber;							// Le nombre de joueurs
	} Game;

#endif		// _TYPES_H_
