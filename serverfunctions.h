#ifndef _SERVER_FUNCTIONS_H_
#define _SERVER_FUNCTIONS_H_

#include <SFML/System.h>
#include "types.h"

/////////////////////////////////////////////////////////////////////
/// Cherche une partie prête à accueillir un nouveau joueur
///	Renvoie l'indice (entre 0 et MAX_ROOM_NUMBER) d'une partie manquant de joueurs, ou d'une nouvelle partie si toutes les actuelles sont complètes
///	Retourne -1 en cas d'impossibilité de créer une nouvelle partie
///
/// \return L'indice de la partie recherchée
///
////////////////////////////////////////////////////////////////////
int findRoom();

/////////////////////////////////////////////////////////////////////
/// Crée une partie vierge
///
/// \return La partie créée
///
////////////////////////////////////////////////////////////////////
Game* createRoom();

////////////////////////////////////////////////////////////////////
/// Gère l'arrivée d'un nouveau client sur le serveur
/// Effectue toutes les vérifications et initialisations nécessaires avant de passer (si possible) le contrôle au thread responsable de la partie attribuée
///
////////////////////////////////////////////////////////////////////
void handleNewPlayer(void* arg);

////////////////////////////////////////////////////////////////////
/// Gère le déroulement d'une partie du jeu
///
////////////////////////////////////////////////////////////////////
void handleGame(void* room);

#endif		// _SERVER_FUNCTIONS_H_
