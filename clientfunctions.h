#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H

#include <stdio.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include "types.h"
#include "graphic.h"
#include "def.h"

void refreshJoueur(Player playerTab[], int sizeTab);

int responseIsForMe(char response[], int responseSize, char pseudo[], int pseudoSize);

#endif
