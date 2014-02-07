#include "graphic.h"

sfRenderWindow* creationFenetre(){

	/* Affichage de la fenêtre */ 
	sfWindowSettings windowSettings = {24, 8, 0};
	sfVideoMode videoMode = {700, 700, 32};
	sfRenderWindow* mainWindow = sfRenderWindow_Create(videoMode, "SFML window", sfResize | sfClose, windowSettings);

	while(sfRenderWindow_IsOpened(mainWindow)){
		sfEvent Event;
        while (sfRenderWindow_GetEvent(mainWindow, &Event)){

			//Fermeture de la fenêtre
            if (Event.Type == sfEvtClosed){
                sfRenderWindow_Close(mainWindow);
			}
		}

		//Remplissage de l'écran par un fond noir
		sfRenderWindow_Clear(mainWindow, sfBlack);


		//* Création de la carte *

			/*/Chargement des images (créer une fonction)
		sfImage *murCoin = sfImage_CreateFromFile("sprites/murCoin.png");
		if (!murCoin){
			printf("\n\nFailure to load image : \"murCoin.png\"\n\n");
		}else{
			sfSprite *murCoinSprite = sfSprite_Create();
			sfSprite_SetImage(murCoinSprite, murCoin);
			sfSprite_SetX(murCoinSprite, 0);
			sfSprite_SetY(murCoinSprite, 0); 
			sfRenderWindow_DrawSprite(mainWindow, murCoinSprite);
		}*/creationBackground(mainWindow, 11, 11);

	    sfRenderWindow_Display(mainWindow);
	}
	




	return mainWindow;
}


void fermetureFenetre(sfRenderWindow* fenetre){
	sfRenderWindow_Close(fenetre);
	sfRenderWindow_Destroy(fenetre);
}

void creationBackground(sfRenderWindow* fenetre, int largeur, int hauteur) {
	// chargement des images
	sfImage *murCoin = sfImage_CreateFromFile("sprites/murCoin.png");
	sfImage *murHorizontal = sfImage_CreateFromFile("sprites/murHB.png");
	sfImage *murVertical = sfImage_CreateFromFile("sprites/murGD.png");
	sfImage *sol = sfImage_CreateFromFile("sprites/sol.png");
	sfImage *obstacle = sfImage_CreateFromFile("sprites/murObstacle.png");
	// vérifications de la réussite des chargements
	if (!murCoin){
		printf("\n\nFailure to load image : \"murCoin.png\"\n\n");
		return;
	}
	if (!murHorizontal){
		printf("\n\nFailure to load image : \"murHB.png\"\n\n");
		return;
	}
	if (!murVertical){
		printf("\n\nFailure to load image : \"murGD.png\"\n\n");
		return;
	}
	if (!sol){
		printf("\n\nFailure to load image : \"sol.png\"\n\n");
		return;
	}
	if (!obstacle){
		printf("\n\nFailure to load image : \"murObstacle.png\"\n\n");
		return;
	}
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
}
