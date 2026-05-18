#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ncurses.h>

#include"Canon.h"
#include"Affichage.h"
#include"Flechette.h"
#include"Canard.h"
#include"Tourelle.h"

enum evenement {CONTINUE, ECHAP, DEFAITE, VICTOIRE};

void afficher_ecran_fin(enum evenement res);
void Jeu_Tir(int L, int H, int nivdiff);

int main(int argc, char **argv)
{
    if (argc != 4){
        printf("Usage: %s L H niv\n", argv[0]);
        exit(1);
    }
    srand(time(NULL));
    Jeu_Tir(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    return EXIT_SUCCESS;
}

/***************************************/

/* Affiche l'écran de victoire ou défaite en ncurses */
void afficher_ecran_fin(enum evenement res)
{
    endwin();  // On quitte ncurses avant les printf

    if (res == VICTOIRE) {
        printf("\n\n");
        printf("  +--------------------------------------+\n");
        printf("  |                                      |\n");
        printf("  |            VICTOIRE !                |\n");
        printf("  |                                      |\n");
        printf("  |  Vous avez abattu tous les canards   |\n");
        printf("  |                                      |\n");
        printf("  +--------------------------------------+\n");

    } else if (res == DEFAITE) {
        printf("\n\n");
        printf("  +--------------------------------------+\n");
        printf("  |                                      |\n");
        printf("  |             DEFAITE !                |\n");
        printf("  |                                      |\n");
        printf("  |    Votre canon a ete detruit...      |\n");
        printf("  |                                      |\n");
        printf("  +--------------------------------------+\n");

    } else {  /* ECHAP */
        printf("\n  Partie abandonnee.\n");
    }

    printf("\n  Appuyez sur Entree pour quitter...\n");
    getchar();
}

/***************************************/

void Jeu_Tir(int L, int H, int nivdiff)
{
    int ch;
    enum evenement res = CONTINUE;

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    halfdelay(1);

    Affichage       *A  = Affichage_initialiser(L, H);
    Canon           *C  = Canon_initialiser(0, L, H);
    Liste_Canard    *LC = Liste_Canard_initialiser_vide(L, H, nivdiff);
    Liste_Flechette *LF = Liste_Flechette_initialiser_vide(H, nivdiff);
    Liste_Tourelle  *LT = Liste_Tourelle_initialiser_vide(L, H, nivdiff);
    Liste_Laser     *LL = Liste_Laser_initialiser_vide();

    enum action_canon actC;

    do {
        ch = getch();

        Affichage_vider(A);
        Affichage_cadre(A);
        Canon_afficher(C, A);
        Canard_afficher(LC, A);
        Flechette_afficher(LF, A);
        Tourelle_afficher(LT, A);
        Laser_afficher(LL, A);
        Affichage_dessiner(A);

        /* Barre de statut sous le cadre */
        printf("\33[0m");   // reset couleursert
        printf("\33[32m PV Canon : ");          // v
        // Couleur des PV selon la valeur
        if      (C->pv > 6) printf("\33[32m");  // vert si PV élevés
        else if (C->pv > 3) printf("\33[33m");  // orange si PV moyens
        else                printf("\33[31m");  // rouge si PV bas
        printf("%d", C->pv);

        printf("\33[37m  |  ");                 // blanc

        printf("\33[36m Canards : ");           // cyan
        printf("\33[37m%d/%d", LC->nb_tues, LC->nb_total);

        printf("\33[37m  |  ");

        printf("\33[35m Tourelles : ");         // magenta
        printf("\33[37m%d", LT->nb_tourelles);

        printf("\33[37m  |  ");

        printf("\33[33m ESC pour quitter ");    // jaune

        printf("\33[0m\n");                     // reset
        fflush(stdout);
        refresh();

        /* Entrées clavier */
        if (ch == 27)      res = ECHAP;
        if (ch == KEY_LEFT)  Canon_action(C, Gauche_canon);
        if (ch == KEY_RIGHT) Canon_action(C, Droite_canon);
        if (ch == 32)        ajouter_flechette(LF, C->ypos + C->largeur / 2);

        /* Actions automatiques */
        Flechette_action(LF);
        Canard_action(LC);
        Tourelle_action(LT, LL, C->ypos, C->xpos);
        Laser_action(LL);

        /* Interactions */
        Interaction_Flechette_Canard(LF, LC);
        Interaction_Flechette_Tourelle(LF, LT);
        Interaction_Laser_Canon(LL, C);

        /* Spawn */
        ajouter_canard(LC);
        if (rand() % 150 == 0) ajouter_tourelle(LT, C->xpos); // 1 chance sur 150 qu'une tourelle apparaît

        /* Conditions de fin */
        if (C->pv <= 0)
            res = DEFAITE;

        if (LC->nb_tues >= LC->nb_total && LC->nb_canards == 0)
            res = VICTOIRE;

    } while (res == CONTINUE);

    /* Écran de fin AVANT endwin */
    afficher_ecran_fin(res);

    Affichage_desallouer(&A);
    Canon_desallouer(&C);
    Flechette_desallouer(&LF);
    Canard_desallouer(&LC);
    Tourelle_desallouer(&LT);
    Laser_desallouer(&LL);
}
