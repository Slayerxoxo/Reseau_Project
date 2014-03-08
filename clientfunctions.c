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
void refreshJoueur(Player* playerTab[], int sizeTab){

	sfSprite *unicornSprite = sfSprite_Create();
	sfSprite *bombSprite = sfSprite_Create();
	Position tempPos;
	Position tempPosBomb;
	sfImage *tempImage;
	sfImage *tempImageBomb;
	int lifeCount;
	int i, j, k;

	for (i=0; i<sizeTab; i++){
		//Maj la position du joueur
		tempPos = playerTab[i]->position;
		//Maj la direction du joueur
			//Vérifie s'il est mort
			if (playerTab[i]->lives < 1){
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
					if (playerTab[j]->lives > 0){
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
							switch(playerTab[i]->looking)
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
							switch(playerTab[i]->looking)
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
							switch(playerTab[i]->looking)
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
							switch(playerTab[i]->looking)
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
		//On vérifie et place les bombes
		for(k=0; k<3; k++){
			tempPosBomb = playerTab[i]->bombs[k].position;
			if(playerTab[i]->bombs[k].state != IDLE){
				switch(playerTab[i]->bombs[k].state){
					case COUNTING1 :
									tempImageBomb = bombe; break;
					case COUNTING2 :
									tempImageBomb = bombe; break;
					case COUNTING3 :
									tempImageBomb = bombe; break;
					case COUNTING4 :
									tempImageBomb = bombe; break;
					case RED :
									tempImageBomb = bombeRouge; break;
					case EXPLODING :
									tempImageBomb = explosionCentre; break;
					default:
						;
				}
			//On affiche les sprites des bombes
				sfSprite_SetImage(bombSprite, tempImageBomb);
				sfSprite_Resize(bombSprite,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
				sfSprite_SetX(bombSprite, (tempPosBomb.x+1)*sfSprite_GetWidth(bombSprite));	
				sfSprite_SetY(bombSprite, (tempPosBomb.y+1)*sfSprite_GetHeight(bombSprite)); 
				sfRenderWindow_DrawSprite(mainWindow, bombSprite);
				// Si la bombe explose, on affiche aussi les explosions
				if(tempImageBomb == explosionCentre) {
					// Le bas
					sfSprite_SetImage(bombSprite, explosionB);
					sfSprite_Resize(bombSprite,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
					sfSprite_SetX(bombSprite, (tempPosBomb.x+1)*sfSprite_GetWidth(bombSprite));	
					sfSprite_SetY(bombSprite, (tempPosBomb.y+2)*sfSprite_GetHeight(bombSprite)); 
					sfRenderWindow_DrawSprite(mainWindow, bombSprite);
					// Le haut
					sfSprite_SetImage(bombSprite, explosionH);
					sfSprite_Resize(bombSprite,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
					sfSprite_SetX(bombSprite, (tempPosBomb.x+1)*sfSprite_GetWidth(bombSprite));	
					sfSprite_SetY(bombSprite, (tempPosBomb.y)*sfSprite_GetHeight(bombSprite)); 
					sfRenderWindow_DrawSprite(mainWindow, bombSprite);
					// La gauche
					sfSprite_SetImage(bombSprite, explosionG);
					sfSprite_Resize(bombSprite,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
					sfSprite_SetX(bombSprite, (tempPosBomb.x)*sfSprite_GetWidth(bombSprite));	
					sfSprite_SetY(bombSprite, (tempPosBomb.y+1)*sfSprite_GetHeight(bombSprite)); 
					sfRenderWindow_DrawSprite(mainWindow, bombSprite);
					// La droite
					sfSprite_SetImage(bombSprite, explosionD);
					sfSprite_Resize(bombSprite,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
					sfSprite_SetX(bombSprite, (tempPosBomb.x+2)*sfSprite_GetWidth(bombSprite));	
					sfSprite_SetY(bombSprite, (tempPosBomb.y+1)*sfSprite_GetHeight(bombSprite)); 
					sfRenderWindow_DrawSprite(mainWindow, bombSprite);
				}
			}
		}
		//On affiche les sprites des joueurs
		sfSprite_SetImage(unicornSprite, tempImage);
		sfSprite_Resize(unicornSprite,(sfRenderWindow_GetWidth(mainWindow)/LARGEUR), (sfRenderWindow_GetHeight(mainWindow)/HAUTEUR));
		sfSprite_SetX(unicornSprite, (tempPos.x+1)*sfSprite_GetWidth(unicornSprite));	
		sfSprite_SetY(unicornSprite, (tempPos.y+1)*sfSprite_GetHeight(unicornSprite)); 
		sfRenderWindow_DrawSprite(mainWindow, unicornSprite);
	}
}

void readBuffer(char receptionBuffer[128]){
	
	
}

int responseIsForMe(char response[], int responseSize, char pseudo[], int pseudoSize) {
	return 1;
}

void stringToPlayers(char* playerString, Player* playersTab[], int tabSize) {
	char* playerToken = strtok(playerString, "/");
	char* itemToken;
	int currentIndex = 0;
	int i;
	char* playerTokens[MAX_PLAYER_NUMBER];
	
	
	while((playerToken != NULL) && (currentIndex < tabSize)) {
		playerTokens[currentIndex] = playerToken;
		playerToken = strtok(NULL, "/");	
		currentIndex+=1;
	}
	
	currentIndex = 0;
	
	while(currentIndex < tabSize) {
		// les vies
		itemToken = strtok(playerTokens[currentIndex], ";");
		playersTab[currentIndex]->lives = atoi(itemToken);
		// la direction
		itemToken = strtok(NULL, ";");
		if(strcmp(itemToken,"LEFT") == 0)
			playersTab[currentIndex]->looking = LEFT;
		if(strcmp(itemToken,"RIGHT") == 0)
			playersTab[currentIndex]->looking = RIGHT;
		if(strcmp(itemToken,"UP") == 0)
			playersTab[currentIndex]->looking = UP;
		if(strcmp(itemToken,"DOWN") == 0)
			playersTab[currentIndex]->looking = DOWN;	
		// la position
		itemToken = strtok(NULL, ";");
		playersTab[currentIndex]->position.x = atoi(itemToken);
		itemToken = strtok(NULL, ";");
		playersTab[currentIndex]->position.y = atoi(itemToken);
		// les bombes
		for(i=0; i< MAX_BOMB_NUMBER; i++) {
			// l'état
			itemToken = strtok(NULL, ";");
			if(strcmp(itemToken, "i") == 0)
				playersTab[currentIndex]->bombs[i].state = IDLE;
			if(strcmp(itemToken, "c1") == 0)
				playersTab[currentIndex]->bombs[i].state = COUNTING1;
			if(strcmp(itemToken, "c2") == 0)
				playersTab[currentIndex]->bombs[i].state = COUNTING2;
			if(strcmp(itemToken, "c3") == 0)
				playersTab[currentIndex]->bombs[i].state = COUNTING3;
			if(strcmp(itemToken, "c4") == 0)
				playersTab[currentIndex]->bombs[i].state = COUNTING4;
			if(strcmp(itemToken, "r") == 0)
				playersTab[currentIndex]->bombs[i].state = RED;
			if(strcmp(itemToken, "e") == 0)
				playersTab[currentIndex]->bombs[i].state = EXPLODING;
			// la position
			itemToken = strtok(NULL, ";");
			playersTab[currentIndex]->bombs[i].position.x = atoi(itemToken);
			itemToken = strtok(NULL, ";");
			playersTab[currentIndex]->bombs[i].position.y = atoi(itemToken);
		}
	
		currentIndex+=1;
	}
}

int playersInString(char* playerString) {
	int i, compteur = 0;

	for(i = 0 ; playerString[i] != '\0'; i++) 
		if (playerString[i] == '/')
			compteur++;
	return compteur+1;
}
