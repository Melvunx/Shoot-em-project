#ifndef TOURELLE_H
#define TOURELLE_H
#include "Affichage.h"
#include "Canon.h"
#include "Flechette.h"

typedef struct Mail_Tourelle {
    int x;                        // colonne
    int y;                        // ligne
    int pv;                       // points de vie
    int cooldown;                 // compteur avant prochain tir
    struct Mail_Tourelle *suivant;
} Tourelle;

typedef struct {
    Tourelle *tete;
    int nb_tourelles;
    int nb_max;
    int largeur_aff;
    int hauteur_aff;
    int cooldown_max;             // intervalle de tir
} Liste_Tourelle;

typedef struct Mail_Laser {
    int x;                        // colonne
    int y;                        // ligne
    struct Mail_Laser *suivant;
} Laser;

typedef struct {
    Laser *tete;
    int nb_lasers;
    char apparence[8];
} Liste_Laser;

Liste_Tourelle* Liste_Tourelle_initialiser_vide(int largeur_aff, int hauteur_aff, int nivdiff);
Liste_Laser*    Liste_Laser_initialiser_vide();
void Tourelle_desallouer(Liste_Tourelle **Lt);
void Laser_desallouer(Liste_Laser **Ll);
void ajouter_tourelle(Liste_Tourelle *Lt, int canon_y);
void Tourelle_afficher(Liste_Tourelle *Lt, Affichage *A);
void Laser_afficher(Liste_Laser *Ll, Affichage *A);
void Tourelle_action(Liste_Tourelle *Lt, Liste_Laser *Ll, int canon_x, int canon_y);
void Laser_action(Liste_Laser *Ll);
void Interaction_Laser_Canon(Liste_Laser *Ll, Canon *C);
void Interaction_Flechette_Tourelle(Liste_Flechette *Lf, Liste_Tourelle *Lt);

#endif
