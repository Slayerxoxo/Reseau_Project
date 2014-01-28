#ifndef _SERVER_FUNCTIONS_H_
#define _SERVER_FUNCTIONS_H_

#include <SFML/System.h>
#include "types.h"

/////////////////////////////////////////////////////////////////////
/// Cherche une partie prête à accueillir un nouveau joueur
///	Renvoie une partie manqauant de joueurs, ou une nouvelle partie si toutes les actuelles sont complètes
///	Retourne NULL en cas d'impossibilité de créer une nouvelle partie
///
/// \return La partie recherchée
///
////////////////////////////////////////////////////////////////////
Game* findRoom();

/////////////////////////////////////////////////////////////////////
/// Crée une partie vierge
///
/// \return La partie créée
///
////////////////////////////////////////////////////////////////////
Game* createRoom();

#endif		// _SERVER_FUNCTIONS_H_
