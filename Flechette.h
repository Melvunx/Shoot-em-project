#ifndef FLECHETTE_H
#define FLECHETTE_H

#include "Affichage.h"
#include "Canard.h"

typedef struct Flechette {
    int x;
    int y;
    struct Flechette* suivant;
} Flechette;

typedef struct {
    Flechette* tete;
    int y_depart;
    int y_max;
    int nb_flechettes;
    int nb_max;
    char apparence[5];
} Liste_Flechette;

Liste_Flechette* Liste_Flechette_initialiser_vide(int hauteur_aff, int nivdiff);
void Flechette_action(Liste_Flechette* LF);
void Flechette_afficher(Liste_Flechette* LF, Affichage* A);
void ajouter_flechette(Liste_Flechette* LF, int y);
void Flechette_desallouer(Liste_Flechette** LF);

void Interaction_Flechette_Canard(Liste_Flechette *LF, Liste_Canard *LC);

#endif
