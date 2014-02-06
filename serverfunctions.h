#ifndef _SERVER_FUNCTIONS_H_
#define _SERVER_FUNCTIONS_H_

#include <SFML/System.h>
#include "types.h"

/////////////////////////////////////////////////////////////////////
/// Cherche une partie prête à accueillir un nouveau joueur
///	Renvoie le thread gérant une partie manquant de joueurs, ou une nouvelle partie si toutes les actuelles sont complètes
///	Retourne NULL en cas d'impossibilité de créer une nouvelle partie
///
/// \return Le thread gérant la partie recherchée
///
////////////////////////////////////////////////////////////////////
sfThread* findRoom();

/////////////////////////////////////////////////////////////////////
/// Crée une partie vierge
///
/// \return La partie créée
///
////////////////////////////////////////////////////////////////////
Game* createRoom();

void handleNewPlayer();

void handleGame(void* room);

#endif		// _SERVER_FUNCTIONS_H_
