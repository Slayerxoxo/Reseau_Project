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

			//Chargement des images (créer une fonction)
		sfImage *murCoin = sfImage_CreateFromFile("sprites/murCoin.png");
		if (!murCoin){
			printf("\n\nFailure to load image : \"murCoin.png\"\n\n");
		}else{
			sfSprite *murCoinSprite = sfSprite_Create();
			sfSprite_SetImage(murCoinSprite, murCoin);
			sfSprite_SetX(murCoinSprite, 0);
			sfSprite_SetY(murCoinSprite, 0); 
			sfRenderWindow_DrawSprite(mainWindow, murCoinSprite);
		}

	    sfRenderWindow_Display(mainWindow);
	}
	




	return mainWindow;
}


void fermetureFenetre(sfRenderWindow* fenetre){
	sfRenderWindow_Close(fenetre);
	sfRenderWindow_Destroy(fenetre);
}
