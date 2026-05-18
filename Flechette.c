#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Flechette.h"

Liste_Flechette* Liste_Flechette_initialiser_vide(int hauteur_aff, int nivdiff){
  Liste_Flechette* F = malloc(sizeof(Liste_Flechette));

  F->tete = NULL;
  F->y_depart = hauteur_aff - 2;
  F->y_max = 1;
  F->nb_flechettes = 0;
  F->nb_max = nivdiff;

  strcpy(F->apparence,"|");
  return F;
}

void ajouter_flechette(Liste_Flechette* F, int y){
  Flechette* n = malloc(sizeof(Flechette));
  n->x = F->y_depart;
  n->y = y;
  n->suivant = F->tete;
  F->tete = n;
}

void Flechette_action(Liste_Flechette* F){
  Flechette *c = F->tete, *p=NULL;

  while(c){
    c->x--;

    if(c->x <= F->y_max){
      Flechette *sup = c;
      if(!p) F->tete = c->suivant;
      else p->suivant = c->suivant;

      c = c->suivant;
      free(sup);
    } else {
      p = c;
      c = c->suivant;
    }
  }
}

void Flechette_afficher(Liste_Flechette* F, Affichage* A){
  Flechette *c = F->tete;

  while(c){
    if(c->x>=0 && c->x<A->H && c->y>=0 && c->y<A->L){
      strcpy(A->tab[c->x][c->y],"|");
    }
    c = c->suivant;
  }
}

void Flechette_desallouer(Liste_Flechette** Lflech){
    Flechette *tmp;

    while((*Lflech)->tete != NULL){

        tmp = (*Lflech)->tete;

        (*Lflech)->tete = (*Lflech)->tete->suivant;

        free(tmp);
    }

    free(*Lflech);

    *Lflech = NULL;

}

void Interaction_Flechette_Canard(Liste_Flechette *Lflech, Liste_Canard *Lcanard) {
    if (Lflech == NULL || Lcanard == NULL) return;

    Flechette *flechette = Lflech->tete;
    Flechette *prec_f = NULL;

    while (flechette != NULL) {
        int touche = 0;

        Canard *canard = Lcanard->tete;
        Canard *prec_c = NULL;

        while (canard != NULL && !touche) {
            int dans_lignes   = (flechette->x >= canard->y &&
                                 flechette->x <  canard->y + Lcanard->hauteur);
            int dans_colonnes = (flechette->y >= canard->x &&
                                 flechette->y <  canard->x + Lcanard->largeur);

            if (dans_lignes && dans_colonnes) {
                touche = 1;
                canard->pv -= 2;   // ← dégâts

                if (canard->pv <= 0) {
                    if (prec_c == NULL) Lcanard->tete = canard->suivant;
                    else prec_c->suivant = canard->suivant;
                    free(canard);
                    Lcanard->nb_canards--;
                    Lcanard->nb_tues++;
                } else {
                    prec_c = canard;
                }
            } else {
                prec_c = canard;
                canard = canard->suivant;
            }
        }

        // Supprimer la fléchette dans tous les cas si elle a touché
        if (touche) {
            if (prec_f == NULL) Lflech->tete = flechette->suivant;
            else prec_f->suivant = flechette->suivant;
            Flechette *sup = flechette;
            flechette = flechette->suivant;
            free(sup);
            Lflech->nb_flechettes--;
        } else {
            prec_f = flechette;
            flechette = flechette->suivant;
        }
    }
}
