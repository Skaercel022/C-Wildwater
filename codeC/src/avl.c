#include "../include/include.h"

AVL* creerAVL(char* carac) {
    AVL* pAVL = malloc(sizeof(AVL));
    if (pAVL == NULL) {
        exit(1);
    }
    pAVL->val = malloc(sizeof(strlen(carac)+1));
    pAVL->val = carac;
    pAVL->pGauche = NULL;
    pAVL->pDroit = NULL;
    pAVL->equilibre = 0;
    return pAVL;
}

int existeGauche(AVL* pAVL) {
    if (pAVL == NULL) {
        exit(2);
    }
    if (pAVL->pGauche != NULL) {
        return 1;
    }
    return 0;
}

int existDroit(AVL* pAVL) {
    if (pAVL == NULL) {
        exit(2);
    }
    if (pAVL->pDroit != NULL) {
        return 1;
    }
    return 0;
}

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
    pAVL->pDroit = rotationDroite(pAVL->pDroit);
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

AVL* insertionAVL(AVL* pAVL, char* id, int* h) {
    if (pAVL == NULL) {
        *h = 1;
        return creerAVL(id);
    }
    else if (strcmp(id, pAVL->val) < 0) {
        pAVL->pGauche = insertionAVL(pAVL->pGauche, id, h);
        *h = - *h;
    }
    else if (strcmp(id, pAVL->val) > 0) {
        pAVL->pDroit = insertionAVL(pAVL->pDroit, id, h);
    }
    else {
        *h = 0;
        return pAVL;
    }

    if (*h != 0) {
        pAVL->equilibre += *h;
        pAVL = equilibrerAVL(pAVL);
        if (pAVL->equilibre == 0) {
            *h = 0;
        }
        else {
            *h = 1;
        }
    }
    return pAVL;
}

AVL* suppMinAVL(AVL* pAVL, char* pe, int *h) {
    AVL* tmp = NULL;
    if (pAVL->pGauche == NULL) {
        *pe = pAVL->val;
        *h = 1;
        tmp = pAVL;
        pAVL = pAVL->pDroit;
        free(tmp);
        return pAVL;
    }
    else {
        pAVL->pGauche = suppMinAVL(pAVL->pGauche, pe , h);
        *h = - *h;
    }
    if (*h != 0) {
        pAVL->equilibre += *h;
        pAVL = equilibrerAVL(pAVL);
        if (pAVL->equilibre == 0) {
            *h = 0;
        }
        else {
            *h = 1;
        }
    }
    return pAVL;
}

AVL* suppressionAVL(AVL* pAVL, char* id, int* h) {
    AVL* tmp = NULL;
    if (pAVL == NULL) {
        *h = 0;
        return pAVL;
    }
    // Recherche de l'élément à supprimer
    else if (strcmp(pAVL->val, id) < 0) {
        pAVL->pDroit = suppressionAVL(pAVL->pDroit, id, h);
    }
    else if (strcmp(pAVL->val, id) > 0) {
        pAVL->pGauche = suppressionAVL(pAVL->pGauche, id, h);
        *h = - *h;
    }
    else if (existDroit(pAVL)) {
        pAVL->pDroit = suppMinAVL(pAVL->pDroit, h, &id);
    }
    else {
        tmp = pAVL;
        pAVL = pAVL->pGauche;
        free(tmp);
        *h = -1;
        return pAVL;
    }
    // Equilibrage
    if (*h != 0) {
        pAVL->equilibre += *h;
        pAVL = equilibrerAVL(pAVL);
        if (pAVL->equilibre == 0) {
            *h = -1;
        }
        else {
            *h = 0;
        }
    }
    return pAVL;
}

Arbre* rechercheAVL(AVL* pAVL, char* id) {
    if (pAVL == NULL) {
        return NULL;
    }
    else if (strcmp(id, pAVL->val) < 0) {
        return rechercheAVL(pAVL->pGauche, id);
    }
    else if (strcmp(id, pAVL->val) > 0) {
        return rechercheAVL(pAVL->pDroit, id);
    }
    else {
        return pAVL->arbre;
    }
}
