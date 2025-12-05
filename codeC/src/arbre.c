#include "../include/include.h"

Arbre* creerArbre(double volume, float fuite){
    Arbre* arbre = malloc(sizeof(Arbre));
    if(arbre == NULL){
        exit(1);
    }

    arbre->V_amont = volume;
    arbre->fuite_amont = fuite;
    arbre->enfant = NULL;
    return arbre;
}

