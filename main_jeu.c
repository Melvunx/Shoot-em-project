#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>

#include"Canon.h"
#include"Affichage.h"

enum evenement {CONTINUE, ECHAP};

void Jeu_Tir(int tailleL, int tailleH, int nivdiff);

/***************************************/

int main(int argv, char **argc){

  if (argv!=4){
     printf("Usage: %s <largeur> <hauteur> <niveau de difficulté>\n",argc[0]);
     exit(2);
  }
  srand(time(NULL));

  Jeu_Tir(atoi(argc[1]), atoi(argc[2]), atoi(argc[3]));
  
  return EXIT_SUCCESS;
}

/***************************************/

void Jeu_Tir(int tailleL, int tailleH, int nivdiff){

  int ch;
  enum evenement res;
  
  /* Pas d'initialisation nécessaire avec conio */

  res = CONTINUE;
  
  Affichage *A = Affichage_initialiser(tailleL, tailleH);  
  Canon *C = Canon_initialiser(0, tailleL, tailleH);
  enum action_canon actC;

  /* Boucle événementielle du jeu */ 
  do{

    /* Remplace halfdelay(1) + getch() de ncurses */
    if (_kbhit()){   /* Si une touche est disponible */
        ch = _getch();

        /* Les touches spéciales (flèches) envoient d'abord 0 ou 224 */
        if (ch == 0 || ch == 224){
            ch = _getch();  /* 2ème appel pour récupérer la vraie touche */
            switch(ch){
                case 75:  /* Flèche gauche */
                    actC = Gauche_canon;
                    break;
                case 77:  /* Flèche droite */
                    actC = Droite_canon;
                    break;
                default:
                    break;
            }
        } else {
            switch(ch){
                case 27:  /* ESC */
                    res = ECHAP;
                    break;
                case 32:  /* ESPACE */
                    actC = Tir;
                    break;
                default:
                    break;
            }
        }

        if (res != ECHAP){
            Canon_action(C, actC);
        }

    } else {
        ch = -1;  /* Aucune touche appuyée */
    }

    Affichage_vider(A);
    Affichage_cadre(A);
    Canon_afficher(C, A);
    Affichage_dessiner(A);
    printf("Pour jouer: utiliser les flèches, SPACE pour tirer (ESC pour Sortir)\33[1E\33[1E");
    fflush(stdout);

  } while (res == CONTINUE);
  
  if (res == ECHAP)
      printf("\n\nVous avez taper ESC pour sortir. \33[1E\n");

  printf("\33[1EAppuyez sur une touche pour sortir\33[1E\n");
  while(!_kbhit());  /* Attend n'importe quelle touche */
  _getch();

  /* Pas de endwin() avec conio */

  Affichage_desallouer(&A);  
  Canon_desallouer(&C);
}