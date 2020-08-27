#include <stdio.h>
#include <stdlib.h>
#include "board.h"

typedef enum sizes_e size;

typedef enum players_e player;

struct board_s{
  int box[DIMENSIONS][DIMENSIONS];
  /*
  box[line][column]

  Si une pièce est placée sur une case:
  - Si elle est grande, la case sera incrémenté de 100
  - Si elle est moyenne, la case sera incrémenté de 10
  - Si elle est petite, la case sera incrémenté de 1
  Attention, si le joueur actuel est le joueur 2, la case sera incrémenté deux fois
  (200 pour une grande pièce, 20 pour une pièce moyenne, 2 pour une petite pièce)

  Exemple:
  Si une case possède une grande pièce du joueur 1 et une petite pièce du joueur 2, la case vaudra 102
  */
  int house[NB_PLAYERS];
  /*
  house[nom_du_joueur]

  Par défaut
  la centaine représente les grandes pièce, la dizaine, les pièces moyennes, les unités les petites pièces
  Chaque valeur commence à 2 car un joueur possède 2 pièces
  */
};

//+--------------------------------------------------------------------------------------------------------------------------------------+

board copy_game(board original_game){
  board copy=malloc(sizeof(struct board_s));//Copie les données du jeu original dans copy
  for(int i=0;i<DIMENSIONS;i++){
    for(int j=0;j<DIMENSIONS;j++){
      copy->box[i][j]=original_game->box[i][j];
      //Initialise chaque case par 0
    }
  }
  for(int i=0;i<NB_PLAYERS;i++)(*copy).house[i]=(*original_game).house[i];
  return copy;
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

void destroy_game(board game){
  free(game);//Libère l'espace alloué dynamiquement pour le jeu
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

int get_nb_piece_in_house(board game, player checked_player, size piece_size){
  int temp;
  if(checked_player==NO_PLAYER)return 0;
  switch (piece_size) {
    case SMALL:
      temp=game->house[checked_player-1]-(game->house[checked_player-1]/10)*10;
      //On soustrait le nombre de pièce dans chaque maison par le nombre de pièces moyennes et grandes
      break;
    case MEDIUM:
      temp=(game->house[checked_player-1]-(game->house[checked_player-1]/100)*100)/10;
      /*
      On soustrait le nombre de pièce de chaque maison par le nombre de grandes pièce
      on divise ensuite le résultat par 10 pour éliminer les petites pièces
      (vu que temp est de type int, les chiffres après la virgule ne seront pas conservés)
      */
      break;
    case LARGE:
      temp=(game->house[checked_player-1]/100);
      /*
      On divise le nombre de pièce de chaque maison par 100 pour élminer les pièce petites et moyennes
      (vu que temp est de type int, les chiffres après la virgule ne seront pas conservés)
      */
      break;
    case NONE:
      temp=0;
      break;
  }
  return temp;
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

size get_piece_size(board game, int line, int column){
  size res=NONE;
  if(game->box[line][column]!=0&&line<DIMENSIONS&&line>=0&&column<DIMENSIONS&&column>=0){//Regarde si une pièce est placé sur la case à vérifier et si son emplacement est correcte
    if(game->box[line][column]>=100){//Regarde si la pièce sur la case est grande
      res=LARGE;
    }else{
      if(game->box[line][column]>=10){//Sinon, regarde si elle est de taille moyenne
        res=MEDIUM;
      }else{//Sinon, renvoie que la pièce est de petite taille
        res=SMALL;
      }
    }
  }
  return res;
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

player get_place_holder(board game, int line, int column){
  player res=NO_PLAYER;
  int taille;
  if(game->box[line][column]!=0&&line<DIMENSIONS&&line>=0&&column<DIMENSIONS&&column>=0){//La suite du programme se lance uniquement si la case possède une pièce et si le nom de la case est correcte
    size piece_size = get_piece_size(game, line, column);
    switch (piece_size) {
      case SMALL:
        taille =1;
        break;
      case MEDIUM:
        taille =10;
        break;
      case LARGE:
        taille =100;
        break;
      default:
        taille=0;
        break;
    }
    if(taille!=0){
      if((game->box[line][column])/taille==2)res=PLAYER_2;
      else if((game->box[line][column])/taille==1)res=PLAYER_1;
    }
    /*
    La boucle regarde si une petite pièce est placé et renvoie dans la variable res, le joueur qui possède cette pièce.
    L'opération s'éxécute ensuite pour les pièces de tailles moyennes puis pour les grandes pièces.
    Ainsi, le joueur possédant la plus grande pièce sera indiqué dans la variable res.
    */
  }
  return res;
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

player get_winner(board game){
  int resLine1,resLine2,resColumn1,resColumn2;
  bool win1=false,win2=false;
  player winner=NO_PLAYER;
  for(int i=0;i<DIMENSIONS&&winner==NO_PLAYER;i++){//Pour chacune des 3 possiblités en lignes et en colonnes (la boucle s'intérromp si un gagnant est désigné)
    resLine1=0;
    resLine2=0;
    resColumn1=0;
    resColumn2=0;
    for(int j=0;j<DIMENSIONS;j++){//Incrémente une valeur selon le joueur qui controle la case sur une ième ligne et sur une ième colonne
      //Vérification pour les lignes:
      if(get_place_holder(game, i, j)==PLAYER_1)resLine1++;
      else{
        if(get_place_holder(game, i, j)==PLAYER_2)resLine2++;
      }
      //Vérification pour les colonnes
      if(get_place_holder(game, j, i)==PLAYER_1)resColumn1++;
      else{
        if(get_place_holder(game, j, i)==PLAYER_2)resColumn2++;
      }
    }
    if(resLine1==3||resColumn1==3)win1=true;//Vérifie si le joueur 1 controle la ième ligne ou la ième colonne et retourne son identifiant si il a gagné
    if(resLine2==3||resColumn2==3)win2=true;//Vérification pour le joueur 2
  }
  if(winner==NO_PLAYER){//Si il n'y a aucun vainqueur
    int resDiag11=0,resDiag12=0,resDiag21=0,resDiag22=0;
    for(int j=0;j<DIMENSIONS;j++){//Vérification pour les 2 diagonales
      if(get_place_holder(game,j,j)==PLAYER_1)resDiag11++;
      else{
        if(get_place_holder(game,j,j)==PLAYER_2)resDiag12++;
      }
      if(get_place_holder(game,j,DIMENSIONS-j-1)==PLAYER_1)resDiag21++;
      else{
        if(get_place_holder(game,j,DIMENSIONS-j-1)==PLAYER_2)resDiag22++;
      }
    }
    if(resDiag11==3||resDiag21==3)win1=true;//Vérifie si le premier joueur contrôle une des deux colonnes
    if(resDiag12==3||resDiag22==3)win2=true;//Vérifie si le deuxième joueur contrôle une des deux colonnes
  }
  if(win1)winner=PLAYER_1;
  else if(win2)winner=PLAYER_2;
  if(win1&&win2)winner=NO_PLAYER;
  return winner;
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

int move_piece(board game, int source_line, int source_column, int target_line, int target_column){
  int temp;
  size piece_size=get_piece_size(game,source_line,source_column);
  if((target_line<DIMENSIONS)&&(target_column<DIMENSIONS)&&(target_line>=0)&&(target_column>=0)){//Vérifie si les lignes et colonnes indiqué respectes les dimensions du plateau.
    if(piece_size!=NONE){//Vérifie si la pièce source (celle qui doit être déplacé) existe
  		if(get_piece_size(game,target_line,target_column)<piece_size){//Vérifie si la case n'est pas déjà occuper par une pièce de la même taille ou une plus grande.
          switch (piece_size) {
            case SMALL:
              temp=1;
              break;
            case MEDIUM:
              temp=10;
              break;
            case LARGE:
              temp=100;
              break;
            case NONE:
              temp=0;
              break;
          }
          if(get_place_holder(game, source_line, source_column)==PLAYER_2)temp*=2;
          game->box[source_line][source_column]-=temp;//Enlève la pièce sur le plateau.
          game->box[target_line][target_column]+=temp;//Rajoute la pièce sur le plateau.
  				return 0;
  		}
			else{
				return 2;
			}
		}
		else{
			return 1;
		}
	}
	else{
		return 3;
	}
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

player next_player(player current_player){
  if(current_player==PLAYER_1)current_player=PLAYER_2;
  else{
    current_player=PLAYER_1;
  }
  return current_player;
}

//+--------------------------------------------------------------------------------------------------------------------------------------+

board new_game(){
  board new_board=malloc(sizeof(struct board_s));//Crée un plateau alloué dynamiquement
  for(int i=0;i<DIMENSIONS;i++){
    for(int j=0;j<DIMENSIONS;j++){
      new_board->box[i][j]=0;
      //Initialise chaque case par 0
    }
  }
  for(int i=0;i<NB_PLAYERS;i++)(*new_board).house[i]=222;
  //Par défaut, chaque joueur à 2 pièce de chaque type
  return new_board;
}

int place_piece(board game, player current_player, size piece_size, int line, int column){
  int res,temp;
  if((line<DIMENSIONS)&&(column<DIMENSIONS)&&(line>=0)&&(column>=0)){//Vérifie si les lignes et colonnes indiqué respectes les dimensions du plateau.
  	if(get_nb_piece_in_house(game,current_player,piece_size)>0){//Vérifie si le joueurs possède au moins une pièce de taille pièce_size dans sa maison.
  		if(get_piece_size(game,line,column)<piece_size){//Vérifie si la case n'est pas déjà occuper par une pièce de la même taille ou une plus grande.
          res= 0;
          switch (piece_size) {
            case SMALL:
              temp=1;
              break;
            case MEDIUM:
              temp=10;
              break;
            case LARGE:
              temp=100;
              break;
            case NONE:
              temp=0;
              break;
          }
          if(current_player==2){//Vérifie si le joueur actuel est le joueur 2. On retire la pièce du joueur qui joue de sa maison.
            game->house[1]-=temp;
            temp*=2;//On multiplie la variable par 2 pour indiquer que la pièce appartient au joueur 2 (voir l'explication dans la structure de board).
          }else{
            game->house[0]-=temp;
          }
          game->box[line][column]+=temp;//Rajoute la pièce sur le plateau.
      }
			else{
				res= 2;
			}
		}
		else{
			res= 1;
		}
	}
	else{
		res= 3;
	}
  return res;
}
