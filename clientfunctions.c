#include "clientfunctions.h"

/****************************************************/
/*       Initialisation des variables globales      */
/****************************************************/

	//Univers
extern	sfImage *murCoin;
extern	sfImage *murHorizontal;
extern	sfImage *murVertical;
extern	sfImage *sol;
extern	sfImage *obstacle;
	//Licornes
extern	sfImage *grisAr;
extern	sfImage *grisAv;
extern	sfImage *grisG;
extern	sfImage *grisD;
extern	sfImage *grisM;
extern	sfImage *grisV;
extern	sfImage *jauneAr;
extern	sfImage *jauneAv;
extern	sfImage *jauneG;
extern	sfImage *jauneD;
extern	sfImage *jauneM;
extern	sfImage *jauneV;
extern	sfImage *rougeAr;
extern	sfImage *rougeAv;
extern	sfImage *rougeG;
extern	sfImage *rougeD;
extern	sfImage *rougeM;
extern	sfImage *rougeV;
extern	sfImage *violetAr;
extern	sfImage *violetAv;
extern	sfImage *violetG;
extern	sfImage *violetD;
extern	sfImage *violetM;
extern	sfImage *violetV;
	//Objets
extern	sfImage *bombe;
extern	sfImage *bombeRouge;
extern	sfImage *explosionCentre;
extern	sfImage *explosionB;
extern	sfImage *explosionD;
extern	sfImage *explosionG;
extern	sfImage *explosionH;

extern sfRenderWindow* mainWindow;

/****************************************************/
/*      Mise à jour de la position des joueurs      */
/****************************************************/
void refreshJoueur(Position grisP, Position jauneP, Position rougeP, Position violetP){

	sfSprite *unicornSpriteG = sfSprite_Create();
	sfSprite_SetImage(unicornSpriteG, grisD);
	sfSprite_Resize(unicornSpriteG,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
	sfSprite_SetX(unicornSpriteG, grisP.x*sfSprite_GetWidth(unicornSpriteG));
	sfSprite_SetY(unicornSpriteG, grisP.y*sfSprite_GetHeight(unicornSpriteG));
	sfRenderWindow_DrawSprite(mainWindow, unicornSpriteG);

	sfSprite *unicornSpriteR = sfSprite_Create();
	sfSprite_SetImage(unicornSpriteR, rougeD);
	sfSprite_Resize(unicornSpriteR,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
	sfSprite_SetX(unicornSpriteR, rougeP.x*sfSprite_GetWidth(unicornSpriteR));
	sfSprite_SetY(unicornSpriteR, rougeP.y*sfSprite_GetHeight(unicornSpriteR));
	sfRenderWindow_DrawSprite(mainWindow, unicornSpriteR);

	sfSprite *unicornSpriteV = sfSprite_Create();
	sfSprite_SetImage(unicornSpriteV, violetG);
	sfSprite_Resize(unicornSpriteV,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
	sfSprite_SetX(unicornSpriteV, violetP.x*sfSprite_GetWidth(unicornSpriteV));
	sfSprite_SetY(unicornSpriteV, violetP.y*sfSprite_GetHeight(unicornSpriteV));
	sfRenderWindow_DrawSprite(mainWindow, unicornSpriteV);

	sfSprite *unicornSpriteJ = sfSprite_Create();
	sfSprite_SetImage(unicornSpriteJ, jauneG);
	sfSprite_Resize(unicornSpriteJ,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
	sfSprite_SetX(unicornSpriteJ, jauneP.x*sfSprite_GetWidth(unicornSpriteJ));
	sfSprite_SetY(unicornSpriteJ, jauneP.y*sfSprite_GetHeight(unicornSpriteJ));
	sfRenderWindow_DrawSprite(mainWindow, unicornSpriteJ);
}

/****************************************************/
/*          test si le joueur peut bouger           */
/****************************************************/
unsigned int testPosition(Position pos){

}
