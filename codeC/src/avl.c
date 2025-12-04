#include "../include/include.h"

AVL* rotationDroite(AVL* pAVL) {
    //rotation
    AVL* pivot = pAVL->pGauche;
    pAVL->pGauche = pivot->pDroit;
    pivot->pDroit = pAVL;

    //equilibrage
    int eq_root = pAVL->equilibre;
    int eq_pivot = pivot->equilibre;
    pAVL->equilibre = eq_root - min(eq_pivot, 0) + 1;
    pivot->equilibre = max(max(eq_root+2, eq_root+eq_pivot+2), eq_pivot+1);

    pAVL = pivot;
    return pAVL;
}
