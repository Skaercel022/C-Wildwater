//objectif de cette partie : ouvrir fichier avec les sources et les usines
//lire les données et modifier les l'avl_usine usine

#include "../include/include.h"

int estVide(const char *s) {
    return (strcmp(s, "-") == 0);
}

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


//fonction utilitaire permettant de lire une ligne complète d'un fichier peu importe sa longueur

char* lireLigne(FILE* fichier) {
    //taille initiale du buffer (augmentera automatiquement si nécessaire)
    unsigned int capacite = 256;
    char* buffer = malloc(capacite);
    if (buffer == NULL) {
        return NULL;
    }

    unsigned int position = 0;
    int c;
    //lecture caractère par caractère jusqu'à la fin de la ligne ou du fichier : si buffer plein, on réalloue le double
    while ((c = fgetc(fichier)) != EOF) {
        if (position >= capacite - 1) {
            capacite *= 2;
            char* tmp = realloc(buffer, capacite);
            if (tmp == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = tmp;
        }
        if (c == '\n')
            break;

        buffer[position++] = c;
    }
    //si on arrive à EOF sans avoir lu un seul caractère
    if (position == 0 && c == EOF) {
        free(buffer);
        return NULL;
    }
    buffer[position] = '\0';
    return buffer;
}

//fonction de lecture du fichier et insertion dans l'avl_usine

void lectureFichierVersAVL(FILE* fichier, AVL_USINE** racine) {

    char* ligne;

    while ((ligne = lireLigne(fichier)) != NULL) {

        if (ligne[0] == '\0') {
            free(ligne);
            continue;
        }

        char* col1 = strtok(ligne, ";");
        char* col2 = strtok(NULL, ";");
        char* col3 = strtok(NULL, ";");
        char* col4 = strtok(NULL, ";");
        char* col5 = strtok(NULL, ";");
        
        //erreur ligne csv mal formée
        if (!col1 || !col2 || !col3 || !col4 || !col5) {
            free(ligne);
            exit(60);
        }

        //ligne "usine" -> on passe à la suivante
        if (estVide(col1) && estVide(col3) && !estVide(col4) && estVide(col5)) {
            free(ligne);
            continue;
        }

        //ligne "source->usine" -> on traite
        if (estVide(col1) && !estVide(col2) && !estVide(col3) && !estVide(col4) && !estVide(col5)) {
            char* id_usine = col3;
            int vol_capte = atoi(col4);
            float fuite = atof(col5);

            if (!rechercheAVL_USINE(*racine, id_usine)) {

                int h = 0;

                char *copie = malloc(strlen(id_usine) + 1);
                strcpy(copie, id_usine);

                *racine = insertionAVL_USINE(*racine, copie, &h);

                AVL_USINE* usine = rechercherAdresse(*racine, copie);
                if (usine != NULL) {
                    usine->capte = vol_capte;
                    usine->traite = vol_capte * (1.0f - fuite / 100.0f);
                }
            }
            else {
                //modifier valeur
            }
        }
        free(ligne);
    }
}