#ifndef FONCTION_H
#define FONCTION_H
//utilitaires
int max(int a, int b);
int min(int a, int b);

void echangerUsines(AVL_USINE** a, AVL_USINE** b);
int partitionUsines(AVL_USINE** tab, int bas, int haut);
void triRapideUsinesDec(AVL_USINE** tab, int bas, int haut);


#endif // FONCTION_H

