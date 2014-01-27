#include "graphic.h"

sfRenderWindow* creationFenetre(){

	/* Affichage de la fenêtre */ 
	sfWindowSettings windowSettings = {24, 8, 0};
	sfVideoMode videoMode = {800, 600, 32};
	sfRenderWindow* mainWindow;
	mainWindow = sfRenderWindow_Create(videoMode, "SFML window", sfResize | sfClose, windowSettings);
	return mainWindow;
}

void fermetureFenetre(sfRenderWindow* fenetre){
	sfRenderWindow_Close(fenetre);
	sfRenderWindow_Destroy(fenetre);
}
