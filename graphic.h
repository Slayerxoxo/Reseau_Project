#ifndef GRAFIC_H
#define GRAFIC_H

#include <stdio.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include "types.h"
#include "def.h"
#include "clientfunctions.h"

/* Gestion de la fenêtre */
sfRenderWindow* creationFenetre();
void fermetureFenetre(sfRenderWindow* fenetre);

/* Gestion de la map */
void creationBackground(sfRenderWindow* fenetre, int largeur, int hauteur);

/* Gestion des chargements */
void chargementImages();
void initialisation();

#endif
