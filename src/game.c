#include <SDL.h>
#include <SDL_image.h>
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define DELAY 10

#define DIMENSIONS 3
#define NBPLAYER 2
#define PIECESIZE 3

#define BACKGROUND "background.png"
#define P1SMALL "small_p1.png"
#define P1MEDIUM "medium_p1.png"
#define P1LARGE "large_p1.png"
#define P2SMALL "small_p2.png"
#define P2MEDIUM "medium_p2.png"
#define P2LARGE "large_p2.png"
#define IMGP1 "joueur_1.png"
#define IMGP2 "joueur_2.png"


typedef struct Env_t{
  SDL_Texture * background;
  SDL_Texture * p1small1;
  SDL_Texture * p1small2;
  SDL_Texture * p1medium1;
  SDL_Texture * p1medium2;
  SDL_Texture * p1large1;
  SDL_Texture * p1large2;
  SDL_Texture * p2small1;
  SDL_Texture * p2small2;
  SDL_Texture * p2medium1;
  SDL_Texture * p2medium2;
  SDL_Texture * p2large1;
  SDL_Texture * p2large2;
  SDL_Texture * imgP1;
  SDL_Texture * imgP2;
  int piece_x[NBPLAYER][PIECESIZE][2];
  int piece_y[NBPLAYER][PIECESIZE][2];
  /*
  Pour identifier les coordonnées de chaque pièce, elles sont triés selon leurs tailles et le joueur qui la possède.
  Etant données que chaque possède deux joueurs possède deux pièce de chaque type, il est nécéssaire d'ajouter une troisième dimensions au tableau
  */
  int imgPlayer[1];
  /*
  Cette variable stocke les corrdonnées de l'image indiquant le joueur qui doit jouer
  */
}Env;

//+--------------------------------------------------------------------------------------------------------------------------------------+

Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]){
  Env * env = malloc(sizeof(struct Env_t));//Création d'un environnement alloué dynamiquement
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  h-=100;
  for(int i=0;i<NBPLAYER;i++){
    for(int j=0;j<PIECESIZE*2;j++){
      env->piece_x[i][j/2][j%2]=750+85*j;
      env->piece_y[i][j/2][j%2]=250+i*120;
      /*
      Saisie les coordonnées de chaque pièce
      */
    }
  }
  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if(!env->background)printf("Erreur lors du chargement d'une texture: %s\n", BACKGROUND);
  env->p1small1 = IMG_LoadTexture(ren, P1SMALL);
  if(!env->p1small1)printf("Erreur lors du chargement d'une texture: %s\n", P1SMALL);
  env->p1small2 = IMG_LoadTexture(ren, P1SMALL);
  if(!env->p1small2)printf("Erreur lors du chargement d'une texture: %s\n", P1SMALL);
  env->p1medium1 = IMG_LoadTexture(ren, P1MEDIUM);
  if(!env->p1medium1)printf("Erreur lors du chargement d'une texture: %s\n", P1MEDIUM);
  env->p1medium2 = IMG_LoadTexture(ren, P1MEDIUM);
  if(!env->p1medium2)printf("Erreur lors du chargement d'une texture: %s\n", P1MEDIUM);
  env->p1large1 = IMG_LoadTexture(ren, P1LARGE);
  if(!env->p1large1)printf("Erreur lors du chargement d'une texture: %s\n", P1LARGE);
  env->p1large2 = IMG_LoadTexture(ren, P1LARGE);
  if(!env->p1large2)printf("Erreur lors du chargement d'une texture: %s\n", P1LARGE);
  env->p2small1 = IMG_LoadTexture(ren, P2SMALL);
  if(!env->p2small1)printf("Erreur lors du chargement d'une texture: %s\n", P2SMALL);
  env->p2small2 = IMG_LoadTexture(ren, P2SMALL);
  if(!env->p2small2)printf("Erreur lors du chargement d'une texture: %s\n", P2SMALL);
  env->p2medium1 = IMG_LoadTexture(ren, P2MEDIUM);
  if(!env->p2medium1)printf("Erreur lors du chargement d'une texture: %s\n", P2MEDIUM);
  env->p2medium2 = IMG_LoadTexture(ren, P2MEDIUM);
  if(!env->p2medium2)printf("Erreur lors du chargement d'une texture: %s\n", P2MEDIUM);
  env->p2large1 = IMG_LoadTexture(ren, P2LARGE);
  if(!env->p2large1)printf("Erreur lors du chargement d'une texture: %s\n", P2LARGE);
  env->p2large2 = IMG_LoadTexture(ren, P2LARGE);
  if(!env->p2large2)printf("Erreur lors du chargement d'une texture: %s\n", P2LARGE);
  env->imgP1 = IMG_LoadTexture(ren, IMGP1);
  if(!env->imgP1)printf("Erreur lors du chargement d'une texture: %s\n", IMGP1);
  env->imgP2 = IMG_LoadTexture(ren, IMGP2);
  if(!env->imgP2)printf("Erreur lors du chargement d'une texture: %s\n", IMGP2);
  /*
  Chargement des différentes images, la console affichera une erreur si une image n'existe pas
  */
  env->imgPlayer[0]=0;
  env->imgPlayer[0]=0;
  env->imgPlayer[1]=2000;
  env->imgPlayer[1]=2000;
  /*
  Position par défaut du joueur actuel (si les coordonnées sont supérieurs aux dimensions de la fenêtre, l'image n'est pas visible)
  */
  return env;
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

void render(SDL_Window* win, SDL_Renderer* ren, Env * env){
  SDL_Rect rect;
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  SDL_RenderCopy(ren, env->background, NULL, NULL);//Création du fond de la fenêtre

  //Rendu des petites pièces
  SDL_QueryTexture(env->p1small1, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[0][0][0]; rect.y = env->piece_y[0][0][0];
  SDL_RenderCopy(ren, env->p1small1, NULL, &rect);
  SDL_QueryTexture(env->p1small2, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[0][0][1]; rect.y = env->piece_y[0][0][1];
  SDL_RenderCopy(ren, env->p1small2, NULL, &rect);

  SDL_QueryTexture(env->p2small1, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[1][0][0]; rect.y = env->piece_y[1][0][0];
  SDL_RenderCopy(ren, env->p2small1, NULL, &rect);
  SDL_QueryTexture(env->p2small2, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[1][0][1]; rect.y = env->piece_y[1][0][1];
  SDL_RenderCopy(ren, env->p2small2, NULL, &rect);


  //Rendu des pièce moyennes
  SDL_QueryTexture(env->p1medium1, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[0][1][0]; rect.y = env->piece_y[0][1][0];
  SDL_RenderCopy(ren, env->p1medium1, NULL, &rect);
  SDL_QueryTexture(env->p1medium2, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[0][1][1]; rect.y = env->piece_y[0][1][1];
  SDL_RenderCopy(ren, env->p1medium2, NULL, &rect);

  SDL_QueryTexture(env->p2medium1, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[1][1][0]; rect.y = env->piece_y[1][1][0];
  SDL_RenderCopy(ren, env->p2medium1, NULL, &rect);
  SDL_QueryTexture(env->p2medium2, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[1][1][1]; rect.y = env->piece_y[1][1][1];
  SDL_RenderCopy(ren, env->p2medium2, NULL, &rect);


  //Rendu des grandes pièces
  SDL_QueryTexture(env->p1large1, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[0][2][0]; rect.y = env->piece_y[0][2][0];
  SDL_RenderCopy(ren, env->p1large1, NULL, &rect);
  SDL_QueryTexture(env->p1large2, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[0][2][1]; rect.y = env->piece_y[0][2][1];
  SDL_RenderCopy(ren, env->p1large2, NULL, &rect);

  SDL_QueryTexture(env->p2large1, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[1][2][0]; rect.y = env->piece_y[1][2][0];
  SDL_RenderCopy(ren, env->p2large1, NULL, &rect);
  SDL_QueryTexture(env->p2large2, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->piece_x[1][2][1]; rect.y = env->piece_y[1][2][1];
  SDL_RenderCopy(ren, env->p2large2, NULL, &rect);

  //Rendu du joueur actuel
  SDL_QueryTexture(env->imgP1, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->imgPlayer[0]; rect.y = env->imgPlayer[0];
  SDL_RenderCopy(ren, env->imgP1, NULL, &rect);

  SDL_QueryTexture(env->imgP2, NULL, NULL, &rect.w, &rect.h);
  rect.x = env->imgPlayer[1]; rect.y = env->imgPlayer[1];
  SDL_RenderCopy(ren, env->imgP2, NULL, &rect);
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e, board game, player * current_player,int * nbtours){
  int w,h,temp_x,temp_y,echec=0,temp_player=0;
  bool placer=false;
  SDL_GetWindowSize(win,&w,&h);
  if(e->type==SDL_QUIT)return true;//Quitte le jeu si le joueur ferme la fenêtre (voir la fonction main)
  else if(e->type==SDL_MOUSEBUTTONDOWN){//Détecte si le joueur a fait un click sur la fenêtre
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x,&mouse.y);//Récupère les coordonnéesou le joueur à cliquer
    if(mouse.x>=750){//Détecte si le joueur a cliqué sur la partie droite du jeu (contenus des maisons)
      temp_x=(mouse.x-750)/88+1;
      temp_y=(mouse.y-300)/88+1;
      //Ces calculs permettent de savoir quel pièce le joueur a sélectionné
      if(*current_player==PLAYER_2)temp_player=1;
      if(temp_y!=(temp_player+1)){//Détection si le joueur ne choisis pas ses pièces
        printf("Veillez attendre votre tour pour jouer");
        return false;//Quitte la fonction
      }
      size taille;
      switch ((temp_x-1)/2+1) {//Récupère la taille de la pièce
        case 1:
          taille=SMALL;
          break;
        case 2:
          taille=MEDIUM;
          break;
        case 3:
          taille=LARGE;
          break;
      } ;
      if(temp_x>=1&&temp_x<=6&&(temp_y==1||temp_y==2)&&get_nb_piece_in_house(game,*current_player,temp_y)>0){
        //Vérifie si le joueur a sélectionné une vraie pièce (clique dans le vide)
        //et si le joueur possède la pièce sélectionné
        SDL_Event e2;
        while(!placer){//Tant que la pièce n'a pas été placé
          while(SDL_PollEvent(&e2)){
            if(e2.type==SDL_QUIT)return true;
            else if(e2.type==SDL_MOUSEBUTTONDOWN){
              SDL_Point mouse2;
              int temp_x2,temp_y2;
              SDL_GetMouseState(&mouse2.x,&mouse2.y);//Récupère les coordonnéesou le joueur à cliquer
              if(mouse2.x<=600&&mouse2.x>=35&&mouse2.y>=80&&mouse2.y<=630){//Vérifie si le joueur a cliqué sur le plateau
                SDL_GetMouseState(&mouse2.x,&mouse2.y);
                temp_x2=(mouse2.x-60)/170+1;
                temp_y2=(mouse2.y-60)/170+1;
                echec=place_piece(game,*current_player,taille,temp_x2-1,temp_y2-1);
          			switch(echec){
          				case 0:
                    placer=true;
                    (*nbtours)++;
                    env->piece_x[temp_player][(temp_x-1)/2][(temp_x-1)%2]=60+(temp_x2-1)*180;
                    env->piece_y[temp_player][(temp_x-1)/2][(temp_x-1)%2]=99+(temp_y2-1)*180;
                    //Change les coordonnées de la pièce sélectionné selon la sélection du joueur
                    *current_player=next_player(*current_player);//Change le joueur actuel
                    int echange=env->imgPlayer[0];
                    env->imgPlayer[0]=env->imgPlayer[1];
                    env->imgPlayer[1]=echange;
                    //Change les coordonnées de l'image qui désigne le joueur actuel
          					break;
          				case 1:
          					printf("\033[31;01mErreur:\n");
          					printf("La pièce sélectionnée ne peut être placée\n\n");
          					break;
          				case 2:
          					printf("\033[31;01mErreur:\n");
          					printf("Vous ne pouvez pas placer votre pièce sur la case indiquée\n\n");
          					break;
          				case 3:
          					printf("\033[31;01mErreur:\n");
          					printf("Vous avez indiqué un emplacement inexistant\n\n");
          					break;
            		}
            		printf("\033[0;01m");
              }
            }
          }
        }
      }
    }
    else if(mouse.x<=600&&mouse.x>=35&&mouse.y>=80&&mouse.y<=630&&(*nbtours)>2){//Détection si le joueur a cliqué sur le plateau
      temp_x=(mouse.x-60)/170+1;
      temp_y=(mouse.y-100)/170+1;
      //Ces calculs permettent de savoir quel pièce le joueur a sélectionné
      if(*current_player==PLAYER_2)temp_player=1;
      if(get_place_holder(game, temp_x-1, temp_y-1)==next_player(*current_player)){
        printf("Veillez attendre votre tour pour jouer\n\n");
        return false;
      }
      //Détecte si le joueur ne choisis pas ses pièces
      if(get_place_holder(game, temp_x-1, temp_y-1)!=*current_player){
        printf("Vous ne possédez pas de pièce à l'emplacement visé\n\n");
        return false;
      }
      //Détecte si le joueur ne possède pas de pièce à l'emplacement cliqué
      size taille= get_piece_size(game, temp_x-1, temp_y-1);
      int val_taille;
      switch (taille) {
        case SMALL:
          val_taille=0;
          break;
        case MEDIUM:
          val_taille=1;
          break;
        case LARGE:
          val_taille=2;
          break;
        case NONE:
          break;
      }
      SDL_Event e2;
      while(!placer){//Tant que la pièce n'a pas été déplacé
        while(SDL_PollEvent(&e2)){
          if(e2.type==SDL_QUIT)return true;
          else if(e2.type==SDL_MOUSEBUTTONDOWN){
            SDL_Point mouse2;
            SDL_GetMouseState(&mouse2.x,&mouse2.y);
            if(mouse2.x<=600&&mouse2.x>=35&&mouse2.y>=80&&mouse2.y<=630&&(*nbtours)>2){//Vérifie si le joueur a cliqué sur le plateau
              int temp_x2,temp_y2;
              temp_x2=(mouse2.x-60)/170+1;
              temp_y2=(mouse2.y-60)/170+1;
              if(temp_x2>=1&&temp_x2<=3&&temp_y2>=1&&temp_y2<=3){
                echec=move_piece(game,temp_x-1,temp_y-1,temp_x2-1,temp_y2-1);
          			switch(echec){
          				case 0:
                    placer=true;
                    (*nbtours)++;
                    int temp=0;
                    if(env->piece_x[temp_player][val_taille][1]==60+(temp_x-1)*180&&env->piece_y[temp_player][val_taille][1]==99+(temp_y2-1)*180)temp=1;
                    env->piece_x[temp_player][val_taille][temp]=60+(temp_x2-1)*180;
                    env->piece_y[temp_player][val_taille][temp]=99+(temp_y2-1)*180;
                    //Change les coordonnées de la pièce sélectionné selon la sélection du joueur
                    *current_player=next_player(*current_player);//Change le joueur actuel
                    int echange=env->imgPlayer[0];
                    env->imgPlayer[0]=env->imgPlayer[1];
                    env->imgPlayer[1]=echange;
                    //Change les coordonnées de l'image qui désigne le joueur actuel
          					break;
          				case 1:
          					printf("\033[31;01mErreur:\n");
          					printf("La pièce sélectionnée ne peut être placée\n\n");
          					break;
          				case 2:
          					printf("\033[31;01mErreur:\n");
          					printf("Vous ne pouvez pas placer votre pièce sur la case indiquée\n\n");
          					break;
          				case 3:
          					printf("\033[31;01mErreur:\n");
          					printf("Vous avez indiqué un emplacement inexistant\n\n");
          					break;
          			}
                if(echec!=0)return false;
          			printf("\033[0;01m");
              }
            }
          }
        }
      }
    }
  }
  return false;
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

void clean(SDL_Window* win, SDL_Renderer* ren, Env * env){
  SDL_DestroyTexture(env->background);
  SDL_DestroyTexture(env->p1small1);
  SDL_DestroyTexture(env->p1small2);
  SDL_DestroyTexture(env->p2small1);
  SDL_DestroyTexture(env->p2small2);
  SDL_DestroyTexture(env->p1medium1);
  SDL_DestroyTexture(env->p1medium2);
  SDL_DestroyTexture(env->p2medium1);
  SDL_DestroyTexture(env->p2medium2);
  SDL_DestroyTexture(env->p1large1);
  SDL_DestroyTexture(env->p1large2);
  SDL_DestroyTexture(env->p2large1);
  SDL_DestroyTexture(env->p2large2);
  free(env);
  //Détruit toutes les textures chargé et libère la mémoire utilisé par l'environnement
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

int main(int argc, char *argv[]){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){//Initialise la fenêtre et affiche une erreur en cas de problème
        printf("Erreur lors de l'initialisation de la SDL : %s ", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    SDL_Window * win = SDL_CreateWindow("Gobblets Gobblers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);//Crée la fenêtre et affiche une erreur en cas de problème
    if(win == 0){
        printf("Erreur lors de la creation de la fenetre : %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    SDL_Renderer * ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);//Affiche la fenêtre et affiche une erreur en cas de problème
    if(!ren){
      printf("Erreur lors de la creation du rendu : %s", SDL_GetError());
      SDL_Quit();
      return -1;
    }
    Env * env = init(win, ren, argc, argv);//Crée un environnement de jeu
    board game = new_game();
    player current_player,winner=NO_PLAYER;
    bool quit = false;
    srand(time(NULL));
    int nbtours=1,random = rand()%2;
    printf("\033[36;01mLe hasard a décidé que le joueur qui commence serait le joueur %d\033[0;01m\n",random+1);
    if(random==0)current_player=PLAYER_1;
		else{
			current_player=PLAYER_2;
      env->imgPlayer[0]=2000;
      env->imgPlayer[0]=2000;
      env->imgPlayer[1]=0;
      env->imgPlayer[1]=0;
      /*
      Modifie l'image du joueur actuel si le joueur 2 commence en premier
      */
		}

    while(!quit&&winner==NO_PLAYER){//Tant que la variable quit ne vaut pas true et qu'il n'y a pas de gagnant
      SDL_Event e;
      while(SDL_PollEvent(&e)){
        quit=process(win, ren, env, &e,game,&current_player,&nbtours);//Lance la commande process et peut modifier les variables e,game,current_player et nbtours
      }
      winner=get_winner(game);//Récupère le gagnant
      SDL_SetRenderDrawColor(ren, 0xA0, 0xA0, 0xA0, 0xFF);//
      SDL_RenderClear(ren);//Supprime le contenu à l'écran pour évité d'avoir deux fois le même rendu superposé
      render(win, ren, env);
      SDL_RenderPresent(ren);
      SDL_Delay(DELAY);//Delai pour éviter de surchager l'ordinateur
    }
    current_player=next_player(current_player);
    printf("\n\033[32;01mBravo, vous avez gagné joueur %d après %d tours\033[0;01m\n\n",current_player,nbtours);
    clean(win, ren, env);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    destroy_game(game);
    return EXIT_SUCCESS;
}
