#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Flechette.h"

// Initialisation d'une liste vide
Liste_Flechette* Liste_Flechette_initialiser_vide(int hauteur_aff, int nivdiff) {

    Liste_Flechette* L = malloc(sizeof(Liste_Flechette));

    // liste vide
    L->tete = NULL;

    // dimensions fléchette (ex: 1 caractère)
    L->largeur = 1;
    L->hauteur = 1;

    // apparence
    strcpy(L->apparence, "🔺");

    // positions
    L->y_depart = hauteur_aff - 2;
    L->y_max = 1;

    // gestion du nombre
    L->nb_flechettes = 0;
    L->nb_max = nivdiff * 5; // ajustable

    return L;
}

void Flechette_desallouer(Liste_Flechette** Lflech) {

    if (Lflech == NULL || *Lflech == NULL) return;

    Flechette* courant = (*Lflech)->tete;
    Flechette* suivant;

    // Parcours de la liste
    while (courant != NULL) {
        suivant = courant->suivant; // sauver le suivant
        free(courant);              // libérer l'actuel
        courant = suivant;          // avancer
    }

    // Libérer la structure principale
    free(*Lflech);

    // Sécuriser le pointeur
    *Lflech = NULL;
}