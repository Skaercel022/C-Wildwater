#include "../include/include.h"



AVL_Fuite* constructeurAVL(char* id){
    return NULL;
}

Liste *creerListe(void* ptr, char* id) {
    Liste* pListe = malloc(sizeof(Liste));
    if (pListe == NULL) {
        exit(1);
    }
    pListe->ptr = ptr;
    pListe->next = NULL;
    pListe->id = id;
    return pListe;
}

Arbre_liste* CreerArbre_liste(void* ptr, char* id) {
    // Crée un Arbre_liste avec un seul élément ptr
    Arbre_liste* arbre = malloc(sizeof(Arbre_liste));
    if (arbre == NULL) {
        exit(1);
    }
    arbre->liste = creerListe(ptr,id);
    arbre->nb_fuite = 0;
    return arbre;
}

Arbre_liste* insererArbre_liste(Arbre_liste* arbre, void* ptr, char* id) {
    //  Inserer ptr à la fin de la liste
    Liste* nouvelleListe = creerListe(ptr, id);
    if (arbre->liste == NULL) {
        arbre->liste = nouvelleListe;
    }
    else {
        Liste* current = arbre->liste;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = nouvelleListe;
    }
    return arbre;
}

Arbre_liste* creationAbre_liste(Arbre_liste* arbre, void* ptr, char* id) {
    // Ajoute ptr à la liste si elle n'y est pas déjà
    if (arbre->liste == NULL) {
        arbre->liste = creerListe(ptr, id);
        return arbre;
    }
    Liste* current = arbre->liste;
    while (current != NULL) {
        if (current->ptr == ptr) {
            return arbre;
        }
        if (current->next == NULL) {
            current->next = creerListe(ptr, id);
            return arbre;
        }
        current = current->next;
    }
    return arbre;
}

AVL_Fuite* InsertionAVL(AVL_Fuite* racine, char* id){
    return NULL;
}

Arbre_liste* recherche(AVL_Fuite* racine, char* id){
    if (racine == NULL){
        return NULL;
    }
    if (strcmp(id, racine->id) == 0){
        return racine->parents;
    }
    else if (strcmp(id, racine->id) < 0){
        return recherche(racine->pGauche, id);
    }
    else if(strcmp(id, racine->id) > 0){
        return recherche(racine->pDroit, id);
    }
    else{
        return NULL;
    }
}