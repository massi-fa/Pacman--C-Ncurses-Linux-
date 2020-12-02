#include "Main.h"


/*FUNZIONI PER THREAD*/
int movimentiGhost(int y,int x,int direzione_save);
_Bool controllo_respawn(int y,int x,int direzione);
_Bool controllo_spari(int x,int y, int dir);
_Bool posso(int x,int y,int moves);


/*DICHIARAZIONI FUNZIONI PACMAN*/
void * pacman(void * param);


/*SPARI PACMAN*/
void * sparo(void * param);


 /*FUNZIONI PER FANTASMI*/
 void * fantasma(void * param);



 /*SPARI FANTASMI*/
 void * sparo_fantasma(void * param);


 /*DICHIARAZIONI FUNZIONI GIOCO*/
 void menu_gioco();
 void areaGioco();
