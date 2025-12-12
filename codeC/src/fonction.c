#include "include.h"

int max(int a, int b) {
    if (a >= b) {
        return a;
    }
    return b;
}

int min(int a, int b) {
    if (a <= b) {
        return a;
    }
    return b;
}

void echangerUsines(AVL_USINE** a, AVL_USINE** b) {
    AVL_USINE* temp = *a;
    *a = *b;
    *b = temp;
}

int partitionUsines(AVL_USINE** tab, int bas, int haut) {
    //dernier element pour pivot
    char* pivot = tab[haut]->val;
    int i = bas - 1;

    for (int j = bas; j < haut; j++) {
        //ordre decorissant
        if (strcmp(tab[j]->val, pivot) > 0) {
            i++;
            echangerUsines(&tab[i], &tab[j]);
        }
    }

    echangerUsines(&tab[i + 1], &tab[haut]);
    return i + 1;
}

void triRapideUsinesDec(AVL_USINE** tab, int bas, int haut) {
    if (bas < haut) {
        int pivot = partitionUsines(tab, bas, haut);
        triRapideUsinesDec(tab, bas, pivot - 1);
        triRapideUsinesDec(tab, pivot + 1, haut);
    }
}