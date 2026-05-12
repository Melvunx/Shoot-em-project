#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

#include"Canon.h"
#include"Affichage.h"
#include"Flechette.h"
#include"Canard.h"

enum evenement {CONTINUE, ECHAP};

void Jeu_Tir(int tailleL, int tailleH, int nivdiff);

/* Codes de touches spéciales avec conio (getch() retourne 0 ou 224, puis le code) */
#define KEY_LEFT  75
#define KEY_RIGHT 77
#define KEY_ESC   27
#define KEY_SPACE 32

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

/* Fonction pour positionner le curseur (remplace ncurses) */
void gotoxy(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/* Efface la console Windows */
void clearScreen(){
    system("cls");
}

/***************************************/

void Jeu_Tir(int tailleL, int tailleH, int nivdiff){

  int ch;
  char *nomFic = "Hello";
  enum evenement res;
  enum action_canon actC;
  int besoin_redessiner = 1;  // 1 = on dessine au moins une fois au début

  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(hConsole, &cursorInfo);
  cursorInfo.bVisible = FALSE;
  SetConsoleCursorInfo(hConsole, &cursorInfo);

  res = CONTINUE;

  Affichage *A = Affichage_initialiser(tailleL, tailleH);
  Canon *C = Canon_initialiser(0, tailleL, tailleH);
  Liste_Canard *LC = Liste_Canard_initialiser_vide(tailleL, tailleH, nivdiff, nomFic);
  Liste_Flechette *LF = Liste_Flechette_initialiser_vide(tailleL, tailleH);


  do {

    ch = -1;
    if (_kbhit()){
        ch = _getch();
        if (ch == 0 || ch == 224){
            ch = _getch();
        }
    }

    /* On traite l'entrée clavier */
    if (ch != -1){
      switch(ch){
        case KEY_ESC:   res = ECHAP;        break;
        case KEY_LEFT:  actC = Gauche_canon; besoin_redessiner = 1; break;
        case KEY_RIGHT: actC = Droite_canon; besoin_redessiner = 1; break;
        case KEY_SPACE: actC = Tir;          besoin_redessiner = 1; break;
        default: break;
      }

      if (res != ECHAP){
        Canon_action(C, actC);
        Canard_action(LC);
      }
    }

    // Ici : actions automatiques (canards, fléchettes...)
    // Si vous en avez, mettez besoin_redessiner = 1; après chaque mouvement
    ajouter_canard(LC);
    besoin_redessiner = 1;

    /* On ne redessine QUE si nécessaire */
    if (besoin_redessiner){
      gotoxy(0, 0);
      Affichage_vider(A);
      Affichage_cadre(A);
      Canon_afficher(C, A);
      Affichage_dessiner(A);
      printf("Pour jouer: utiliser les fleches, SPACE pour tirer (ESC pour Sortir)\n\n");
      fflush(stdout);
      besoin_redessiner = 0;  // On remet à 0 jusqu'au prochain changement
    }

    Sleep(100);

  } while (res == CONTINUE);


  cursorInfo.bVisible = TRUE;
  SetConsoleCursorInfo(hConsole, &cursorInfo);

  if (res == ECHAP)
      printf("\n\nVous avez tape ESC pour sortir.\n\n");

  printf("Appuyez sur une touche pour sortir\n");
  while(!_kbhit());

  Affichage_desallouer(&A);
  Canon_desallouer(&C);
  Flechette_desallouer(&LF);
  Canard_desallouer(&LC);
}