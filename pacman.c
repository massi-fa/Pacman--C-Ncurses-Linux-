#include "pacman.h"
char mappa[MAXY][MAXX]={
 "###########################",
 "#.    .     .#.     .    .#",
 "# #### ##### # ##### #### #",
 "# #### ##### # ##### #### #",
 "#.    .  .  . .  .  .    .#",
 "# #### ## ####### ## #### #",
 "#.    .##.  . .  .##.    .#",
 "###### ##### # ##### ######",
 "     # ##.  . .  .## #     ",
 "     # ## !-----? ## #     ",
 "###### ## |     | ## ######",
 "      .  .|     |.  .      ",
 "###### ## |     | ## ######",
 "     # ## $-----& ## #     ",
 "     # ##.       .## #     ",
 "###### ## ####### ## ######",
 "#.    .  .  .#.  .  .    .#",
 "# #### ##### # ##### #### #",
 "#. .##.  .  . .  .  .##. .#",
 "### ## ## ####### ## ## ###",
 "#. .  .##.  .#.  .##.  . .#",
 "# ########## # ########## #",
 "#.          . .          .#",
 "###########################"};
  char mappa_punti[MAXY][MAXX]={
   "                           ",
   " ............ ............ ",
   " .    .     . .     .    . ",
   " .    .     . .     .    . ",
   " ......................... ",
   " .    .  .       .  .    . ",
   " ......  .........  ...... ",
   "      .     . .     .      ",
   "      .  .........  .      ",
   "      .  .       .  .      ",
   "      .  .       .  .      ",
   "..........       ..........",
   "      .  .       .  .      ",
   "      .  .       .  .      ",
   "      .  .........  .      ",
   "      .  .       .  .      ",
   " ............ ............ ",
   " .    .     . .     .    . ",
   " ...  ....... .......  ... ",
   "   .  .  .       .  .  .   ",
   " ......  .... ....  ...... ",
   " .          . .          . ",
   " ......................... ",
   "                           "};


    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*creo e inizializzo il semaforo*/
   /*STRUTTURE PER PACMAN*/
     pthread_t pacmanID;
     pos pos_pacman;
     par par_pacman;
    int punti_game=0;
   /*STRUTTURE SPARI PACMAN*/
    pthread_t spariID[NUMEROSPARI];
    pos pos_spari[NUMEROSPARI];
    spar par_spari[NUMEROSPARI];
   /*STRUTTURE SPARI FANTASMA*/
    pthread_t fantasmaID[NUMEROFANTASMI];
    fant fant_fantasma[NUMEROFANTASMI];
    pos pos_fantasma[NUMEROFANTASMI];
   /*STRUTTURE SPARI FANTASMA*/
    pthread_t spari_fantasmaID[NUMEROFANTASMI][NUMEROSPARI];
    pos pos_spari_fantasmi[NUMEROFANTASMI][NUMEROSPARI];
    spar par_spari_fantasmi[NUMEROFANTASMI][NUMEROSPARI];

    _Bool spari_pacman=false,posso_sparare=true;
    int contatore_spari_pacman=NUMEROSPARI;
    _Bool finespari[NUMEROSPARI];
    int contatore_fantasmi_morti=0;
    int counter_fantasmi=0;
    int posso_sparare_fantasmi[NUMEROFANTASMI];
    int tempo_spari[NUMEROFANTASMI];
    int numero_fantasmi_game=0;
    int vite_pacman_game=0;
    int tempo_respan_fant_game=0;
    int armatura=0;
    int  vite_fantasma_game=0;
    int tempo_spari_fantasma=0;
    int tempo_massimo=0;
    _Bool finespari_fantasma[NUMEROFANTASMI][NUMEROSPARI];


    int main()
    {
      srand(time(NULL));
      initscr();
      start_color();
      nodelay(stdscr,true);
      noecho();
      curs_set(0);

      /*COLORE MAPPA*/
      init_pair(1,COLOR_BLUE,COLOR_BLUE);
      /*COLORE PUNTI GIOCO*/
      init_pair(15,COLOR_BLACK,COLOR_RED);
      /*COLORE PACMAN*/
      init_pair(2,COLOR_BLACK,COLOR_YELLOW);
      /*COLRE FANTSAMI*/
      init_pair(3,COLOR_BLACK,COLOR_RED);
      init_pair(4,COLOR_BLACK,COLOR_GREEN);
      init_pair(5,COLOR_BLACK,COLOR_CYAN);
      init_pair(6,COLOR_BLACK,COLOR_MAGENTA);
      init_pair(7,COLOR_BLACK,COLOR_WHITE);

      /*COLRE FANTSAMI RESPAWNED*/
      init_pair(16,COLOR_BLACK,COLOR_WHITE);
      /*COLORE PROIETTILI FANTASMA*/
      init_pair(17,COLOR_RED,COLOR_BLACK);
      /*COLORI MENU*/
      init_pair(18,COLOR_RED,COLOR_BLACK);


      fant_fantasma[0].colore=3;
      fant_fantasma[1].colore=4;
      fant_fantasma[2].colore=5;
      fant_fantasma[3].colore=6;
      fant_fantasma[4].colore=7;

      menu_gioco();

      /*INIZIALIZZAZIONE THREAD PACMAN*/
      par_pacman.num_vite=vite_pacman_game;
      par_pacman.mutex=&mutex;
      par_pacman.invincibilita=false;
      par_pacman.posizione=&pos_pacman;
      par_pacman.colore=2;


      /*INVOCAIONE AREA GIOCO*/
      areaGioco();
      //pthread_join(pacmanID,NULL);  /* abbiano terminato la loro esecuzione       */

      //mvprintw(MAXY/2-1,(MAXX/2)-5,"GAME OVER!");//Stampo la scritta "GAME OVER!" al centro dello schermo
      //mvprintw(MAXY/2,(MAXX/2)-11,"premi invio per uscire");
      //while (getch()!='\n');//Aspetto che l'utente prema invio
      endwin();//Ripristino il normale funzionamento del terminale
      return 0;
    }
    void menu_gioco(){
      int i,movimento_up_down=0,movimentoLRVP=0,movimentoLRVF=0,movimentoLRSF=0,movimentoLRT=0,movimentoLRTR=0,movimentoLRNF=0,movimentoLRTM=0;
      int vitePacman[3]={3,6,10},viteFantasmi[3]={1,2,6},sparifantasmi[3]={3,5,7},tolleranza[3]={10,15,20},tempoMAXRESPAWN[3]={4,6,10},numeroFant[3]={3,4,5},tempo[3]={0,30,600};
      char c;
    mvprintw(3,PUNTIX,"8b,dPPYba,  ,adPPYYba,  ,adPPYba, 88,dPYba,,adPYba,  ,adPPYYba, 8b,dPPYba, ");
    mvprintw(4,PUNTIX,"88P'    '8a ''     'Y8 a8'     '' 88P'   '88'    '8a ''     'Y8 88P'   ''8a");
    mvprintw(5,PUNTIX,"88       d8 ,adPPPPP88 8b         88      88      88 ,adPPPPP88 88       88");
    mvprintw(6,PUNTIX,"88b,   ,a8' 88,    ,88 '8a,   ,aa 88      88      88 88,    ,88 88       88");
    mvprintw(7,PUNTIX,"88 YbbdP''  `'8bbdP'Y8  ''Ybbd8'' 88      88      88  '8bbdP'Y8 88       88");
    mvprintw(8,PUNTIX,"88                                                                         ");
    mvprintw(9,PUNTIX,"88                                                                         ");
    refresh();
    usleep(3000000);
    mvprintw(3,PUNTIX,"                                                                           ");
    mvprintw(4,PUNTIX,"                                                                           ");
    mvprintw(5,PUNTIX,"                                                                           ");
    mvprintw(6,PUNTIX,"                                                                           ");
    mvprintw(7,PUNTIX,"                                                                           ");
    mvprintw(8,PUNTIX,"                                                                           ");
    mvprintw(9,PUNTIX,"                                                                           ");

    /*STAMPA SCRITTA BENVENUTO E PARAMETRI GIOCO*/
    mvprintw(3,1,"BENVENUTI NEL MIO PACMAN, ");
    mvprintw(4,1,"IN QUESTO MENU POTETE SCEGLIERE I PARAMETRI DEL GIOCO");
    mvprintw(5,1,"PER MUOVERVI USATE LE FRECCIE SU E GIU");
    mvprintw(6,1,"PER CAMBIARE I PARAEMTRI LE FRECCIE LATERALI");
    mvprintw(7,1,"QUANDO AVRAI DECISO PREMI SPAZIO E TI CONDURRO\' AL GIOCO");
    mvprintw(8,1,"NON SARAI SOLO, HO PIAZZATTO DUE BOMBE ROSSE, TI AIUTERANNO A SMALTIRE I FANTASMI");
    refresh();
    usleep(3000000);
    mvprintw(10,1,"MENU:");
    mvprintw(12,1,"NUMERO VITE PACMAN                  --> %d",vitePacman[0]);
    mvprintw(14,1,"NUMERO VITE FANTASMI                --> %d ",viteFantasmi[0]);
    mvprintw(16,1,"INTERVALLO S MASSIMO SPARI FANTASMI --> %d ",sparifantasmi[0]);
    mvprintw(18,1,"TOLLERANZA AGLI SPARI DEI FANTASMI  --> %d ",tolleranza[0]);
    mvprintw(20,1,"TEMPO S RESPAWN MASSIMO FANTASMI    --> %d ",tempoMAXRESPAWN[0]);
    mvprintw(22,1,"NUMERO FANTASMI                     --> %d ",numeroFant[0]);
    mvprintw(24,1,"TEMPO A DISPOSIZIONE PER VINCERE    --> %d ",tempo[0]);
    mvprintw(26,PUNTIX,"8b,dPPYba,  ,adPPYYba,  ,adPPYba, 88,dPYba,,adPYba,  ,adPPYYba, 8b,dPPYba, ");
    mvprintw(27,PUNTIX,"88P'    '8a ''     'Y8 a8'     '' 88P'   '88'    '8a ''     'Y8 88P'   ''8a");
    mvprintw(28,PUNTIX,"88       d8 ,adPPPPP88 8b         88      88      88 ,adPPPPP88 88       88");
    mvprintw(29,PUNTIX,"88b,   ,a8' 88,    ,88 '8a,   ,aa 88      88      88 88,    ,88 88       88");
    mvprintw(30,PUNTIX,"88 YbbdP''  `'8bbdP'Y8  ''Ybbd8'' 88      88      88  '8bbdP'Y8 88       88");
    mvprintw(31,PUNTIX,"88                                                                         ");
    mvprintw(32,PUNTIX,"88                                                                         ");
    attron(COLOR_PAIR(18));
    mvaddch(12,0,ACS_BULLET);
    attroff(COLOR_PAIR(18));

    refresh();
    usleep(300000);
    do{
      char c,save;
      c=getch();
      if(c==GIU){
        if(movimento_up_down<12){
          mvaddch(12+movimento_up_down,0,' ');
          movimento_up_down+=2;
          attron(COLOR_PAIR(18));
          mvaddch(12+movimento_up_down,0,ACS_BULLET);
          attroff(COLOR_PAIR(18));
          refresh();
        }
      }
      if(c==SU){
        if(movimento_up_down>0){
          mvaddch(12+movimento_up_down,0,' ');
          movimento_up_down-=2;
          attron(COLOR_PAIR(18));
          mvaddch(12+movimento_up_down,0,ACS_BULLET);
          attroff(COLOR_PAIR(18));
          refresh();
        }
      }
      if(c==DESTRA){
        switch (movimento_up_down){
          case 0:
          if(movimentoLRVP>=0&&movimentoLRVP<2){
            movimentoLRVP+=1;
            mvprintw(12,1,"                                           ");
            mvprintw(12,1,"NUMERO VITE PACMAN                  --> %d",vitePacman[movimentoLRVP]);
            refresh();
          }
          break;
          case 2:
          if(movimentoLRVF>=0&&movimentoLRVF<2){
            movimentoLRVF+=1;
            mvprintw(14,1,"                                           ");
            mvprintw(14,1,"NUMERO VITE FANTASMI                --> %d",viteFantasmi[movimentoLRVF]);
            refresh();
          }
          break;
          case 4:
          if(movimentoLRSF>=0&&movimentoLRSF<2){
            movimentoLRSF+=1;
            mvprintw(16,1,"                                           ");
            mvprintw(16,1,"INTERVALLO S MASSIMO SPARI FANTASMI --> %d",sparifantasmi[movimentoLRSF]);
            refresh();
          }
          break;
          case 6:
          if(movimentoLRT>=0&&movimentoLRT<2){
            movimentoLRT+=1;
            mvprintw(18,1,"                                           ");
            mvprintw(18,1,"TOLLERANZA AGLI SPARI DEI FANTASMI  --> %d",tolleranza[movimentoLRT]);
            refresh();
          }
          break;
          case 8:
          if(movimentoLRTR>=0&&movimentoLRTR<2){
            movimentoLRTR+=1;
            mvprintw(20,1,"                                           ");
            mvprintw(20,1,"TEMPO S RESPAWN MASSIMO FANTASMI    --> %d",tempoMAXRESPAWN[movimentoLRTR]);
            refresh();
          }
          break;
          case 10:
          if(movimentoLRNF>=0&&movimentoLRNF<2){
            movimentoLRNF+=1;
            mvprintw(22,1,"                                           ");
            mvprintw(22,1,"NUMERO FANTASMI                     --> %d ",numeroFant[movimentoLRNF]);
            refresh();
          }
          break;
          case 12:
          if(movimentoLRTM>=0&&movimentoLRTM<2){
            movimentoLRTM+=1;
            mvprintw(24,1,"                                           ");
            mvprintw(24,1,"TEMPO A DISPOSIZIONE PER VINCERE    --> %d ",tempo[movimentoLRTM]);
            refresh();
          }
          break;
        }
      }
      if(c==SINISTRA){
        switch (movimento_up_down){
          case 0:
          if(movimentoLRVP>0&&movimentoLRVP<=3){
            movimentoLRVP-=1;
            mvprintw(12,1,"                                           ");
            mvprintw(12,1,"NUMERO VITE PACMAN                  --> %d",vitePacman[movimentoLRVP]);
            refresh();
          }
          break;
          case 2:
          if(movimentoLRVF>0&&movimentoLRVF<=3){
            movimentoLRVF-=1;
            mvprintw(14,1,"                                           ");
            mvprintw(14,1,"NUMERO VITE FANTASMI                --> %d",viteFantasmi[movimentoLRVF]);
            refresh();
          }
          break;
          case 4:
          if(movimentoLRSF>0&&movimentoLRSF<=3){
            movimentoLRSF-=1;
            mvprintw(16,1,"                                           ");
            mvprintw(16,1,"INTERVALLO S MASSIMO SPARI FANTASMI --> %d",sparifantasmi[movimentoLRSF]);
            refresh();
          }
          break;
          case 6:
          if(movimentoLRT>0&&movimentoLRT<=3){
            movimentoLRT-=1;
            mvprintw(18,1,"                                           ");
            mvprintw(18,1,"TOLLERANZA AGLI SPARI DEI FANTASMI  --> %d",tolleranza[movimentoLRT]);
            refresh();
          }
          break;
          case 8:
          if(movimentoLRTR>0&&movimentoLRTR<=3){
            movimentoLRTR-=1;
            mvprintw(20,1,"                                           ");
            mvprintw(20,1,"TEMPO S RESPAWN MASSIMO FANTASMI    --> %d",tempoMAXRESPAWN[movimentoLRTR]);
            refresh();
          }
          break;
          case 10:
          if(movimentoLRNF>0&&movimentoLRNF<=3){
            movimentoLRNF-=1;
            mvprintw(22,1,"                                           ");
            mvprintw(22,1,"NUMERO FANTASMI                     --> %d ",numeroFant[movimentoLRNF]);
            refresh();
          }
          break;
          case 12:
          if(movimentoLRTM>0&&movimentoLRTM<=3){
            movimentoLRTM-=1;
            mvprintw(24,1,"                                           ");
            mvprintw(24,1,"TEMPO A DISPOSIZIONE PER VINCERE    --> %d ",tempo[movimentoLRTM]);
            refresh();
          }
          break;
        }
      }
      if(c==SPAZIO){
        for(i=0;i<33;i++){
          mvprintw(i,0,"                                                                                                                           ");
        }
        refresh();
        numero_fantasmi_game=numeroFant[movimentoLRNF];
        vite_pacman_game=vitePacman[movimentoLRVP];
        tempo_respan_fant_game=tempoMAXRESPAWN[movimentoLRTR];
        armatura=tolleranza[movimentoLRT];
        vite_fantasma_game=viteFantasmi[movimentoLRVF];
        tempo_spari_fantasma=sparifantasmi[movimentoLRSF];
        tempo_massimo=tempo[movimentoLRTM];
      return;
      }
    }while(1);


    }


    void areaGioco() {
        int i,c,d,f,punti_gen=0,colore,tempo_passato=0;
        _Bool generazione=false,continua=true,perso=false,vinto=false,timeout=false;
        time_t tempo_salvato=time((time_t*)NULL),tempo_gioco;

        if(tempo_massimo>0){
          timeout=true;
        }

         for(i=0;i<numero_fantasmi_game;i++){
          posso_sparare_fantasmi[i]=true;
          tempo_spari[i]=numero_casuale(2,tempo_spari_fantasma);
        }
        for(i=0;i<numero_fantasmi_game;i++){
          for(c=0;c<NUMEROSPARI;c++){
            pos_spari_fantasmi[i][c].x=101;
            pos_spari_fantasmi[i][c].y=101;
          }
        }



        pthread_mutex_lock(&mutex);

        /* STAMPA MAPPA*/
        for (i = 0; i < MAXY; i++) {
            for (c = 0; c < MAXX; c++) {
                switch (mappa[i][c]) {
                    case '#':
                        attron(COLOR_PAIR(1));
                        mvaddch(i, c, ACS_BLOCK);
                        attroff(COLOR_PAIR(1));
                        break;
                    case '|':
                        mvaddch(i, c, ACS_VLINE);
                        break;
                    case '-':
                        mvaddch(i, c, ACS_HLINE);
                        break;
                    case '!':
                        mvaddch(i, c, ACS_ULCORNER);
                        break;
                    case '?':
                        mvaddch(i, c, ACS_URCORNER);
                        break;
                    case '$':
                        mvaddch(i, c, ACS_LLCORNER);
                        break;
                    case '&':
                        mvaddch(i, c, ACS_LRCORNER);
                        break;
                }
            }
        }


        /*STAMPA PUNTI OBBIETTIVO*/
        for (i = 0; i < MAXY; i++) {
          for (c = 0; c < MAXX; c++) {
            switch (mappa_punti[i][c]) {
                case '.':
              if(i==1&&c==1){
                attron(COLOR_PAIR(18));
                mvaddch(i, c, ACS_BULLET);
                attroff(COLOR_PAIR(18));
              }else if(i==22&&c==25){
                attron(COLOR_PAIR(18));
                mvaddch(i, c, ACS_BULLET);
                attroff(COLOR_PAIR(18));
              }else{
                mvaddch(i, c, ACS_BULLET);
              }
              break;
              }
          }
        }
        if(timeout){
            attron(COLOR_PAIR(18));
            mvprintw(1,27,"TIME");
            attroff(COLOR_PAIR(18));
        }
       refresh();
       pthread_mutex_unlock(&mutex);/*Fine sezione critica*/

       /*CREAZIONE THREAD PACMAN*/
       pthread_create(&pacmanID,NULL,&pacman,(void*)&par_pacman);

        tempo_gioco=time((time_t*)NULL);
        while(continua){
          /*CONTA PUNTI GIOCO CON STAMPA E GENERAZIONE FANTASMI INIZIALE*/
          if(mappa_punti[pos_pacman.y][pos_pacman.x]=='.'){
            if(pos_pacman.y==1&&pos_pacman.x==1){
              if(counter_fantasmi>0){
                for(i=0;i<counter_fantasmi;i++){
                  fant_fantasma[i].num_vite=0;
                }
              }
            }
            if(pos_pacman.y==22&&pos_pacman.x==25){
              if(counter_fantasmi>0){
                for(i=0;i<counter_fantasmi;i++){
                  fant_fantasma[i].num_vite=0;
                }
              }
            }
            mappa_punti[pos_pacman.y][pos_pacman.x]=' ';
            punti_game+=10;
            punti_gen+=10;
            if(punti_gen>60){
              if((int)numero_casuale(1,7)==3){
                generazione=true;
                }
              }
          }
          if(generazione){
            if(counter_fantasmi<numero_fantasmi_game){
                fant_fantasma[counter_fantasmi].mutex=&mutex;
                fant_fantasma[counter_fantasmi].fine_generazione=false;
                fant_fantasma[counter_fantasmi].posizione=&pos_fantasma[counter_fantasmi];
                fant_fantasma[counter_fantasmi].id_fantasma=counter_fantasmi;
                fant_fantasma[counter_fantasmi].morto=false;
                pos_fantasma[counter_fantasmi].x=pos_pacman.x;
                pos_fantasma[counter_fantasmi].y=pos_pacman.y;
                pthread_create(&fantasmaID[counter_fantasmi],NULL,&fantasma,(void*)&fant_fantasma[counter_fantasmi]);
                counter_fantasmi+=1;
                generazione=false;
                punti_gen=0;
              }else{
                generazione=false;
              }
          }else{
            generazione=false;
          }
          pthread_mutex_lock(&mutex);/*Inizio sezione critica*/
          attron(COLOR_PAIR(15));
          mvprintw(PUNTIY,PUNTIX,"GAME POINTS --> %d",punti_game);
          attroff(COLOR_PAIR(15));
          pthread_mutex_unlock(&mutex);/*Fine sezione critica*/

          /*CONTA TEMPO*/
          tempo_passato=-(tempo_gioco-time((time_t*)NULL));

          /*STAMPA TEMPO LIMITE*/

          if(timeout){
            pthread_mutex_lock(&mutex);/*Inizio sezione critica*/
            mvprintw(2,27,"%d",tempo_passato);
            pthread_mutex_unlock(&mutex);/*Fine sezione critica*/

          }
          /*STAMPA VITE PACMAN,ARMATURA E AGGIORNA VITE*/
          pthread_mutex_lock(&mutex);/*Inizio sezione critica*/
          mvprintw(PUNTIY+1,PUNTIX,"                                       ");
          mvprintw(PUNTIY+2,PUNTIX,"                                       ");
          attron(COLOR_PAIR(2));
          mvprintw(PUNTIY+1,PUNTIX,"VITE_PACMAN --> %d",par_pacman.num_vite);
          mvprintw(PUNTIY+2,PUNTIX,"ARMATURA_PACMAN --> %d",armatura);
          attroff(COLOR_PAIR(2));
          pthread_mutex_unlock(&mutex);/*Fine sezione critica*/
          /*STAMPA VITE FANTASMI*/
          if(counter_fantasmi>0){
            for(i=0,colore=3;i<counter_fantasmi;i++,colore++){
              pthread_mutex_lock(&mutex);/*Inizio sezione critica*/
              attron(COLOR_PAIR(colore));
              mvprintw(YVITEFANTASMA+i,PUNTIX,"VITE_FANTASMA --> %d",fant_fantasma[i].num_vite);
              attroff(COLOR_PAIR(colore));
              pthread_mutex_unlock(&mutex);/*Fine sezione critica*/
            }
          }

          /*RESPAWNFANTASMI MORTI*/
          if(counter_fantasmi>0){
            for(i=0;i<counter_fantasmi;i++){
               if(fant_fantasma[i].morto==true){
                 pos_fantasma[i].x=13;
                 pos_fantasma[i].y=11;
                 fant_fantasma[i].fine_generazione=false;
                 pthread_create(&fantasmaID[i],NULL,&fantasma,(void*)&fant_fantasma[i]);
               }
             }
          }

          /*STUDIO COLLISSIONI CON FANTASMI*/
          if(counter_fantasmi>0){
            for(i=0;i<counter_fantasmi;i++){
              if(fant_fantasma[i].fine_generazione==true){
                if(pos_pacman.x==pos_fantasma[i].x&&pos_pacman.y==pos_fantasma[i].y){
                  if(!par_pacman.invincibilita){
                    par_pacman.num_vite-=1;
                    pos_pacman.x=13;
                    pos_pacman.y=18;
                    pos_pacman.direzione=0;
                    par_pacman.invincibilita=true;
                  }
                }
              }
            }
          }
          /*SPARI PACMAN*/
          if(spari_pacman==true&&posso_sparare==true){
                posso_sparare=false;
                spari_pacman=false;
                int i,c;
                for(i=0,c=1;i<NUMEROSPARI;i++,c++){
                par_spari[i].mutex=&mutex;
                par_spari[i].posizione=&pos_spari[i];
                par_spari[i].posizione->direzione=c;
                par_spari[i].contatto=false;
                par_spari[i].posizione->x=pos_pacman.x;
                par_spari[i].posizione->y=pos_pacman.y;
                finespari[i]=false;
                }
                for(i=0;i<NUMEROSPARI;i++){
                pthread_create(&spariID[i],NULL,&sparo,(void*)&par_spari[i]);
                }
              }
              if(finespari[0]==true){
                if(finespari[1]==true){
                  if(finespari[2]==true){
                    if(finespari[3]==true){
                      posso_sparare=true;
                    }
                  }
                }
              }
          /*COLLISIONI SPARI PACMAN CON FANTASMI*/
          for(i=0;i<NUMEROSPARI;i++){
            if(par_spari[i].contatto==false){
              for(c=0;c<=counter_fantasmi;c++){
                if(controllo_collisione_sp_ogg(pos_spari[i].x,pos_spari[i].y,pos_spari[i].direzione,pos_fantasma[c].x,pos_fantasma[c].y)){
                  par_spari[i].contatto=true;
                  if(fant_fantasma[c].fine_generazione==true&&fant_fantasma[c].invincibilita==false){
                    fant_fantasma[c].num_vite--;
                    fant_fantasma[c].invincibilita=true;
                  }
                }
              }
            }
          }
          /*RIMBALZI FANTASMI*/
          if(counter_fantasmi>1){
            for(i=0;i<=counter_fantasmi;i++){
              for(c=0;c<=counter_fantasmi;c++){
                if(i!=c){
                  if(controllo_rimbalzi(pos_fantasma[i].y,pos_fantasma[i].x,pos_fantasma[i].direzione,pos_fantasma[c].y,pos_fantasma[c].x)){
                    fant_fantasma[i].rimbalzo=true;
                  }
                }
              }
            }
          }

         /*SPARI FANTASMI*/
         for(i=0;i<counter_fantasmi;i++){
          if(finespari_fantasma[i][0]==true){
                  if(finespari_fantasma[i][1]==true){
                    if(finespari_fantasma[i][2]==true){
                      if(finespari_fantasma[i][3]==true){
                        posso_sparare_fantasmi[i]=true;
                      }
                    }
                  }
          }
        }

        for(d=0;d<counter_fantasmi;d++){
          if((time(NULL)%tempo_spari[d])==0){
          if(posso_sparare_fantasmi[d]&&fant_fantasma[d].fine_generazione==true){
            posso_sparare_fantasmi[d]=false;
            tempo_spari[d]=numero_casuale(2,tempo_spari_fantasma);
            for(i=0,c=1;i<NUMEROSPARI;i++,c++){
            par_spari_fantasmi[d][i].mutex=&mutex;
            par_spari_fantasmi[d][i].posizione=&pos_spari_fantasmi[d][i];
            par_spari_fantasmi[d][i].posizione->direzione=c;
            par_spari_fantasmi[d][i].contatto=false;
            par_spari_fantasmi[d][i].posizione->x=pos_fantasma[d].x;
            par_spari_fantasmi[d][i].posizione->y=pos_fantasma[d].y;
            par_spari_fantasmi[d][i].proprietario=d;
            par_spari_fantasmi[d][i].colore=17;
            finespari_fantasma[d][i]=false;
            }
            for(i=0;i<NUMEROSPARI;i++){
            pthread_create(&spari_fantasmaID[d][i],NULL,&sparo_fantasma,(void*)&par_spari_fantasmi[d][i]);
            }
          }
        }
        }

        /*CONTROLLO COLLISIONI SPARI FANTASMI CON PACMAN*/
        if(counter_fantasmi>0){
          for(i=0;i<counter_fantasmi;i++){
            for(c=0;c<NUMEROSPARI;c++){
              if(par_spari_fantasmi[i][c].contatto==false){
                if(par_pacman.invincibilita==false){
                  if(controllo_collisione_sp_ogg(pos_spari_fantasmi[i][c].x,pos_spari_fantasmi[i][c].y,pos_spari_fantasmi[i][c].direzione,pos_pacman.x,pos_pacman.y)){
                    armatura-=1;
                    par_spari_fantasmi[i][c].contatto=true;
                  }
                }
              }
            }
          }
        }
        if(counter_fantasmi>0){
          for(i=0;i<counter_fantasmi;i++){
            for(c=0;c<NUMEROSPARI;c++){
              if(par_spari_fantasmi[i][c].contatto==false){
                if(par_pacman.invincibilita==true){
                  if(controllo_collisione_sp_ogg(pos_spari_fantasmi[i][c].x,pos_spari_fantasmi[i][c].y,pos_spari_fantasmi[i][c].direzione,pos_pacman.x,pos_pacman.y)){
                    par_spari_fantasmi[i][c].contatto=true;
                  }
                }
              }
            }
          }
        }
        if(armatura<=0){
          armatura=10;
          par_pacman.num_vite-=1;
          pos_pacman.x=13;
          pos_pacman.y=18;
          pos_pacman.direzione=0;
          par_pacman.invincibilita=true;
        }

        /*CONTROLLO COLLISIONI SPARI FANTASMA SPARI PACMAN*/
        if(counter_fantasmi>0){
          for(i=0;i<counter_fantasmi;i++){
            for(c=0;c<NUMEROSPARI;c++){
              for(d=0;d<NUMEROSPARI;d++){
                if(par_spari_fantasmi[i][c].contatto==false){
                  if(par_spari[d].contatto==false){
                    if(pos_spari_fantasmi[i][c].x==pos_spari[d].x&&pos_spari_fantasmi[i][c].y==pos_spari[d].y){
                      par_spari_fantasmi[i][c].contatto=true;
                      par_spari[d].contatto=true;
                    }
                  }
                }
              }
            }
          }
        }
        /*CONTROLLO COLLISIONI SPARI FANTASMI CON ALTRI FANTASMI*/
        if(counter_fantasmi>0){
          for(i=0;i<counter_fantasmi;i++){
            for(c=0;c<counter_fantasmi;c++){
              if(i!=c){
                for(d=0;d<NUMEROSPARI;d++){
                  if(controllo_collisione_sp_ogg(pos_spari_fantasmi[i][d].x,pos_spari_fantasmi[i][d].y,pos_spari_fantasmi[i][d].direzione,pos_fantasma[c].x,pos_fantasma[c].y)){
                    par_spari_fantasmi[i][d].contatto=true;
                  }
                }
              }
            }
          }
        }

        /*VITTORIA O SCONFITTA*/
        if(par_pacman.num_vite==0){
        continua=false;
        perso=true;
        }
        if(punti_game==2590){
          continua=false;
          vinto=true;
        }
        if(timeout){
          if(tempo_massimo==tempo_passato){
            continua=false;
            timeout=false;
          }
        }
        usleep(10000);
        }
        par_pacman.num_vite=0;
        for(i=0;i<NUMEROSPARI;i++){
          par_spari[i].contatto=true;
        }
        for(i=0;i<counter_fantasmi;i++){
          fant_fantasma[i].num_vite=0;
        }
        for(i=0;i<counter_fantasmi;i++){
          for(c=0;c<NUMEROSPARI;c++){
            par_spari_fantasmi[i][c].contatto=true;
          }
        }
        usleep(200000);
        for(i=0;i<33;i++){
          mvprintw(i,0,"                                                                                            ");
        }
        refresh();
        if(perso){
          mvprintw(MAXY/2-1,(MAXX/2)-5,"GAME OVER!");//Stampo la scritta "GAME OVER!" al centro dello schermo
          mvprintw(MAXY/2,(MAXX/2)-11,"HAI TERMINATO LE VITE");
          while (getch()!='\n');//Aspetto che l'utente prema invio
          endwin();//Ripristino il normale funzionamento del terminale
          return;
        }
        if(vinto){
          mvprintw(MAXY/2-1,(MAXX/2)-5,"COMPLIMENTI!");//Stampo la scritta "GAME OVER!" al centro dello schermo
          mvprintw(MAXY/2,(MAXX/2)-11,"  HAI VINTO MANGIANDO");
          while (getch()!='\n');//Aspetto che l'utente prema invio
          endwin();//Ripristino il normale funzionamento del terminale
          return;
        }
        if(!timeout){
          mvprintw(MAXY/2-1,(MAXX/2)-5,"HAI PERSO!");//Stampo la scritta "GAME OVER!" al centro dello schermo
          mvprintw(MAXY/2,(MAXX/2)-11,"   HAI FINITO IL TEMPO");
          while (getch()!='\n');//Aspetto che l'utente prema invio
          endwin();//Ripristino il normale funzionamento del terminale
          return;
        }

    }
void * pacman(void * param)
{
  par * parametri = (par*) param;/*Converto la mia variabile in input*/
  pos * pos_pacman = parametri->posizione;
  pthread_mutex_t * mutex = parametri->mutex;

  int controllo;

  pos_pacman->x=13;
  pos_pacman->y=18;

  pthread_mutex_lock(mutex);/*Inizio sezione critica*/
  attron(COLOR_PAIR(parametri->colore));
  mvaddch(pos_pacman->y,pos_pacman->x,'<');
  attroff(COLOR_PAIR(parametri->colore));
  pthread_mutex_unlock(mutex);/*Fine sezione critica*/


  while(parametri->num_vite>0)
  {

    char c,save;

    c=getch();
    if(c!=ERR){
       if(c==32){
         if(posso_sparare==true){
            spari_pacman=true;
         }
       }else{
           usleep(10000);
           if(posso(pos_pacman->x,pos_pacman->y,c)){
             pos_pacman->direzione=c;
             parametri->invincibilita=false;
           }else{
             save=c;
           }
       }
     }else{
       if(posso(pos_pacman->x,pos_pacman->y,save)){
         pos_pacman->direzione=save;
         save=99;
       }
     }
    switch(pos_pacman->direzione)// Mi sposto in base al tasto che ho premuto
    {				    // controllando ogni volta di non uscire dai limiti
      case SU:
      controllo=pos_pacman->y-1;
      if(mappa[controllo][pos_pacman->x]==' '||mappa[controllo][pos_pacman->x]=='.'){
        pos_pacman->y-=1;
      }
      pthread_mutex_lock(mutex);/*Inizio sezione critica*/
      attron(COLOR_PAIR(parametri->colore));
      mvaddch(pos_pacman->y,pos_pacman->x,'v');
      attroff(COLOR_PAIR(parametri->colore));
      pthread_mutex_unlock(mutex);/*Fine sezione critica*/
      break;

      case GIU:
      controllo=pos_pacman->y+1;
      if(mappa[controllo][pos_pacman->x]==' '||mappa[controllo][pos_pacman->x]=='.'){
        pos_pacman->y+=1;
      }
      pthread_mutex_lock(mutex);/*Inizio sezione critica*/
      attron(COLOR_PAIR(parametri->colore));
      mvaddch(pos_pacman->y,pos_pacman->x,'^');
      attroff(COLOR_PAIR(parametri->colore));
      pthread_mutex_unlock(mutex);/*Fine sezione critica*/
      break;

      case SINISTRA:
      controllo=pos_pacman->x-1;
      if(pos_pacman->y==11&&pos_pacman->x==0){
        pos_pacman->x=26;
      }else if(mappa[pos_pacman->y][controllo]==' '||mappa[pos_pacman->y][controllo]=='.'){
        pos_pacman->x-=1;
      }
      pthread_mutex_lock(mutex);/*Inizio sezione critica*/
      attron(COLOR_PAIR(parametri->colore));
      mvaddch(pos_pacman->y,pos_pacman->x,'>');
      attroff(COLOR_PAIR(parametri->colore));
      pthread_mutex_unlock(mutex);/*Fine sezione critica*/
      break;

      case DESTRA:
      controllo=pos_pacman->x+1;
      if(pos_pacman->y==11&&pos_pacman->x==26){
        pos_pacman->x=0;
      }else if(mappa[pos_pacman->y][controllo]==' '||mappa[pos_pacman->y][controllo]=='.'){
        pos_pacman->x+=1;
      }
      pthread_mutex_lock(mutex);/*Inizio sezione critica*/
      attron(COLOR_PAIR(parametri->colore));
      mvaddch(pos_pacman->y,pos_pacman->x,'<');
      attroff(COLOR_PAIR(parametri->colore));
      pthread_mutex_unlock(mutex);/*Fine sezione critica*/
      break;
      case 0:
      pthread_mutex_lock(mutex);
      attron(COLOR_PAIR(16));
      mvaddch(pos_pacman->y,pos_pacman->x,'<');
      attroff(COLOR_PAIR(16));
      pthread_mutex_unlock(mutex);
      break;
    }
    /*TEMPORIZZAZIONE*/
    pthread_mutex_lock(mutex);/*Inizio sezione critica*/
    refresh();
    pthread_mutex_unlock(mutex);/*Fine sezione critica*/
    usleep(140000);
    pthread_mutex_lock(mutex);/*Inizio sezione critica*/
    mvaddch(pos_pacman->y,pos_pacman->x,' ');
    pthread_mutex_unlock(mutex);/*Fine sezione critica*/

  }
}

/*THREAD SPARI*/
 void * sparo(void * param){

   spar * parametri = (spar*) param;/*Converto la mia variabile in input*/
   pos * pos_sparo = parametri->posizione;
   pthread_mutex_t * mutex = parametri->mutex;
   int controllo;

   while(controllo_spari(pos_sparo->x,pos_sparo->y,parametri->posizione->direzione)&&parametri->contatto==false){
     switch(parametri->posizione->direzione){
       case 1:
           pos_sparo->y-=1;
           pthread_mutex_lock(mutex);/*Inizio sezione critica*/
           mvaddch(pos_sparo->y,pos_sparo->x,'|');
           pthread_mutex_unlock(mutex);/*Fine sezione critica*/
         break;
       case 2:
           pos_sparo->y+=1;
           pthread_mutex_lock(mutex);/*Inizio sezione critica*/
           mvaddch(pos_sparo->y,pos_sparo->x,'|');
           pthread_mutex_unlock(mutex);/*Fine sezione critica*/
           break;
       case 3:
           pos_sparo->x-=1;
           pthread_mutex_lock(mutex);/*Inizio sezione critica*/
           mvaddch(pos_sparo->y,pos_sparo->x,'-');
           pthread_mutex_unlock(mutex);/*Fine sezione critica*/
           break;
       case 4:
           pos_sparo->x+=1;
           pthread_mutex_lock(mutex);/*Inizio sezione critica*/
           mvaddch(pos_sparo->y,pos_sparo->x,'-');
           pthread_mutex_unlock(mutex);/*Fine sezione critica*/
           break;
         }
          usleep(90000);
          pthread_mutex_lock(mutex);/*Inizio sezione critica*/
          mvaddch(pos_sparo->y,pos_sparo->x,' ');
          pthread_mutex_unlock(mutex);/*Fine sezione critica*/

  }
  controllo=parametri->posizione->direzione;
  controllo-=1;
  finespari[controllo]=true;
  pos_sparo->y=99;
  pos_sparo->x=100;
  parametri->contatto=true;
}

 /*THREAD FANTASMI*/
void * fantasma(void * param)
{
  fant * parametri = (fant*) param;/*Converto la mia variabile in input*/
  pos * pos = parametri->posizione;
  pthread_mutex_t * mutex = parametri->mutex;
  int tempo_spawn=numero_casuale(2,4),tempo_salvato = (int)time(NULL),primo_passo=0;
  int tempo_spawn_rinascita=numero_casuale(2,tempo_respan_fant_game);
  _Bool respawn=false;

  if(parametri->morto==true){
    respawn=true;
    parametri->morto=false;
  }else{
    do{
      pthread_mutex_lock(mutex);
      attron(COLOR_PAIR(parametri->colore));
      mvaddch(pos->y,pos->x,'@');
      attroff(COLOR_PAIR(parametri->colore));
      pthread_mutex_unlock(mutex);/*Fine sezione critica*/
    }while((time(NULL)-tempo_salvato)<tempo_spawn);
  }

  parametri->num_vite=vite_fantasma_game;
  parametri->rimbalzo=false;

  if(respawn==true){
    do{
      pthread_mutex_lock(mutex);
      attron(COLOR_PAIR(16));
      mvaddch(pos->y,pos->x,'@');
      attroff(COLOR_PAIR(16));
      pthread_mutex_unlock(mutex);/*Fine sezione critica*/
    }while((time(NULL)-tempo_salvato)<tempo_spawn_rinascita);

    pos->direzione=numero_casuale(1,4);

    contatore_fantasmi_morti-=1;

    if(contatore_fantasmi_morti==0){
      pthread_mutex_lock(mutex);
      mvaddch(pos->y,pos->x,' ');
      pthread_mutex_unlock(mutex);/*Fine sezione critica*/
    }
    do{
      switch (pos->direzione){
              case 1:
                  pos->y-=1;
                  break;

              case 2:
                  pos->y+=1;
                  break;
              case 3:
                  if(pos->y==11&&pos->x==0){
                      pos->x=27;
                  }else{
                      pos->x-=1;
                      }
                  break;
              case 4:
                  if(pos->y==11&&pos->x==27){
                      pos->x=0;
                  }else{
                      pos->x+=1;
                   }
                   break;
        }
        pthread_mutex_lock(mutex);/*Inizio sezione critica*/
        attron(COLOR_PAIR(16));
        mvaddch(pos->y,pos->x,'@');
        attroff(COLOR_PAIR(16));
        pthread_mutex_unlock(mutex);/*Fine sezione critica*/

        usleep(170000);

        pthread_mutex_lock(mutex);/*Inizio sezione critica*/
        mvaddch(pos->y,pos->x,' ');
        pthread_mutex_unlock(mutex);/*Fine sezione critica*/

        if(mappa[pos->y][pos->x]=='|'){
          pthread_mutex_lock(mutex);/*Inizio sezione critica*/
          mvaddch(pos->y,pos->x,ACS_VLINE);
          pthread_mutex_unlock(mutex);/*Fine sezione critica*/
        }
        if(mappa[pos->y][pos->x]=='-'){
          pthread_mutex_lock(mutex);/*Inizio sezione critica*/
          mvaddch(pos->y,pos->x,ACS_HLINE);
          pthread_mutex_unlock(mutex);/*Fine sezione critica*/
        }
    }while(controllo_respawn(pos->y,pos->x,pos->direzione));
  }



  parametri->fine_generazione=true;
  parametri->invincibilita=false;

  pthread_mutex_lock(mutex);/*Inizio sezione critica*/
  mvaddch(pos->y,pos->x,' ');
  pthread_mutex_unlock(mutex);/*Fine sezione critica*/

  if(mappa_punti[pos->y][pos->x]=='.'){
    pthread_mutex_lock(mutex);/*Inizio sezione critica*/
    mvaddch(pos->y,pos->x, ACS_BULLET);
    pthread_mutex_unlock(mutex);/*Fine sezione critica*/
  }

  while(parametri->num_vite>0)
  {
    if(parametri->rimbalzo){
      pos->direzione=rimbalzi(pos->y,pos->x,pos->direzione);
      parametri->rimbalzo=false;
    }
    if(mappa[pos->y][pos->x]=='.'){
      pos->direzione=movimentiGhost(pos->y,pos->x,pos->direzione);
    }
    if(primo_passo==0){
      pos->direzione=movimentiGhost(pos->y,pos->x,0);
      primo_passo+=1;
    }

    if(parametri->invincibilita==true){
      parametri->invincibilita=false;
    }
    switch (pos->direzione){
            case 1:
                pos->y-=1;
                break;

            case 2:
                pos->y+=1;
                break;
            case 3:
                if(pos->y==11&&pos->x==0){
                    pos->x=26;
                }else{
                    pos->x-=1;
                    }
                break;
            case 4:
                if(pos->y==11&&pos->x==26){
                    pos->x=0;
                }else{
                    pos->x+=1;
                 }
                 break;
      }

    pthread_mutex_lock(mutex);/*Inizio sezione critica*/
    attron(COLOR_PAIR(parametri->colore));
    mvaddch(pos->y,pos->x,'@');
    attroff(COLOR_PAIR(parametri->colore));
    pthread_mutex_unlock(mutex);/*Fine sezione critica*/
    usleep(170000);

    pthread_mutex_lock(mutex);/*Inizio sezione critica*/
    mvaddch(pos->y,pos->x,' ');
    pthread_mutex_unlock(mutex);/*Fine sezione critica*/

    if(mappa_punti[pos->y][pos->x]=='.'){
      if(pos->x==1&&pos->y==1){
      pthread_mutex_lock(mutex);/*Inizio sezione critica*/
      attron(COLOR_PAIR(18));
      mvaddch(pos->y,pos->x, ACS_BULLET);
      attroff(COLOR_PAIR(18));
      pthread_mutex_unlock(mutex);/*Fine sezione critica*/
      }else if(pos->x==25&&pos->y==22){
      pthread_mutex_lock(mutex);/*Inizio sezione critica*/
      attron(COLOR_PAIR(18));
      mvaddch(pos->y,pos->x, ACS_BULLET);
      attroff(COLOR_PAIR(18));
      pthread_mutex_unlock(mutex);/*Fine sezione critica*/
      }else{
      pthread_mutex_lock(mutex);/*Inizio sezione critica*/
      mvaddch(pos->y,pos->x, ACS_BULLET);
      pthread_mutex_unlock(mutex);/*Fine sezione critica*/
     }
    }
  }
  contatore_fantasmi_morti+=1;
  parametri->morto=true;
  pos->x=99;
  pos->x=99;
}

/*THREAD SPARI FANTASMI*/
void * sparo_fantasma(void * param){

  spar * parametri = (spar*) param;/*Converto la mia variabile in input*/
  pos * pos_sparo = parametri->posizione;
  pthread_mutex_t * mutex = parametri->mutex;
  int controllo;


  while(controllo_spari(pos_sparo->x,pos_sparo->y,parametri->posizione->direzione)&&parametri->contatto==false){
    switch(parametri->posizione->direzione){
      case 1:
          pos_sparo->y-=1;
          pthread_mutex_lock(mutex);/*Inizio sezione critica*/
          attron(COLOR_PAIR(parametri->colore));
          mvaddch(pos_sparo->y,pos_sparo->x,'|');
          attroff(COLOR_PAIR(parametri->colore));
          pthread_mutex_unlock(mutex);/*Fine sezione critica*/
        break;
      case 2:
          pos_sparo->y+=1;
          pthread_mutex_lock(mutex);/*Inizio sezione critica*/
          attron(COLOR_PAIR(parametri->colore));
          mvaddch(pos_sparo->y,pos_sparo->x,'|');
          attroff(COLOR_PAIR(parametri->colore));
          pthread_mutex_unlock(mutex);/*Fine sezione critica*/
          break;
      case 3:
          pos_sparo->x-=1;
          pthread_mutex_lock(mutex);/*Inizio sezione critica*/
          attron(COLOR_PAIR(parametri->colore));
          mvaddch(pos_sparo->y,pos_sparo->x,'-');
          attroff(COLOR_PAIR(parametri->colore));
          pthread_mutex_unlock(mutex);/*Fine sezione critica*/
          break;
      case 4:
          pos_sparo->x+=1;
          pthread_mutex_lock(mutex);/*Inizio sezione critica*/
          attron(COLOR_PAIR(parametri->colore));
          mvaddch(pos_sparo->y,pos_sparo->x,'-');
          attroff(COLOR_PAIR(parametri->colore));
          pthread_mutex_unlock(mutex);/*Fine sezione critica*/
          break;
        }

         usleep(90000);
         pthread_mutex_lock(mutex);/*Inizio sezione critica*/
         mvaddch(pos_sparo->y,pos_sparo->x,' ');
         pthread_mutex_unlock(mutex);/*Fine sezione critica*/

 }
 controllo=parametri->posizione->direzione;
 controllo-=1;
 finespari_fantasma[parametri->proprietario][controllo]=true;
 pos_sparo->y=99;
 pos_sparo->x=100;
 parametri->contatto=true;
}

/*CONTROLLI THREAD*/
_Bool posso(int x,int y,int moves){
  int controllo;
  switch(moves){
    case SU:
    controllo=y-1;
    if(mappa[controllo][x]==' '||mappa[controllo][x]=='.'){
      return true;
    }else{return false;}
    case GIU:
    controllo=y+1;
    if(mappa[controllo][x]==' '||mappa[controllo][x]=='.'){
      return true;
    }else{ return false;}

    case SINISTRA:
    controllo=x-1;
    if(y==11&&x==0){
      return true;
    }else if(mappa[y][controllo]==' '||mappa[y][controllo]=='.'){
      return true;
    }else{ return false;}
    break;

    case DESTRA:
    controllo=x+1;
    if(y==11&&x==27){
      return true;
    }else if(mappa[y][controllo]==' '||mappa[y][controllo]=='.'){
      return true;
    }else{return false;}
    break;
    default:
    return false;
    break;
  }
}

_Bool controllo_respawn(int y,int x,int direzione){
  int cn_su,cn_giu,cn_dx,cn_sx;
  cn_su=y-1;
  cn_giu=y+1;
  cn_sx=x+1;
  cn_dx=x-1;

  if(direzione==1){
    if(mappa[cn_su][x]!='#'){
      return true;
    }
  }
  if(direzione==2){
    if(mappa[cn_giu][x]!='#'){
      return true;
    }
  }
  if(direzione==3){
    if(mappa[y][cn_sx]!='|'){
      return true;
    }
  }
  if(direzione==4){
    if(mappa[y][cn_dx]!='|'){
      return true;
    }
  }
  return false;
}
int movimentiGhost(int y,int x,int direzione_save){
  int cn_su,cn_giu,cn_dx,cn_sx,direzione[4],counter=0,scelta,opposto_save=0,casuale;

  /*USO VARIABILI AUSILIARI PER IL CONTRLLO DELLA POSIZIONE*/
  cn_su=y-1;
  cn_giu=y+1;
  cn_sx=x-1;
  cn_dx=x+1;

  if(direzione_save!=0){
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
  }else{
    opposto_save=0;
  }
  if(mappa[cn_su][x]==' '||mappa[cn_su][x]=='.'){
    if(opposto_save!=1){
      direzione[counter]=1;
      counter++;}
    }
    if(mappa[cn_giu][x]==' '||mappa[cn_giu][x]=='.'){
      if(opposto_save!=2){
        direzione[counter]=2;
        counter++;}
      }
      if(mappa[y][cn_sx]==' '||mappa[y][cn_sx]=='.'){
        if(opposto_save!=3){
          direzione[counter]=3;
          counter++;}
        }
        if(mappa[y][cn_dx]==' '||mappa[y][cn_dx]=='.'){
          if(opposto_save!=4){
            direzione[counter]=4;
            counter++;}
          }
          scelta=direzione[rand()%counter];
          return scelta;

        }
_Bool controllo_spari(int x,int y, int dir){
  int controllo;
  switch(dir){
    case 1:
    controllo=y-1;
    if(mappa[controllo][x]==' '||mappa[controllo][x]=='.'){
      if(mappa_punti[controllo][x]==' '){
        return true;
      }else{
        return false;}
      }else{
        return false;}
      break;
    case 2:
        controllo=y+1;
        if(mappa[controllo][x]==' '||mappa[controllo][x]=='.'){
          if(mappa_punti[controllo][x]==' '){
            return true;
          }else{
            return false;}
          }else{
            return false;}
            break;
    case 3:
         controllo=x-1;
         if(mappa[y][controllo]==' '||mappa[y][controllo]=='.'){
           if(mappa_punti[y][controllo]==' '){
             return true;
            }else{
              return false;}
          }else{
            return false;}
          break;
    case 4:
         controllo=x+1;
         if(mappa[y][controllo]==' '||mappa[y][controllo]=='.'){
           if(mappa_punti[y][controllo]==' '){
              return true;
            }else{
              return false;}
          }else{
            return false;}
           break;
      }
}
