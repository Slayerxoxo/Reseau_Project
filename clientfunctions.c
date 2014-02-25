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
void refreshJoueur(Player playerTab[], int sizeTab){

	sfSprite *unicornSprite = sfSprite_Create();
	Position tempPos;
	sfImage *tempImage;
	int lifeCount;
	int i, j;

	for (i=0; i<sizeTab; i++){
		//Maj la position du joueur
		tempPos = playerTab[i].position;
		//Maj la direction du joueur
			//Vérifie s'il est mort
			if (playerTab[i].lives < 1){
				switch(i)
				{
					case 0 :
						tempImage = grisM; break;
					case 1 :
						tempImage = rougeM;	break;
					case 2 :
						tempImage = jauneM; break;
					case 3 :
						tempImage = violetM; break;
					default:
						;
				}
			}
			else{
				//Vérifie s'il y a victoire
				lifeCount = 1;
				for (j=1; j<sizeTab; j++){
					if (playerTab[j].lives > 0){
						lifeCount = 0;
					}
				}
				if (lifeCount == 1){
					switch(i)
					{
						case 0 :
							tempImage = grisV; break;
						case 1 :
							tempImage = rougeV; break;
						case 2 :
							tempImage = jauneV; break;
						case 3 :
							tempImage = violetV; break;
						default:
							;
					}
				}
				else{	//Prend la bonne direction
					switch(i)
					{
						case 0 :
							switch(playerTab[i].looking)
							{
								case LEFT :
									tempImage = grisG; break;
								case RIGHT :
									tempImage = grisD; break;
								case UP :
									tempImage = grisAr; break;
								case DOWN :
									tempImage = grisAv; break;
								default:
									;
							} break;
						case 1 :
							switch(playerTab[i].looking)
							{
								case LEFT :
									tempImage = rougeG; break;
								case RIGHT :
									tempImage = rougeD; break;
								case UP :
									tempImage = rougeAr; break;
								case DOWN :
									tempImage = rougeAv; break;
								default:
									;
							} break;
						case 2 :
							switch(playerTab[i].looking)
							{
								case LEFT :
									tempImage = jauneG; break;
								case RIGHT :
									tempImage = jauneD; break;
								case UP :
									tempImage = jauneAr; break;
								case DOWN :
									tempImage = jauneAv; break;
								default:
									;
							} break;
						case 3 :
							switch(playerTab[i].looking)
							{
								case LEFT :
									tempImage = violetG; break;
								case RIGHT :
									tempImage = violetD; break;
								case UP :
									tempImage = violetAr; break;
								case DOWN :
									tempImage = violetAv; break;
								default:
									;
							} break;
					}
				}
			}
		sfSprite_SetImage(unicornSprite, tempImage);
		sfSprite_Resize(unicornSprite,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
		sfSprite_SetX(unicornSprite, tempPos.x*sfSprite_GetWidth(unicornSprite));	
		sfSprite_SetY(unicornSprite, tempPos.y*sfSprite_GetHeight(unicornSprite)); 
		sfRenderWindow_DrawSprite(mainWindow, unicornSprite);
	}
	
}

int responseIsForMe(char response[], int responseSize, char pseudo[], int pseudoSize) {
	return 1;
}
