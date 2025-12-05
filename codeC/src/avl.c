#include "../include/include.h"

AVL_USINE* creerAVL_USINE(char* carac) {
    AVL_USINE* pAVL_USINE = malloc(sizeof(AVL_USINE));
    if (pAVL_USINE == NULL) {
        exit(1);
    }
    pAVL_USINE->val = malloc(sizeof(strlen(carac)+1));
    pAVL_USINE->val = carac;
    pAVL_USINE->pGauche = NULL;
    pAVL_USINE->pDroit = NULL;
    pAVL_USINE->equilibre = 0;
    return pAVL_USINE;
}

int existeGauche(AVL_USINE* pAVL_USINE) {
    if (pAVL_USINE == NULL) {
        exit(2);
    }
    if (pAVL_USINE->pGauche != NULL) {
        return 1;
    }
    return 0;
}

int existDroit(AVL_USINE* pAVL_USINE) {
    if (pAVL_USINE == NULL) {
        exit(2);
    }
    if (pAVL_USINE->pDroit != NULL) {
        return 1;
    }
    return 0;
}

AVL_USINE* rotationDroite(AVL_USINE* pAVL_USINE) {
    //rotation
    AVL_USINE* pivot = pAVL_USINE->pGauche;
    pAVL_USINE->pGauche = pivot->pDroit;
    pivot->pDroit = pAVL_USINE;

    //equilibrage
    int eq_root = pAVL_USINE->equilibre;
    int eq_pivot = pivot->equilibre;
    pAVL_USINE->equilibre = eq_root - min(eq_pivot, 0) + 1;
    pivot->equilibre = max(max(eq_root+2, eq_root+eq_pivot+2), eq_pivot+1);

    pAVL_USINE = pivot;
    return pAVL_USINE;
}

AVL_USINE* rotationGauche(AVL_USINE* pAVL_USINE) {
    //rotation
    AVL_USINE* pivot = pAVL_USINE->pDroit;
    pAVL_USINE->pDroit = pivot->pGauche;
    pivot->pGauche = pAVL_USINE;

    //equilibrage
    int eq_root = pAVL_USINE->equilibre;
    int eq_pivot = pivot->equilibre;
    pAVL_USINE->equilibre = eq_root - max(eq_pivot, 0) - 1;
    pivot->equilibre = min(min(eq_root-2, eq_root+eq_pivot-2), eq_pivot-1);

    pAVL_USINE = pivot;
    return pAVL_USINE;
}

AVL_USINE* doubleRotationDroite(AVL_USINE* pAVL_USINE) {
    pAVL_USINE->pGauche = rotationGauche(pAVL_USINE->pGauche);
    return rotationDroite(pAVL_USINE);
}

AVL_USINE* doubleRotationGache(AVL_USINE* pAVL_USINE) {
    pAVL_USINE->pDroit = rotationDroite(pAVL_USINE->pDroit);
    return rotationGauche(pAVL_USINE);
}

AVL_USINE* equilibrerAVL_USINE(AVL_USINE* pAVL_USINE) {
    if (pAVL_USINE->equilibre >= 2) {
        if (pAVL_USINE->pDroit->equilibre >= 0) {
            return rotationGauche(pAVL_USINE);
        }
        else {
            return doubleRotationGache(pAVL_USINE);
        }
    }
    else if (pAVL_USINE->equilibre <= -2) {
        if (pAVL_USINE->pGauche->equilibre <= 0) {
            return rotationDroite(pAVL_USINE);
        }
        else {
            return doubleRotationDroite(pAVL_USINE);
        }
    }
    return pAVL_USINE;
}

AVL_USINE* insertionAVL_USINE(AVL_USINE* pAVL_USINE, char* id, int* h) {
    if (pAVL_USINE == NULL) {
        *h = 1;
        return creerAVL_USINE(id);
    }
    else if (strcmp(id, pAVL_USINE->val) < 0) {
        pAVL_USINE->pGauche = insertionAVL_USINE(pAVL_USINE->pGauche, id, h);
        *h = - *h;
    }
    else if (strcmp(id, pAVL_USINE->val) > 0) {
        pAVL_USINE->pDroit = insertionAVL_USINE(pAVL_USINE->pDroit, id, h);
    }
    else {
        *h = 0;
        return pAVL_USINE;
    }

    if (*h != 0) {
        pAVL_USINE->equilibre += *h;
        pAVL_USINE = equilibrerAVL_USINE(pAVL_USINE);
        if (pAVL_USINE->equilibre == 0) {
            *h = 0;
        }
        else {
            *h = 1;
        }
    }
    return pAVL_USINE;
}

AVL_USINE* suppMinAVL_USINE(AVL_USINE* pAVL_USINE, char* pe, int *h) {
    AVL_USINE* tmp = NULL;
    if (pAVL_USINE->pGauche == NULL) {
        *pe = pAVL_USINE->val;
        *h = 1;
        tmp = pAVL_USINE;
        pAVL_USINE = pAVL_USINE->pDroit;
        free(tmp);
        return pAVL_USINE;
    }
    else {
        pAVL_USINE->pGauche = suppMinAVL_USINE(pAVL_USINE->pGauche, pe , h);
        *h = - *h;
    }
    if (*h != 0) {
        pAVL_USINE->equilibre += *h;
        pAVL_USINE = equilibrerAVL_USINE(pAVL_USINE);
        if (pAVL_USINE->equilibre == 0) {
            *h = 0;
        }
        else {
            *h = 1;
        }
    }
    return pAVL_USINE;
}

AVL_USINE* suppressionAVL_USINE(AVL_USINE* pAVL_USINE, char* id, int* h) {
    AVL_USINE* tmp = NULL;
    if (pAVL_USINE == NULL) {
        *h = 0;
        return pAVL_USINE;
    }
    // Recherche de l'élément à supprimer
    else if (strcmp(pAVL_USINE->val, id) < 0) {
        pAVL_USINE->pDroit = suppressionAVL_USINE(pAVL_USINE->pDroit, id, h);
    }
    else if (strcmp(pAVL_USINE->val, id) > 0) {
        pAVL_USINE->pGauche = suppressionAVL_USINE(pAVL_USINE->pGauche, id, h);
        *h = - *h;
    }
    else if (existDroit(pAVL_USINE)) {
        pAVL_USINE->pDroit = suppMinAVL_USINE(pAVL_USINE->pDroit, h, &id);
    }
    else {
        tmp = pAVL_USINE;
        pAVL_USINE = pAVL_USINE->pGauche;
        free(tmp);
        *h = -1;
        return pAVL_USINE;
    }
    // Equilibrage
    if (*h != 0) {
        pAVL_USINE->equilibre += *h;
        pAVL_USINE = equilibrerAVL_USINE(pAVL_USINE);
        if (pAVL_USINE->equilibre == 0) {
            *h = -1;
        }
        else {
            *h = 0;
        }
    }
    return pAVL_USINE;
}

Arbre* rechercheAVL_USINE(AVL_USINE* pAVL_USINE, char* id) {
    if (pAVL_USINE == NULL) {
        return NULL;
    }
    else if (strcmp(id, pAVL_USINE->val) < 0) {
        return rechercheAVL_USINE(pAVL_USINE->pGauche, id);
    }
    else if (strcmp(id, pAVL_USINE->val) > 0) {
        return rechercheAVL_USINE(pAVL_USINE->pDroit, id);
    }
    else {
        return pAVL_USINE->arbre;
    }
}
