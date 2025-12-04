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

AVL* rotationGauche(AVL* pAVL) {
    //rotation
    AVL* pivot = pAVL->pDroit;
    pAVL->pDroit = pivot->pGauche;
    pivot->pGauche = pAVL;

    //equilibrage
    int eq_root = pAVL->equilibre;
    int eq_pivot = pivot->equilibre;
    pAVL->equilibre = eq_root - max(eq_pivot, 0) - 1;
    pivot->equilibre = min(min(eq_root-2, eq_root+eq_pivot-2), eq_pivot-1);

    pAVL = pivot;
    return pAVL;
}

AVL* doubleRotationDroite(AVL* pAVL) {
    pAVL->pGauche = rotationGauche(pAVL->pGauche);
    return rotationDroite(pAVL);
}

AVL* doubleRotationGache(AVL* pAVL) {
    pAVL->pDroit = rightRotation(pAVL->pDroit);
    return rotationGauche(pAVL);
}

AVL* equilibrerAVL(AVL* pAVL) {
    if (pAVL->equilibre >= 2) {
        if (pAVL->pDroit->equilibre >= 0) {
            return rotationGauche(pAVL);
        }
        else {
            return doubleRotationGache(pAVL);
        }
    }
    else if (pAVL->equilibre <= -2) {
        if (pAVL->pGauche->equilibre <= 0) {
            return rotationDroite(pAVL);
        }
        else {
            return doubleRotationDroite(pAVL);
        }
    }
    return pAVL;
}



