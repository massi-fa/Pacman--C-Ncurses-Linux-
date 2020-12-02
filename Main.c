#include "Main.h"

/*FUNZIONE USATA PER GENERARE CASUALMENTE UN NUMERO*/
int numero_casuale(int min,int max){
  int risultato;
  risultato=min+rand()%max;
  return risultato;
}
/*FUNZIONE CHE CONTROLLA LE COLLISIONI DEGLI SPARI*/
_Bool controllo_collisione_sp_ogg(int x_sparo,int y_sparo,int direzione,int xogg,int yogg){
  int cn_su,cn_giu,cn_dx,cn_sx;
  /*USO VARIABILI AUSILIARI PER IL CONTRLLO DELLA POSIZIONE*/
  cn_su=y_sparo-1;
  cn_giu=y_sparo+1;
  cn_sx=x_sparo-1;
  cn_dx=x_sparo+1;

  if(direzione==1){
    if(cn_su==yogg&&x_sparo==xogg){
      return true;
    }
  }
  if(direzione==2){
    if(cn_giu==yogg&&x_sparo==xogg){
      return true;
    }
  }
  if(direzione==3){
    if(cn_sx==xogg&&y_sparo==yogg){
      return true;
    }
  }
  if(direzione==4){
    if(cn_dx==xogg&&y_sparo==yogg){
      return true;
    }
  }
  return false;
}
/*FUNZIONE CHE CONTROLLA I RIMBALZI DEI FANTASMI*/
int rimbalzi(int y,int x,int direzione_save){
  int opposto_save,risultato;
  int cn_su,cn_giu,cn_dx,cn_sx,counter=0,direzione[4];
  /*CALCOLO LA DIREZINE OPPOSTA*/
    if(direzione_save==1){
      opposto_save=2;
    }
    if(direzione_save==2){
      opposto_save=1;
    }
    if(direzione_save==3){
      opposto_save=4;
    }
    if(direzione_save==4){
      opposto_save=3;
    }
    return opposto_save;


}
_Bool controllo_rimbalzi(int y,int x,int direzione,int y1 ,int x1){
    int controllo,controllo1;
    switch(direzione){
      case 1:
        y=y-1;
        break;
      case 2:
        y=y+1;
        break;
      case 3:
        x=x-1;
        break;
      case 4:
        x=x+1;
        break;
    }
    if(x==x1){
      if(y==y1){
        return true;
      }
    }
    return false;
}
