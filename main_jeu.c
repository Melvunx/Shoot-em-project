   #include<stdio.h>
   #include<stdlib.h>
   #include<time.h>
   #include<ncurses.h>

   #include"Canon.h"
   #include"Affichage.h"
   #include"Flechette.h"
   #include"Canard.h"

   // Liste des évènements possibles
   enum evenement {CONTINUE, ECHAP};
      // CONTINUE: le jeu debute ou continue
      // ECHAP: la touche ESC a été utilisée
      // (on peut en ajouter d'autres (victoire etc)

   /* Coeur du jeu contenant la boucle événementielle */
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

   int ch;   /* Récupération de la saisie clavier, valeur # pour fin de jeu */
   enum evenement res; /* Retour des évènements  */
   char *nomFic = "Hello";

   /* Initialisation de ncurses et du clavier(4*/
   initscr();
   raw();
   keypad(stdscr, TRUE);
   noecho();
   halfdelay(1);  /* Temps d'exécution max de getch à 1/10ème de seconde */

   /* Initialisation des éléments du jeu */

   res= CONTINUE;

   Affichage *A=Affichage_initialiser(tailleL, tailleH);

   Canon *C = Canon_initialiser(0, tailleL, tailleH);
      Liste_Canard *LC = Liste_Canard_initialiser_vide(tailleL, tailleH, nivdiff, nomFic);
      Liste_Flechette *LF = Liste_Flechette_initialiser_vide(tailleL, tailleH);
   enum action_canon actC;




   /* Boucle événementielle du jeu */
   do{

      ch = getch(); /* Si aucune touche utilisée, getch renvoie -1 */

      Affichage_vider(A);
      Affichage_cadre(A);
      Canon_afficher(C, A);
      Canard_afficher(LC, A);
      Flechette_afficher(LF, A);

      // Autre affichage
      Affichage_dessiner(A);
      printf("Pour jouer: utiliser les flèches, SPACE pour tirer (ESC pour Sortir)\33[1E\33[1E");
      fflush(stdout);  /* Force l'affichage complet	 */



      if ( ch!=-1 ){ /* Si une touche a été appuyée */

      switch(ch) {
         case 27:  /* Code ASCII de la touche ESC-Echap */
            res=ECHAP;
            break;
         case KEY_LEFT:
            actC = Gauche_canon;
            break;
         case KEY_RIGHT:
            actC = Droite_canon;
            break;
         case 32:  /* Code ASCII de la touche ESPACE */
            actC = Tir;
            ajouter_flechette(LF, C->ypos + C->largeur / 2);
            break;
         default:
            break;
         }

         if (res!=ECHAP){
      // Zone des actions dirigeant un élément
            Canon_action(C, actC);
         }

      }
      Canard_action(LC);
      Flechette_action(LF);
      ajouter_canard(LC);
      // Zone des actions d'éléments agissant automatiquement
      // Comme les fléchettes et les canards
   } while (res== CONTINUE);


   if (res==ECHAP)
         printf("\n\nVous avez taper ESC pour sortir. \33[1E\n");

   printf("\33[1EAppuyez sur une touche pour sortir\33[1E\n");
   do{
         ch = getch();
   }while(ch==-1);


   endwin(); /* Doit obligatoirement etre mis en fin de programme pour remettre le terminal en etat */

   Affichage_desallouer(&A);
   Canon_desallouer(&C);
      Flechette_desallouer(&LF);
   Canard_desallouer(&LC);

   }
