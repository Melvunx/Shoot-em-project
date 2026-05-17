#ifndef _CANARD_H_
#define _CANARD_H_

#include "Affichage.h"

enum Direction {
  DROITE,
  GAUCHE
};

typedef struct Mail_Canard {
    int x;                       // coordonnée horizontale du coin haut-gauche
    int y;                       // coordonnée verctical
    enum Direction direction;    // direction du canard
    struct Mail_Canard *suivant; // maillon suivant
} Canard;

typedef struct {
    Canard *tete;
    int largeur, hauteur;
    int largeur_aff, hauteur_aff;   // ← dimensions réelles de l'écran
    int coord_apparition, coord_disparition;
    int pas;
    int nb_canards, nb_max_canards;
} Liste_Canard;

Canard* initialiser_canard(int largeur_aff, int hauteur_aff);

Liste_Canard* Liste_Canard_initialiser_vide(int largeur_aff, int hauteur_aff,int nivdiff);

void Canard_desallouer(Liste_Canard** Lcanard);

void ajouter_canard(Liste_Canard* Lcanard);

void Canard_afficher(Liste_Canard *Lcanard, Affichage *A);

void Canard_action(Liste_Canard *Lcanard);

#endif
