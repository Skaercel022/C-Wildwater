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

Liste_arbre* creerListeArbre(char* id){
    Liste_arbre* liste = malloc(sizeof(Liste_arbre));
    if(liste == NULL){
        exit(1);
    }

    liste->id = id;
    liste->pSuivant = NULL;
    return liste;
}

void ajouterEnfant(Arbre* arbre, char* id_enfant){
    if(arbre == NULL){
        exit(2);
    }

    Liste_arbre* nouvel_enfant = creerListeArbre(id_enfant);
    if(arbre->enfant == NULL){
        arbre->enfant = nouvel_enfant;
    } else {
        Liste_arbre* temp = arbre->enfant;
        while(temp->pSuivant != NULL){
            temp = temp->pSuivant;
        }
        temp->pSuivant = nouvel_enfant;
    }
}

void libererListeArbre(Liste_arbre* liste){
    Liste_arbre* p1 = liste;
    Liste_arbre* next;

    while(p1 != NULL){
        next = p1->pSuivant;
        free(p1);
        p1 = next;
    }
}

void libererArbre(Arbre* arbre){
    if(arbre != NULL){
        libererListeArbre(arbre->enfant);
        free(arbre);
    }
}
