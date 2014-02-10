#include "clientfunctions.h"

/****************************************************/
/*       Initialisation des variables globales      */
/****************************************************/




/****************************************************/
/*      Mise à jour de la position des joueurs      */
/****************************************************/
void refreshJoueur(Position grisP, Position jauneP, Position rougeP, Position violetP){

	printf("bla");
/*
	// création de sprite
	sfSprite *sprite = sfSprite_Create();

	// affichage
	int c, l;
	
	for(c=0; c < largeur; c++) {
		for(l=0; l < hauteur; l++) {
			if(l == 0 || l == hauteur-1) {
				if(c == 0 || c == largeur-1)
					sfSprite_SetImage(sprite, murCoin);
				else
					sfSprite_SetImage(sprite, murHorizontal);
			} else {
				if(c == 0 || c == largeur-1) {
					if(l != 0 && l != hauteur-1)
						sfSprite_SetImage(sprite, murVertical);
				} else {
					if(c%2 == 0 && l%2 == 0)
						sfSprite_SetImage(sprite, obstacle);
					else
						sfSprite_SetImage(sprite, sol);
				}
			}
			
			sfSprite_Resize(sprite,(sfRenderWindow_GetWidth(fenetre)/largeur), (sfRenderWindow_GetHeight(fenetre)/hauteur));
			
			sfSprite_SetX(sprite, c*sfSprite_GetWidth(sprite));
			sfSprite_SetY(sprite, l*sfSprite_GetHeight(sprite)); 
			sfRenderWindow_DrawSprite(fenetre, sprite);
		}
	}
*/



}

/****************************************************/
/*          test si le joueur peut bouger           */
/****************************************************/
unsigned int testPosition(Position pos){

}
