#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Tourelle.h"
#include "Canon.h"
#include "Flechette.h"

#define MARGE 4
#define RATE_OF_FIRE 37

Liste_Tourelle* Liste_Tourelle_initialiser_vide(int largeur_aff, int hauteur_aff, int nivdiff) {
    Liste_Tourelle *Lt = malloc(sizeof(Liste_Tourelle));
    if (Lt == NULL) return NULL;

    Lt->tete = NULL;
    Lt->nb_tourelles = 0;
    Lt->nb_max = nivdiff;       // nb max = niveau de difficulté
    Lt->largeur_aff = largeur_aff;
    Lt->hauteur_aff = hauteur_aff;
    Lt->cooldown_max = RATE_OF_FIRE - nivdiff;  // plus difficile = tir plus rapide
    if (Lt->cooldown_max < 2) Lt->cooldown_max = 2;

    return Lt;
}

Liste_Laser* Liste_Laser_initialiser_vide() {
    Liste_Laser *Ll = malloc(sizeof(Liste_Laser));
    if (Ll == NULL) return NULL;

    Ll->tete = NULL;
    Ll->nb_lasers = 0;
    strcpy(Ll->apparence, "\33[31m!");  // rouge pour le laser

    return Ll;
}

void Tourelle_desallouer(Liste_Tourelle **Lt) {
    if (Lt == NULL || *Lt == NULL) return;
    Tourelle *cour = (*Lt)->tete;
    while (cour != NULL) {
        Tourelle *suiv = cour->suivant;
        free(cour);
        cour = suiv;
    }
    free(*Lt);
    *Lt = NULL;
}

void Laser_desallouer(Liste_Laser **Ll) {
    if (Ll == NULL || *Ll == NULL) return;
    Laser *cour = (*Ll)->tete;
    while (cour != NULL) {
        Laser *suiv = cour->suivant;
        free(cour);
        cour = suiv;
    }
    free(*Ll);
    *Ll = NULL;
}

void ajouter_tourelle(Liste_Tourelle *Lt, int canon_y) {
    if (Lt->nb_tourelles >= Lt->nb_max) return;

    Tourelle *t = malloc(sizeof(Tourelle));
    if (t == NULL) return;

    int tentatives = 0;
    do {
        t->x = 1 + rand() % (Lt->largeur_aff - MARGE);
        t->y = 1 + rand() % (Lt->hauteur_aff - MARGE);
        tentatives++;
    } while (t->y >= canon_y - 2 && tentatives < 20);
    // On réessaie tant que la tourelle est trop proche du canon

    if (tentatives >= 20) {   // aucune position valide trouvée
        free(t);
        return;
    }

    t->pv = 10;
    t->cooldown = Lt->cooldown_max;
    t->suivant = Lt->tete;
    Lt->tete = t;
    Lt->nb_tourelles++;
}

void Tourelle_afficher(Liste_Tourelle *Lt, Affichage *A) {
    if (Lt == NULL) return;

    Tourelle *t = Lt->tete;
    while (t != NULL) {
        int x = t->x;
        int y = t->y;

        // Canon (ligne du haut)
        if (y-2 >= 0 && x+1 >= 0 && x+1 < A->L)
            strcpy(A->tab[y-2][x+1], "\33[35m|");

        // Toit
        if (y-1 >= 0) {
            if (x >= 0 && x < A->L) strcpy(A->tab[y-1][x],   "\33[35m/");
            if (x+1 >= 0 && x+1 < A->L) strcpy(A->tab[y-1][x+1], "\33[35mT");
            if (x+2 >= 0 && x+2 < A->L) strcpy(A->tab[y-1][x+2], "\33[35m\\");
        }

        // Base
        if (y >= 0) {
            if (x >= 0 && x < A->L) strcpy(A->tab[y][x],   "\33[35m[");
            if (x+1 >= 0 && x+1 < A->L) strcpy(A->tab[y][x+1], "\33[35m#");
            if (x+2 >= 0 && x+2 < A->L) strcpy(A->tab[y][x+2], "\33[35m]");
        }

        // PV au-dessus
        char pv_str[16];
        snprintf(pv_str, sizeof(pv_str), "\33[37m%d", t->pv);
        if (y-3 >= 0 && x >= 0 && x < A->L)
            afficher_barre_vie(A, y - 3, x, t->pv, 5, "\33[31m+");

        t = t->suivant;
    }
}

void Laser_afficher(Liste_Laser *Ll, Affichage *A) {
    if (Ll == NULL) return;

    Laser *l = Ll->tete;
    while (l != NULL) {
        if (l->y >= 0 && l->y < A->H && l->x >= 0 && l->x < A->L)
            strcpy(A->tab[l->y][l->x], Ll->apparence);
        l = l->suivant;
    }
}

/* Chaque tourelle vise le canon et tire un laser vers lui */
void Tourelle_action(Liste_Tourelle *Lt, Liste_Laser *Ll, int canon_x, int canon_y) {
    if (Lt == NULL || Ll == NULL) return;

    Tourelle *t = Lt->tete;
    while (t != NULL) {
        t->cooldown--;

        if (t->cooldown <= 0) {
            t->cooldown = Lt->cooldown_max;

            // Crée un laser qui part de la tourelle vers le canon
            Laser *l = malloc(sizeof(Laser));
            if (l == NULL) { t = t->suivant; continue; }

            l->x = t->x;
            l->y = t->y;

            // Direction : on descend d'une ligne par frame (vers le canon en bas)
            l->suivant = Ll->tete;
            Ll->tete   = l;
            Ll->nb_lasers++;
        }
        t = t->suivant;
    }
}

/* Les lasers descendent vers le canon */
void Laser_action(Liste_Laser *Ll) {
    if (Ll == NULL) return;

    Laser *l = Ll->tete;
    Laser *prec = NULL;

    while (l != NULL) {
        l->y++;   // descend d'une ligne

        // Sorti par le bas
        if (l->y >= 200) {
            Laser *sup = l;
            if (prec == NULL) Ll->tete = l->suivant;
            else  prec->suivant = l->suivant;
            l = l->suivant;
            free(sup);
            Ll->nb_lasers--;
        } else {
            prec = l;
            l = l->suivant;
        }
    }
}

/* Collision laser / canon */
void Interaction_Laser_Canon(Liste_Laser *Ll, Canon *C) {
    if (Ll == NULL || C == NULL) return;

    Laser *l = Ll->tete;
    Laser *prec = NULL;

    while (l != NULL) {
        int touche = (l->y >= C->xpos && l->y < C->xpos + C->hauteur &&
                      l->x >= C->ypos && l->x < C->ypos + C->largeur);

        if (touche) {
            C->pv -= 1;   // ← dégât sur le canon

            Laser *sup = l;
            if (prec == NULL) Ll->tete     = l->suivant;
            else              prec->suivant = l->suivant;
            l = l->suivant;
            free(sup);
            Ll->nb_lasers--;
        } else {
            prec = l;
            l = l->suivant;
        }
    }
}

/* Collision fléchette / tourelle */
void Interaction_Flechette_Tourelle(Liste_Flechette *Lf, Liste_Tourelle *Lt) {
    if (Lf == NULL || Lt == NULL) return;

    Flechette *f = Lf->tete;
    Flechette *prec_f = NULL;

    while (f != NULL) {
        int touche = 0;

        Tourelle *t    = Lt->tete;
        Tourelle *prec_t = NULL;

        while (t != NULL && !touche) {
            int dans_lignes   = (f->x == t->y || f->x == t->y - 1 || f->x == t->y - 2);
            int dans_colonnes = (f->y >= t->x && f->y <= t->x + 2);

            if (dans_lignes && dans_colonnes) {
                touche = 1;
                t->pv -= 2;

                if (t->pv <= 0) {
                    if (prec_t == NULL) Lt->tete = t->suivant;
                    else  prec_t->suivant = t->suivant;
                    free(t);
                    Lt->nb_tourelles--;
                } else {
                    prec_t = t;
                }
            } else {
                prec_t = t;
                t = t->suivant;
            }
        }

        if (touche) {
            if (prec_f == NULL) Lf->tete = f->suivant;
            else prec_f->suivant = f->suivant;
            Flechette *sup = f;
            f = f->suivant;
            free(sup);
            Lf->nb_flechettes--;
        } else {
            prec_f = f;
            f = f->suivant;
        }
    }
}
