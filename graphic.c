#include "graphic.h"

sfRenderWindow* creationFenetre(){

	/* Affichage de la fenêtre */ 
	sfWindowSettings windowSettings = {24, 8, 0};
	sfVideoMode videoMode = {800, 600, 32};
	sfRenderWindow* mainWindow = sfRenderWindow_Create(videoMode, "SFML window", sfResize | sfClose, windowSettings);

	/* Création de la carte */
	sfImage *murCoin = sfImage_CreateFromFile("sprites/murCoin.png");
	if (!murCoin)
	{
		printf("\n\nFailure to load image : \"murCoin.png\"\n\n");
	}
	sfSprite *murCoinSprite = sfSprite_Create();
	sfSprite_SetImage(murCoinSprite, murCoin);
	sfSprite_SetX(murCoinSprite, 0);
	sfSprite_SetY(murCoinSprite, 0);

    sfRenderWindow_Display(mainWindow);

	return mainWindow;
}

void fermetureFenetre(sfRenderWindow* fenetre){
	sfRenderWindow_Close(fenetre);
	sfRenderWindow_Destroy(fenetre);
}
