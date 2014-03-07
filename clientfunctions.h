#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H

#include <stdio.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include "types.h"
#include "graphic.h"
#include "def.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////
/// Actualise ou place les sprites des joueurs et des bombes
///
////////////////////////////////////////////////////////////////////
void refreshJoueur(Player* playerTab[], int sizeTab);

void readBuffer(char receptionBuffer[128]);

int responseIsForMe(char response[], int responseSize, char pseudo[], int pseudoSize);

void stringToPlayers(char* playerString, Player* playersTab[], int tabSize);

int playersInString(char* playerString);

#endif
