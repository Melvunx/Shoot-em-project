#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Canard.h"

Canard* initialiser_canard(int hauteur)
{
  Canard* c = malloc(sizeof(Canard));
  c->x = rand() % hauteur;
  c->direction = rand() % 2; // 0 pour Droite et 1 pour Gauche
  c->suivant = NULL;

  return c;
} 

Liste_Canard* Liste_Canard_initialiser_vide(int largeur_aff, int hauteur_aff,int nivdiff, char * nomfic)
{
  Liste_Canard* Lcanard = malloc(sizeof(Liste_Canard));
  if (Lcanard == NULL) return NULL;
  

  Lcanard->tete = NULL;
  Lcanard->hauteur = hauteur_aff;
  Lcanard->largeur = largeur_aff;
  Lcanard->nb_max_canards = nivdiff;
  Lcanard->nb_canards = 0;
  Lcanard->pas = 1;

  return Lcanard;
}

void Canard_desallouer(Liste_Canard** Lcanard)
{
  if (Lcanard == NULL || *Lcanard == NULL) return;

  Canard *cour = (*Lcanard)->tete;
  while (cour != NULL)
  {
    Canard *suiv = cour->suivant;
    free(cour);
    cour = suiv;
  }
  
  free(*Lcanard);
  *Lcanard = NULL;
}

void ajouter_canard(Liste_Canard* Lcanard)
{
  if (Lcanard->nb_max_canards == Lcanard->nb_canards) return;
  
  Canard* nouv = initialiser_canard(Lcanard->hauteur);
  nouv->suivant = Lcanard->tete;
  Lcanard->tete = nouv;

  Lcanard->nb_canards++;
}

void Canard_action(Liste_Canard *Lcanard) {
  if (Lcanard == NULL || Lcanard->tete == NULL) return;

  Canard *courant = Lcanard->tete;
  Canard *precedent = NULL;

  while (courant != NULL) {
  
    // Faire avancer le canard d'un pas selon sa direction
      if (courant->direction == DROITE) courant->x += Lcanard->pas;
      else courant->x -= Lcanard->pas;
      
      // Vérifier si le canard a atteint un bord
      if (courant->x >= Lcanard->coord_disparition || courant->x <= Lcanard->coord_apparition) 
      {
        // Supprimer le maillon
        Canard *a_supprimer = courant;
        if (precedent == NULL)          // c'était la tête
            Lcanard->tete = courant->suivant;
        else            // c'était un maillon du milieu/fin
            precedent->suivant = courant->suivant;

        courant = courant->suivant;
        free(a_supprimer);
        Lcanard->nb_max_canards--;      // mettre à jour le compteur
      } else {
          precedent = courant;
          courant = courant->suivant;
      }
  }
}