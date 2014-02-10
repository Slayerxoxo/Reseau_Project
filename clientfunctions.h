#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H

#include <stdio.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include "types.h"
#include "graphic.h"

void refreshJoueur(Position grisP, Position jauneP, Position rougeP, Position violetP);
unsigned int testPosition(Position pos);

#endif
