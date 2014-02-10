#include "graphic.h"

/****************************************************/
/*       Initialisation des variables globales      */
/****************************************************/

	//Univers
	sfImage *murCoin;
	sfImage *murHorizontal;
	sfImage *murVertical;
	sfImage *sol;
	sfImage *obstacle;
	//Licornes
	sfImage *grisAr;
	sfImage *grisAv;
	sfImage *grisG;
	sfImage *grisD;
	sfImage *grisM;
	sfImage *grisV;
	sfImage *jauneAr;
	sfImage *jauneAv;
	sfImage *jauneG;
	sfImage *jauneD;
	sfImage *jauneM;
	sfImage *jauneV;
	sfImage *rougeAr;
	sfImage *rougeAv;
	sfImage *rougeG;
	sfImage *rougeD;
	sfImage *rougeM;
	sfImage *rougeV;
	sfImage *violetAr;
	sfImage *violetAv;
	sfImage *violetG;
	sfImage *violetD;
	sfImage *violetM;
	sfImage *violetV;
	//Objets
	sfImage *bombe;
	sfImage *bombeRouge;
	sfImage *explosionCentre;
	sfImage *explosionB;
	sfImage *explosionD;
	sfImage *explosionG;
	sfImage *explosionH;

/****************************************************/
/*            Création de la fenêtre                */
/****************************************************/

sfRenderWindow* creationFenetre(){

	/* Affichage de la fenêtre */ 
	sfWindowSettings windowSettings = {24, 8, 0};
	sfVideoMode videoMode = {700, 700, 32};
	sfRenderWindow* mainWindow = sfRenderWindow_Create(videoMode, "Bomber Unicorn !", sfClose, windowSettings);
	sfRenderWindow_SetFramerateLimit(mainWindow,60);

	chargementImages();

	/* Gestion des évènements */

	while(sfRenderWindow_IsOpened(mainWindow)){
		sfEvent Event;
        while (sfRenderWindow_GetEvent(mainWindow, &Event)){

			//Fermeture de la fenêtre
            if (Event.Type == sfEvtClosed){
                sfRenderWindow_Close(mainWindow);
			}
		}

		//Affichage
		sfRenderWindow_Clear(mainWindow, sfBlack);	//Remplissage de l'écran par un fond noir

		creationBackground(mainWindow, 11, 11);		//Création de la carte

		Position grisP, jauneP, rougeP, violetP;

		grisP.x = 0;
		grisP.y = 0;
		jauneP.x = 10;
		jauneP.y = 10;
		rougeP.x = 0;
		rougeP.y = 10;
		violetP.x =10;
		violetP.y = 0;

		refreshJoueur(grisP, jauneP, rougeP, violetP);

	    sfRenderWindow_Display(mainWindow);
	}

	return mainWindow;
}


/****************************************************/
/*            Fermeture de la fenêtre               */
/****************************************************/

void fermetureFenetre(sfRenderWindow* fenetre){
	sfRenderWindow_Close(fenetre);
	sfRenderWindow_Destroy(fenetre);
}


/****************************************************/
/*               Création de la map                 */
/****************************************************/

void creationBackground(sfRenderWindow* fenetre, int largeur, int hauteur) {

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


/****************************************************/
/*      Chargement et vérification des images       */
/****************************************************/

void chargementImages(){

	//Univers
	murCoin = sfImage_CreateFromFile("sprites/murCoin.png");
	if (!murCoin){
		printf("\n\nFailure to load image : \"murCoin.png\"\n\n");
	}
	murHorizontal = sfImage_CreateFromFile("sprites/murHB.png");
	if (!murHorizontal){
		printf("\n\nFailure to load image : \"murHB.png\"\n\n");
	}
	murVertical = sfImage_CreateFromFile("sprites/murGD.png");
	if (!murVertical){
		printf("\n\nFailure to load image : \"murGD.png\"\n\n");
	}
	sol = sfImage_CreateFromFile("sprites/sol.png");
	if (!sol){
		printf("\n\nFailure to load image : \"sol.png\"\n\n");
	}
	obstacle = sfImage_CreateFromFile("sprites/murObstacle.png");
	if (!obstacle){
		printf("\n\nFailure to load image : \"murObstacle.png\"\n\n");
	}
	//Licornes
	grisAr = sfImage_CreateFromFile("sprites/grisAr.png");
	if (!grisAr){
		printf("\n\nFailure to load image : \"grisAr.png\"\n\n");
	}
	grisAv = sfImage_CreateFromFile("sprites/grisAv.png");
	if (!grisAv){
		printf("\n\nFailure to load image : \"grisAv.png\"\n\n");
	}
	grisG = sfImage_CreateFromFile("sprites/grisG.png");
	if (!grisG){
		printf("\n\nFailure to load image : \"grisG.png\"\n\n");
	}
	grisD = sfImage_CreateFromFile("sprites/grisD.png");
	if (!grisD){
		printf("\n\nFailure to load image : \"grisD.png\"\n\n");
	}
	grisM = sfImage_CreateFromFile("sprites/grisM.png");
	if (!grisM){
		printf("\n\nFailure to load image : \"grisM.png\"\n\n");
	}
	grisV = sfImage_CreateFromFile("sprites/grisV.png");
	if (!grisV){
		printf("\n\nFailure to load image : \"grisV.png\"\n\n");
	}
	jauneAr = sfImage_CreateFromFile("sprites/jauneAr.png");
	if (!jauneAr){
		printf("\n\nFailure to load image : \"jauneAr.png\"\n\n");
	}
	jauneAv = sfImage_CreateFromFile("sprites/jauneAv.png");
	if (!jauneAv){
		printf("\n\nFailure to load image : \"jauneAv.png\"\n\n");
	}
	jauneG = sfImage_CreateFromFile("sprites/jauneG.png");
	if (!jauneG){
		printf("\n\nFailure to load image : \"jauneG.png\"\n\n");
	}
	jauneD = sfImage_CreateFromFile("sprites/jauneD.png");
	if (!jauneD){
		printf("\n\nFailure to load image : \"jauneD.png\"\n\n");
	}
	jauneM = sfImage_CreateFromFile("sprites/jauneM.png");
	if (!jauneM){
		printf("\n\nFailure to load image : \"jauneM.png\"\n\n");
	}
	jauneV = sfImage_CreateFromFile("sprites/jauneV.png");
	if (!jauneV){
		printf("\n\nFailure to load image : \"jauneV.png\"\n\n");
	}
	rougeAr = sfImage_CreateFromFile("sprites/rougeAr.png");
	if (!rougeAr){
		printf("\n\nFailure to load image : \"rougeAr.png\"\n\n");
	}
	rougeAv = sfImage_CreateFromFile("sprites/rougeAv.png");
	if (!rougeAv){
		printf("\n\nFailure to load image : \"rougeAv.png\"\n\n");
	}
	rougeG = sfImage_CreateFromFile("sprites/rougeG.png");
	if (!rougeG){
		printf("\n\nFailure to load image : \"rougeG.png\"\n\n");
	}
	rougeD = sfImage_CreateFromFile("sprites/rougeD.png");
	if (!rougeD){
		printf("\n\nFailure to load image : \"rougeD.png\"\n\n");
	}
	rougeM = sfImage_CreateFromFile("sprites/rougeM.png");
	if (!rougeM){
		printf("\n\nFailure to load image : \"rougeM.png\"\n\n");
	}
	rougeV = sfImage_CreateFromFile("sprites/rougeV.png");
	if (!rougeV){
		printf("\n\nFailure to load image : \"rougeV.png\"\n\n");
	}
	violetAr = sfImage_CreateFromFile("sprites/violetAr.png");
	if (!violetAr){
		printf("\n\nFailure to load image : \"violetAr.png\"\n\n");
	}
	violetAv = sfImage_CreateFromFile("sprites/violetAv.png");
	if (!violetAv){
		printf("\n\nFailure to load image : \"violetAv.png\"\n\n");
	}
	violetG = sfImage_CreateFromFile("sprites/violetG.png");
	if (!violetG){
		printf("\n\nFailure to load image : \"violetG.png\"\n\n");
	}
	violetD = sfImage_CreateFromFile("sprites/violetD.png");
	if (!violetD){
		printf("\n\nFailure to load image : \"violetD.png\"\n\n");
	}
	violetM = sfImage_CreateFromFile("sprites/violetM.png");
	if (!violetM){
		printf("\n\nFailure to load image : \"violetM.png\"\n\n");
	}
	violetV = sfImage_CreateFromFile("sprites/violetV.png");
	if (!violetV){
		printf("\n\nFailure to load image : \"violetV.png\"\n\n");
	}
	//Objets
	bombe = sfImage_CreateFromFile("sprites/bombe.png");
	if (!bombe){
		printf("\n\nFailure to load image : \"bombe.png\"\n\n");
	}
	bombeRouge = sfImage_CreateFromFile("sprites/bombeRouge.png");
	if (!bombeRouge){
		printf("\n\nFailure to load image : \"bombeRouge.png\"\n\n");
	}
	explosionCentre = sfImage_CreateFromFile("sprites/explosionCentre.png");
	if (!explosionCentre){
		printf("\n\nFailure to load image : \"explosionCentre.png\"\n\n");
	}
	explosionB = sfImage_CreateFromFile("sprites/explosionB.png");
	if (!explosionB){
		printf("\n\nFailure to load image : \"explosionB.png\"\n\n");
	}
	explosionD = sfImage_CreateFromFile("sprites/explosionD.png");
	if (!explosionD){
		printf("\n\nFailure to load image : \"explosionD.png\"\n\n");
	}
	explosionG = sfImage_CreateFromFile("sprites/explosionG.png");
	if (!explosionG){
		printf("\n\nFailure to load image : \"explosionG.png\"\n\n");
	}
	explosionH = sfImage_CreateFromFile("sprites/explosionH.png");
	if (!explosionH){
		printf("\n\nFailure to load image : \"explosionH.png\"\n\n");
	}
}


