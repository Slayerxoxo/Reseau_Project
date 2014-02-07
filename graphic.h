#ifndef GRAFIC_H
#define GRAFIC_H

#include <stdio.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>

sfRenderWindow* creationFenetre();
void fermetureFenetre(sfRenderWindow* fenetre);

void creationBackground(sfRenderWindow* fenetre, int largeur, int hauteur);

#endif
