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
    Canard *tete;             // tête de la liste chaînée
    int largeur, hauteur;     // hauteur et largeur du canard (taille)
    char apparence[5];
    int coord_apparition ,coord_disparition;    // coordonnée d'apparition et de disparition
    int pas;                  // pas de déplacement
    int nb_canards, nb_max_canards;           // nombre de canards dans la liste
} Liste_Canard;

Canard* initialiser_canard(int largeur_aff, int hauteur_aff);

Liste_Canard* Liste_Canard_initialiser_vide(int largeur_aff, int hauteur_aff,int nivdiff, char * nomfic);

void Canard_desallouer(Liste_Canard** Lcanard);

void ajouter_canard(Liste_Canard* Lcanard);

void Canard_afficher(Liste_Canard *Lcanard, Affichage *A);

void Canard_action(Liste_Canard *Lcanard);

#endif
