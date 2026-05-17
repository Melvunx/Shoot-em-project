#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ncurses.h>

#include"Canon.h"
#include"Affichage.h"
#include"Flechette.h"
#include"Canard.h"

enum evenement {CONTINUE, ECHAP};

int victoire(Liste_Canard *LC)
{
    return LC->tete == NULL;
}

void Jeu_Tir(int L, int H, int nivdiff);

int main(int argc, char **argv)
{
    if (argc!=4){
        printf("Usage: %s L H niv\n", argv[0]);
        exit(1);
    }

    srand(time(NULL));
    Jeu_Tir(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]));
}

void Jeu_Tir(int L,int H,int nivdiff)
{
    int ch;
    enum evenement res=CONTINUE;

    initscr();
    raw();
    keypad(stdscr,TRUE);
    noecho();
    halfdelay(1);

    Affichage *A=Affichage_initialiser(L,H);
    Canon *C=Canon_initialiser(0,L,H);
    Liste_Canard *LC=Liste_Canard_initialiser_vide(L,H,nivdiff);
    Liste_Flechette *LF=Liste_Flechette_initialiser_vide(H,nivdiff);

    enum action_canon actC;

    do{
        ch=getch();

        Affichage_vider(A);
        Affichage_cadre(A);

        Canon_afficher(C,A);
        Canard_afficher(LC,A);
        Flechette_afficher(LF,A);

        Affichage_dessiner(A);

        if (ch==27) res=ECHAP;
        if (ch==KEY_LEFT) Canon_action(C,Gauche_canon);
        if (ch==KEY_RIGHT) Canon_action(C,Droite_canon);
        if (ch==32) ajouter_flechette(LF,C->ypos);

        Flechette_action(LF);
        Canard_action(LC);
        Interaction_Flechette_Canard(LF,LC);
        ajouter_canard(LC);

    } while(res==CONTINUE && !victoire(LC));

    endwin();

    if (victoire(LC)) printf("\nVICTOIRE\n");

    Affichage_desallouer(&A);
    Canon_desallouer(&C);
    Flechette_desallouer(&LF);
    Canard_desallouer(&LC);
}
