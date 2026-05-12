#ifndef FLECHETTE_H
#define FLECHETTE_H

// ===== Struct d'une fléchette (maillon) =====
typedef struct Mail_Flechette {
    int x;                  // coordonnée verticale (ligne)
    int y;                  // coordonnée horizontale (colonne)
    struct Mail_Flechette* suivant;
} Flechette;

// ===== Struct liste de fléchettes =====
typedef struct {
    // liste chaînée
    Flechette* tete;

    // informations sur l'affichage
    int largeur, hauteur;
    char apparence[5];      // ASCII / ANSI / emoji

    // limites verticales
    int y_depart;           // position de départ (bas écran)
    int y_max;              // limite haute

    // gestion quantité (optionnel mais recommandé)
    int nb_flechettes;
    int nb_max;

} Liste_Flechette;

Liste_Flechette* Liste_Flechette_initialiser_vide(int hauteur_aff, int nivdiff);

void Flechette_desallouer(Liste_Flechette** Lflech);

#endif