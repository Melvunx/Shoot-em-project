#ifndef _CANON_H_
#define _CANON_H_

#include"Affichage.h"

enum action_canon {Gauche_canon, Droite_canon, Tir};

typedef struct{
  int largeur, hauteur;
  int pos_min, pos_max;
  int xpos, ypos;
  int pas;
  int ypos_old;
  int pv;
} Canon;

Canon* Canon_initialiser(int pos_min, int pos_max, int hauteur_aff);
void Canon_desallouer(Canon** C);
void Canon_action(Canon *C, enum action_canon actC);
void Canon_afficher(Canon *C, Affichage* A);


#endif 

