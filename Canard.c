#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Canard.h"

Canard* initialiser_canard(int coord, enum Direction d)
{
  Canard* c = malloc(sizeof(Canard));
  c->x = coord;
  c->direction = d;
  c->suivant = NULL;

  return c;
} 

Liste_Canard* Liste_Canard_initialiser_vide(int largeur_aff, int hauteur_aff,int nivdiff, char * nomfic)
{
  Liste_Canard* Lcanard = malloc(sizeof(Liste_Canard));

  Lcanard->tete = NULL;
  Lcanard->hauteur = hauteur_aff;
  Lcanard->largeur = largeur_aff;
  Lcanard->nb_max_canards = nivdiff;

  return Lcanard;
}

void Canard_desallouer(Liste_Canard** Lcanard)
{
  
}