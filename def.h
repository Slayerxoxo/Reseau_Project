#ifndef _DEF_H_
#define _DEF_H_

#define MAX_ROOM_NUMBER 100				// Le nombre maximum de parties simultanées
#define MAX_PLAYER_NUMBER 4				// Le nombre de joueurs par partie
#define MAX_BOMB_NUMBER 3				// Le nombre de bombes posables simultanément par joueur
#define MAX_PSEUDO_SIZE 25				// La taille maximale du pseudo des joueurs
#define MAX_PLAYER_LIVES 2				// Le nombre de vies d'un joueur

#define HAUTEUR 11						// Hauteur de la carte
#define LARGEUR 11						// Largeur de la carte

#endif		// _DEF_H_

/*
	Convention sur les attributions de ports
	========================================
	
	Serveur:
		Ecoute des demandes de conexion								->		5000
		Ecoute de la partie n (0 -> maxPartiesSimultanées)			->		5100 + n * ( maxJoueursParSalle + 1 )

	Client:
		Ecoute des réponses aux demandes de connexion																	->		5001
		Ecoute du serveur pour le joueur j (1 -> maxJoueursParSalle) de la partie n (0 -> maxPartiesSimultanées)		->		5100 + n * ( maxJoueursParSalle + 1 ) + j

*/
