#ifndef _CANARD_H_
#define _CANARD_H_

#include "Affichage.h"

enum Direction { DROITE, GAUCHE };

typedef struct Mail_Canard {
    int x;
    int y;
    int pv;
    enum Direction direction;
    struct Mail_Canard *suivant;
} Canard;

// Dans Liste_Canard (Canard.h)
typedef struct {
    Canard *tete;
    int largeur, hauteur;
    int largeur_aff, hauteur_aff;
    int coord_apparition, coord_disparition;
    int pas;
    int nb_canards; // nombre de canards dans l'affichage
    int nb_max_canards; // nombre de canards max selon le niveau de difficulté
    int nb_tues;        // ← nombre de canards tués
    int nb_total;       // ← nombre total à tuer pour gagner
} Liste_Canard;

Canard* initialiser_canard(int largeur_aff, int hauteur_aff);
Liste_Canard* Liste_Canard_initialiser_vide(int largeur_aff, int hauteur_aff,int nivdiff);
void Canard_desallouer(Liste_Canard** Lcanard);
void ajouter_canard(Liste_Canard* Lcanard);
void Canard_afficher(Liste_Canard *Lcanard, Affichage *A);
void Canard_action(Liste_Canard *Lcanard);


#endif
