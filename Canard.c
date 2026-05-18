#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Canard.h"
#include "Flechette.h"

#define MARGE 4

Canard* initialiser_canard(int largeur_aff, int hauteur_aff)
{
  Canard* c = malloc(sizeof(Canard));
  if (c == NULL) return NULL;

  c->direction = rand() % 2;
  c->y = 1 + rand() % (hauteur_aff - MARGE);
  c->pv = 4;

  if (c->direction == DROITE)
      c->x = 1;
  else
      c->x = largeur_aff - MARGE;

  c->suivant = NULL;
  return c;
}

Liste_Canard* Liste_Canard_initialiser_vide(int largeur_aff, int hauteur_aff, int nivdiff){
  Liste_Canard* L = malloc(sizeof(Liste_Canard));

  L->tete = NULL;
  L->hauteur = 2;
  L->largeur = 3;
  L->largeur_aff = largeur_aff;
  L->hauteur_aff = hauteur_aff;

  L->nb_max_canards = nivdiff;
  L->nb_canards = 0;
  L->nb_tues  = 0;
  L->nb_total = nivdiff * 2;

  L->pas = 1;
  L->coord_disparition = largeur_aff - 1;

  return L;
}

void Canard_desallouer(Liste_Canard** L){
  Canard *c = (*L)->tete;
  while(c){
    Canard *n = c->suivant;
    free(c);
    c = n;
  }
  free(*L);
  *L = NULL;
}

void ajouter_canard(Liste_Canard *Lcanard)
{
    // Ne plus spawner si on a déjà fait apparaître assez de canards
    if (Lcanard->nb_tues + Lcanard->nb_canards >= Lcanard->nb_total) return;
    if (Lcanard->nb_canards >= Lcanard->nb_max_canards) return;

    Canard *nouv = initialiser_canard(Lcanard->largeur_aff, Lcanard->hauteur_aff);
    if (nouv == NULL) return;

    nouv->suivant = Lcanard->tete;
    Lcanard->tete = nouv;
    Lcanard->nb_canards++;
}

void Canard_action(Liste_Canard *L){
  Canard *c = L->tete;
  Canard *p = NULL;

  while(c){
    if(c->direction == DROITE) c->x++;
    else c->x--;

    int sorti = (c->x <= 0 || c->x >= L->coord_disparition);

    if(sorti){
      Canard *sup = c;
      if(!p) L->tete = c->suivant;
      else p->suivant = c->suivant;

      c = c->suivant;
      free(sup);
      L->nb_canards--;
    } else {
      p = c;
      c = c->suivant;
    }
  }
}

void Canard_afficher(Liste_Canard *Lcanard, Affichage *A) {
    Canard *canard_courant = Lcanard->tete;

    while (canard_courant != NULL) {
        int col   = canard_courant->x;
        int ligne = canard_courant->y;

        // Corps
        for (int l = ligne; l < ligne + Lcanard->hauteur; l++)
            for (int c = col; c < col + Lcanard->largeur; c++)
                if (l >= 0 && l < A->H && c >= 0 && c < A->L)
                    strcpy(A->tab[l][c], "\33[42m ");

        // Tête et bec
        if (canard_courant->direction == DROITE) {
            int c1 = col + Lcanard->largeur - 1;
            int c2 = col + Lcanard->largeur;
            int cb = col + Lcanard->largeur + 1;
            if (ligne-1 >= 0 && c1 < A->L) strcpy(A->tab[ligne-1][c1], "\33[42m ");
            if (ligne-1 >= 0 && c2 < A->L) strcpy(A->tab[ligne-1][c2], "\33[42m ");
            if (ligne-1 >= 0 && cb < A->L) strcpy(A->tab[ligne-1][cb], "\33[43m ");
        } else {
            int c1 = col;
            int c2 = col - 1;
            int cb = col - 2;
            if (ligne-1 >= 0 && c1 >= 0) strcpy(A->tab[ligne-1][c1], "\33[42m ");
            if (ligne-1 >= 0 && c2 >= 0) strcpy(A->tab[ligne-1][c2], "\33[42m ");
            if (ligne-1 >= 0 && cb >= 0) strcpy(A->tab[ligne-1][cb], "\33[43m ");
        }

        // Affichage des PV au-dessus du canard
        char pv_str[16];
        snprintf(pv_str, sizeof(pv_str), "\33[37m%d", canard_courant->pv);
        if (ligne-2 >= 0 && col >= 0 && col < A->L)
            afficher_barre_vie(A, ligne - 2, col, canard_courant->pv, 3, "\33[33m+");  // jaune

        canard_courant = canard_courant->suivant;
    }
}
