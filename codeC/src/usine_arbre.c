#include "../include/include.h"



Abre_liste* creerArbre_liste() {
    Abre_liste* pArbre_liste = malloc(sizeof(Abre_liste));
    if (pArbre_liste == NULL) {
        exit(1);
    }
    pArbre_liste->liste = NULL;
    pArbre_liste->Suivant = NULL;
    pArbre_liste->nb_fuite = 0;
    return pArbre_liste;
}

Liste *creerListe(void* ptr) {
    Liste* pListe = malloc(sizeof(Liste));
    if (pListe == NULL) {
        exit(1);
    }
    pListe->ptr = ptr;
    pListe->next = NULL;
    return pListe;
}

Abre_liste* insererArbre_liste(Abre_liste* arbre, void* ptr) {
    Liste* nouvelleListe = creerListe(ptr);
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
}
