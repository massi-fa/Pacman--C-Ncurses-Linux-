#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include <pthread.h>

#define PASSO 1 					/* Entita spostamento della fantasma */
#define SU 65 						/* Freccia su */
#define GIU 66 						/* Freccia giu */
#define SINISTRA 68				/* Freccia sinsitra */
#define DESTRA 67					/* Freccia destra */
#define SPAZIO 32         /*BARRA SPAZIATRICE PER SPARO*/
#define MAXX 29
#define MAXY 25
#define VITE 20
#define DIREZIONI 4

/* define per fantasmi*/
#define NUMEROFANTASMI 5
#define NUMEROSPAWN 1
#define GENERAZIONENUMERO 6

/* define per spari*/
#define NUMEROSPARI 4

/*VARIABILI PER STAMPA PUNTEGGIO*/
#define PUNTIX 0
#define PUNTIY 24
#define YVITEFANTASMA 27





/*STRUTTURA USATA PER L POSIZIONE DEGLI OGGETTI DI GIOCO*/
typedef struct
{
  int x;
  int y;
  int direzione;
}pos;
/*STRUTTURA PER PACMAN*/
typedef struct
{
  pos * posizione;
  int  colore;
  int  num_vite;
  _Bool invincibilita;
  pthread_mutex_t* mutex;
}par;
/*STRUTTURA FANTASMI*/
typedef struct
{
  pos * posizione;
  int  colore;
  int num_vite;
  int id_fantasma;
  _Bool fine_generazione;
  _Bool rimbalzo;
    _Bool invincibilita;
  _Bool morto;
  _Bool game;
  pthread_mutex_t* mutex;
}fant;
/*STRUTTURA SPARI*/
typedef struct
{
  pos * posizione;
  int  colore;
  int  provenienza;
  int proprietario;
  _Bool contatto;
  pthread_mutex_t* mutex;
}spar;

int numero_casuale(int min,int max);
_Bool controllo_collisione_sp_ogg(int x_sparo,int y_sparo,int direzione,int xogg,int yogg);
int rimbalzi(int y,int x,int direzione);
_Bool controllo_rimbalzi(int y,int x,int direzione,int y1 ,int x1 );
