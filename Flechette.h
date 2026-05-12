#ifndef FLECHETTE_H
#define FLECHETTE_H
#include "Affichage.h"

// ===== Struct d'une fléchette (maillon) =====
typedef struct Flechette {
    int x;                  // coordonnée verticale (ligne)
    int y;                  // coordonnée horizontale (colonne)
    struct Flechette* suivant;
} Flechette;

// ===== Struct liste de fléchettes =====
typedef struct {
    // liste chaînée
    Flechette* tete;

    // informations sur l'affichage
    int largeur;
    int hauteur;
    char apparence[5];

    // limites verticales
    int y_depart;           // position de départ (bas écran)
    int y_max;              // limite haute

    // gestion quantité
    int nb_flechettes;
    int nb_max;

} Liste_Flechette;

Liste_Flechette* Liste_Flechette_initialiser_vide(int hauteur_aff, int nivdiff);
void Flechette_desallouer(Liste_Flechette** Lflech);
void ajouter_flechette(Liste_Flechette* Lflech, int y);
void Flechette_action(Liste_Flechette* Lflech);
void Flechette_afficher(Liste_Flechette* Lflech, Affichage* A);


#endif
