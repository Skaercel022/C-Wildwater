//objectif de cette partie : ouvrir fichier avec les sources et les usines
//lire les données et modifier l'avl_usine usine
//creer fichiers avec les 10 plus grandes usines et 50 plus petites usines
//creer fichier avec toutes les usines triées par ordre alphabétique décroissant

//to do list de chose à faire :
//supprimer premiere ligne des fichiers csv (entete)
//convertir les milliers de m3 en millions de m3

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

int estVide(const char *s) {
    return (strcmp(s, "-") == 0);
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
//FONCTION A TESTER

void lectureFichierVersAVL(FILE* fichier, AVL_USINE** racine) {
    char* ligne = NULL;
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
        
        //-----------------------------------
        //ligne "usine" on ajouter volume max
        //-----------------------------------
        if (estVide(col1) && estVide(col3) && !estVide(col4) && estVide(col5)) {
            char* id_usine_entier = col2;
            int vol_max = atoi(col4);

            //cherche le caractère # pour enlever le nom de l'usine
            char* ptr = strchr(id_usine_entier, '#');
            char* id_usine = NULL;

            if (ptr != NULL) {
                //avance d'un caractère pour sauter le #
                id_usine = ptr + 1;
            }
            else {
                free(ligne);
                exit(61);
            }

            if (!rechercheAVL_USINE(*racine, id_usine)) {

                int h = 0;

                char* copie = malloc(strlen(id_usine) + 1);
                if (copie == NULL) {
                    free(ligne);
                    exit(52);
                }
                strcpy(copie, id_usine);

                *racine = insertionAVL_USINE(*racine, copie, &h);

                AVL_USINE* usine = rechercheAdresse(*racine, copie);
                if (usine != NULL) {
                    usine->max = vol_max;
                }
            }
            //usine déja creee
            else{
                AVL_USINE* usine = rechercheAdresse(*racine, id_usine);
                if (usine != NULL) {
                    usine->max = vol_max;
                }
            }
        }
        //------------------------------------------------------
        //ligne "source->usine" on ajouter volume capte et fuite
        //------------------------------------------------------
        else if (estVide(col1) && !estVide(col2) && !estVide(col3) && !estVide(col4) && !estVide(col5)) {
            char* id_usine_entier = col3;
            int vol_capte = atoi(col4);
            float fuite = atof(col5);

            //cherche le caractère # pour enlever le nom de l'usine
            char* ptr = strchr(id_usine_entier, '#');
            char* id_usine = NULL;

            if (ptr != NULL) {
                //avance d'un caractère pour sauter le #
                id_usine = ptr + 1;
            }
            else {
                free(ligne);
                exit(61);
            }

            if (!rechercheAVL_USINE(*racine, id_usine)) {

                int h = 0;

                char* copie = malloc(strlen(id_usine) + 1);
                if (copie == NULL) {
                    free(ligne);
                    exit(52);
                }
                strcpy(copie, id_usine);

                *racine = insertionAVL_USINE(*racine, copie, &h);

                AVL_USINE* usine = rechercheAdresse(*racine, copie);
                if (usine != NULL) {
                    usine->capte = vol_capte;
                    usine->traite = vol_capte * (1.0f - fuite / 100.0f);
                }
            }
            //usine déja creee
            else {
                AVL_USINE* usine = rechercheAdresse(*racine, id_usine);
                if (usine != NULL){
                    usine->capte += vol_capte;
                    usine->traite += vol_capte * (1.0f - fuite / 100.0f);
                }
            }
        }
        free(ligne);
    }
}

//fichier avec les 50 plus petites usines en volume max

// Insère une usine dans un tableau trié selon le champ max
void insererTrie(AVL_USINE** tableau, int* compteur, int maxUsines, AVL_USINE* usine) {
    int i = *compteur - 1;

    //si on a moins de 50 elements
    if (*compteur < maxUsines) {
        (*compteur)++;
    }
    //sinon si l'usine actuelle a un max plus grand que le dernier, on ne l'insere pas
    else if (usine->max >= tableau[*compteur - 1]->max) {
        return;
    }

    //decale vers la droite tant que l'usine insérée a un max plus petit
    while (i >= 0 && tableau[i]->max > usine->max) {
        if (i + 1 < maxUsines) {
            tableau[i + 1] = tableau[i];
        }
        i--;
    }

    //place l'usine à la bonne position
    tableau[i + 1] = usine;
}

void parcoursInfixeInsertionPetites(AVL_USINE* noeud, AVL_USINE** tableauUsines, int* compteur) {
    if (noeud == NULL) {
        return;
    }

    parcoursInfixeInsertionPetites(noeud->pGauche, tableauUsines, compteur);
    insererTrie(tableauUsines, compteur, 50, noeud);
    parcoursInfixeInsertionPetites(noeud->pDroit, tableauUsines, compteur);
}

void fichier50PlusPetites(AVL_USINE* racine, const char* nomFichierSortie) {
    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL) {
        exit(53);
    }

    AVL_USINE* tableauUsines[50];
    int compteur = 0;

    parcoursInfixeInsertionPetites(racine, tableauUsines, &compteur);

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d;%d\n", tableauUsines[i]->val, tableauUsines[i]->max, tableauUsines[i]->capte, tableauUsines[i]->traite);
    }

    fclose(fichierSortie);
}

//fichier avec les 10 plus grandes usines volume max

void insererTrieGrandes(AVL_USINE** tableau, int* compteur, int maxUsines, AVL_USINE* usine) {
    int i = *compteur - 1;

    if (*compteur < maxUsines) {
        (*compteur)++;
    }
    else if (usine->max <= tableau[*compteur - 1]->max) {
        return;
    }

    while (i >= 0 && tableau[i]->max < usine->max) {
        if (i + 1 < maxUsines) {
            tableau[i + 1] = tableau[i];
        }
        i--;
    }

    tableau[i + 1] = usine;
}

void parcoursInfixeInsertionGrandes(AVL_USINE* noeud, AVL_USINE** tableauUsines, int* compteur) {
    if (noeud == NULL) {
        return;
    }

    parcoursInfixeInsertionGrandes(noeud->pGauche, tableauUsines, compteur);
    insererTrieGrandes(tableauUsines, compteur, 10, noeud);
    parcoursInfixeInsertionGrandes(noeud->pDroit, tableauUsines, compteur);
}

void fichier10PlusGrandes(AVL_USINE* racine, const char* nomFichierSortie) {
    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL) {
        exit(53);
    }

    AVL_USINE* tableauUsines[10];
    int compteur = 0;

    parcoursInfixeInsertionGrandes(racine, tableauUsines, &compteur);

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d;%d\n", tableauUsines[i]->val, tableauUsines[i]->max, tableauUsines[i]->capte, tableauUsines[i]->traite);
    }

    fclose(fichierSortie);
}

//creer fichier de toutes les usines triées par ordre alphabétique decroissant

void fichierUsinesAlphabetiqueDecroissant(AVL_USINE* racine, const char* nomFichierSortie) {
    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL) {
        exit(54);
    }

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    parcoursDecroissantEcriture(racine, fichierSortie);

    fclose(fichierSortie);
}

void parcoursDecroissantEcriture(AVL_USINE* noeud, FILE* fichier) {
    if (noeud == NULL) {
        return;
    }

    parcoursDecroissantEcriture(noeud->pDroit, fichier);

    fprintf(fichier, "%s;%d;%d;%d\n", noeud->val, noeud->max, noeud->capte, noeud->traite);

    parcoursDecroissantEcriture(noeud->pGauche, fichier);
}

void creerFichiersRapports(AVL_USINE* racine) {
    fichier10PlusGrandes(racine, "10_plus_grandes_usines.csv");
    fichier50PlusPetites(racine, "50_plus_petites_usines.csv");
    fichierUsinesAlphabetiqueDecroissant(racine, "usines_alphabetique_decroissant.csv");
}
