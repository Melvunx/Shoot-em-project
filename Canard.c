#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Canard.h"
#include "Affichage.h"

#define MARGE 4

Canard* initialiser_canard(int largeur_aff, int hauteur_aff)
{
  Canard* c = malloc(sizeof(Canard));
  if (c == NULL) return NULL;

  c->direction = rand() % 2;
  c->y = 1 + rand() % (hauteur_aff - MARGE);

  if (c->direction == DROITE)
      c->x = 1;                        // apparaît à gauche, va vers la droite
  else
      c->x = largeur_aff - MARGE;          // apparaît à droite, va vers la gauche

  c->suivant = NULL;
  return c;
}
Liste_Canard* Liste_Canard_initialiser_vide(int largeur_aff, int hauteur_aff, int nivdiff, char *nomfic)
{
  Liste_Canard* Lcanard = malloc(sizeof(Liste_Canard));
  if (Lcanard == NULL) return NULL;

  Lcanard->tete = NULL;
  Lcanard->hauteur = 2;           // hauteur visuelle du canard en cases
  Lcanard->largeur = 3;           // largeur visuelle du canard en cases
  Lcanard->nb_max_canards = nivdiff;
  Lcanard->nb_canards = 0;
  Lcanard->pas = 1;
  strcpy(Lcanard->apparence, "🦆");

  Lcanard->coord_apparition  = 1;              // ligne où apparaissent les canards
  Lcanard->coord_disparition = hauteur_aff - 1; // colonne de sortie à droite

  return Lcanard;
}

void Canard_desallouer(Liste_Canard** Lcanard)
{
  if (Lcanard == NULL || *Lcanard == NULL) return;

  Canard *cour = (*Lcanard)->tete;
  while (cour != NULL)
  {
    Canard *suiv = cour->suivant;
    free(cour);
    cour = suiv;
  }
  
  free(*Lcanard);
  *Lcanard = NULL;
}

void ajouter_canard(Liste_Canard* Lcanard)
{
  if (Lcanard->nb_max_canards == Lcanard->nb_canards) return;
  
  Canard* nouv = initialiser_canard(Lcanard->largeur, Lcanard->hauteur);
  if (nouv == NULL) return;

  nouv->suivant = Lcanard->tete;
  Lcanard->tete = nouv;

  Lcanard->nb_canards++;
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

        // Tête et bec selon la direction
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

        canard_courant = canard_courant->suivant;
    }
}

void Canard_action(Liste_Canard *Lcanard) {
  if (Lcanard == NULL || Lcanard->tete == NULL) return;

  Canard *courant = Lcanard->tete;
  Canard *precedent = NULL;

  while (courant != NULL) {

    if (courant->direction == DROITE) courant->x += Lcanard->pas;
    else                              courant->x -= Lcanard->pas;

    // Sorti par la droite ou par la gauche
    int sorti = (courant->x >= Lcanard->coord_disparition || courant->x <= 0);

    if (sorti) {
      Canard *a_supprimer = courant;
      if (precedent == NULL) Lcanard->tete = courant->suivant;
      else                   precedent->suivant = courant->suivant;

      courant = courant->suivant;
      free(a_supprimer);
      Lcanard->nb_canards--;   // ← nb_canards et non nb_max_canards
    } else {
      precedent = courant;
      courant = courant->suivant;
    }
  }
}
