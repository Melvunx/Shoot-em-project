#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Flechette.h"


Liste_Flechette* Liste_Flechette_initialiser_vide(int hauteur_aff, int nivdiff){

    Liste_Flechette *F;

    F = malloc(sizeof(Liste_Flechette));

    // liste vide
    F->tete = NULL;

    // taille fléchette
    F->largeur = 1;
    F->hauteur = 1;

    // apparence
    strcpy(F->apparence, "\33[31m||");

    // départ bas écran
    F->y_depart = hauteur_aff - 2;

    // limite haute
    F->y_max = 1;

    // gestion quantité
    F->nb_flechettes = 0;
    F->nb_max = nivdiff;

    return F;
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
void ajouter_flechette(Liste_Flechette* Lflech, int y){
    Flechette* nouveauF;

    //vérification du nombre maximum
    if (Lflech->nb_flechettes >= Lflech-> nb_max){
        printf("Limite atteinte\n");
        return;
    }

    //allocation mémoire
    nouveauF = malloc(sizeof(Flechette));

    //position de départ
    nouveauF->x = Lflech->y_depart;
    nouveauF->y =y;

    //insertion en têtede liste
    nouveauF->suivant = Lflech->tete;

    Lflech->tete = nouveauF;

    //mise à jour compteur
    Lflech->nb_flechettes++;

}
void Flechette_afficher(Liste_Flechette *Lflech, Affichage* A){

    if (Lflech == NULL) return;

    Flechette *tmp = Lflech->tete;

    while(tmp != NULL){

        if(tmp->x >= 0 && tmp->x < A->H &&
           tmp->y >= 0 && tmp->y < A->L){

            strcpy(A->tab[tmp->x][tmp->y], Lflech->apparence);
        }

        tmp = tmp->suivant;
    }
}

void Flechette_action(Liste_Flechette *Lflech){

    if (Lflech == NULL) return;

    Flechette *tmp = Lflech->tete;
    Flechette *prec = NULL;

    while(tmp != NULL){

        tmp->x--; // montée

        // sortie écran
        if(tmp->x <= Lflech->y_max){

            Flechette *sup = tmp;

            if(prec == NULL){
                Lflech->tete = tmp->suivant;
                tmp = Lflech->tete;
            }
            else{
                prec->suivant = tmp->suivant;
                tmp = tmp->suivant;
            }

            free(sup);
            Lflech->nb_flechettes--;
        }
        else{
            prec = tmp;
            tmp = tmp->suivant;
        }
    }
}
