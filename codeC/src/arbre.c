#include "../include/include.h"

Liste_arbre* creerListeArbre(char* id){
    Liste_arbre* liste = malloc(sizeof(Liste_arbre));
    if(liste == NULL){
        exit(1);
    }

    liste->id = malloc(strlen(id) + 1);
    if(liste->id == NULL){
        free(liste);
        exit(1);
    }
    strcpy(liste->id, id);

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

void suppListeArbre(Liste_arbre* liste){
    Liste_arbre* tmp;

    while(liste != NULL){
        tmp = liste;
        liste = liste->pSuivant;

        free(tmp->id);   // libère la chaîne copiée
        free(tmp);
    }
}

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

void suppNoeud(Arbre* arbre){
    if(arbre != NULL){
        suppListeArbre(arbre->enfant);
        free(arbre);
    }
}