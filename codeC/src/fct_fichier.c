//objectif de cette partie : ouvrir fichier avec les sources et les usines
//lire les données et modifier les l'avl usine

#include "../include/include.h"

//fonction d'ouverture de fichier
FILE* ouvrirFichier(char* nom_fichier){
    FILE* fichier = fopen(nom_fichier, "r");
    if(fichier == NULL){
        exit(50);
    }
    return fichier;
}

//fonction de fermeture de fichier
void fermerFichier(FILE* fichier){
    if(fclose(fichier) != 0){
        exit(51);
    }
}

//fonction de lecture des données du fichier source


