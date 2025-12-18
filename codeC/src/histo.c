#include "../include/include.h"

//fonction d'ouverture de fichier
FILE* ouvrirFichierEcriture(char* nom_fichier) {

    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
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

int estVide(char *s) {
    return (s == NULL || strcmp(s, "-") == 0);
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
        
        //ligne "usine" on ajouter volume max
        if (estVide(col1) && estVide(col3) && !estVide(col4) && estVide(col5)) {
            char* id_usine = col2;
            int vol_max = atoi(col4);

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
                if (usine != NULL && usine->max == 0) {
                    usine->max = vol_max;
                }
            }
            //usine déja creee
            else{
                AVL_USINE* usine = rechercheAdresse(*racine, id_usine);
                if (usine != NULL && usine->max == 0) {
                    usine->max = vol_max;
                }
            }
        }
        //ligne "source->usine" on ajouter volume capte et fuite
        else if (estVide(col1) && !estVide(col2) && !estVide(col3) && !estVide(col4) && !estVide(col5)) {
            char* id_usine = col3;
            int vol_capte = atoi(col4);
            float fuite = atof(col5);

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
        //ordre decroissant
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

//fichiers avec les 50 plus petites usines en volume max

//pour volume max
void insererTrierPetitesMax(AVL_USINE** tableau, int* compteur, int maxUsines, AVL_USINE* usine) {

    int i = *compteur - 1;

    //si on a moins de 50 elements
    if (*compteur < maxUsines) {
        (*compteur)++;
    }
    //sinon si l’usine actuelle a un volume plus grand ou égal que la dernière
    else if (usine->max >= tableau[*compteur - 1]->max) {
        return;
    }

    //decale vers la droite si l’usine inseree a un volume plus petit
    while (i >= 0 && tableau[i]->max > usine->max) {
        if (i + 1 < maxUsines) {
            tableau[i + 1] = tableau[i];
        }
        i--;
    }

    tableau[i + 1] = usine;
}

void parcoursInfixeInsertionPetitesMax(AVL_USINE* noeud, AVL_USINE** tableauUsines, int* compteur) {

    if (noeud == NULL)
        return;

    parcoursInfixeInsertionPetitesMax(noeud->pGauche, tableauUsines, compteur);
    insererTrierPetitesMax(tableauUsines, compteur, 50, noeud);
    parcoursInfixeInsertionPetitesMax(noeud->pDroit, tableauUsines, compteur);
}

void fichier50PlusPetitesMax(AVL_USINE* racine, char* nomFichierSortie) {

    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL)
        exit(53);

    AVL_USINE* tableauUsines[50];
    int compteur = 0;

    // sélection basée sur volume max/capte/traite
    parcoursInfixeInsertionPetitesMax(racine, tableauUsines, &compteur);

    // tri alphabetique décroissant
    triRapideUsinesDec(tableauUsines, 0, compteur - 1);

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d;%f\n", tableauUsines[i]->val, tableauUsines[i]->max, tableauUsines[i]->capte, tableauUsines[i]->traite);
    }

    fclose(fichierSortie);
}

// pour volume capte
void insererTrierPetitesCapte(AVL_USINE** tableau, int* compteur, int maxUsines, AVL_USINE* usine) {

    int i = *compteur - 1;

    if (*compteur < maxUsines) {
        (*compteur)++;
    }
    else if (usine->capte >= tableau[*compteur - 1]->capte) {
        return;
    }

    while (i >= 0 && tableau[i]->capte > usine->capte) {
        if (i + 1 < maxUsines) {
            tableau[i + 1] = tableau[i];
        }
        i--;
    }

    tableau[i + 1] = usine;
}

void parcoursInfixeInsertionPetitesCapte(AVL_USINE* noeud, AVL_USINE** tableauUsines, int* compteur) {

    if (noeud == NULL)
        return;

    parcoursInfixeInsertionPetitesCapte(noeud->pGauche, tableauUsines, compteur);
    insererTrierPetitesCapte(tableauUsines, compteur, 50, noeud);
    parcoursInfixeInsertionPetitesCapte(noeud->pDroit, tableauUsines, compteur);
}

void fichier50PlusPetitesCapte(AVL_USINE* racine, char* nomFichierSortie) {

    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL)
        exit(53);

    AVL_USINE* tableauUsines[50];
    int compteur = 0;

    parcoursInfixeInsertionPetitesCapte(racine, tableauUsines, &compteur);
    triRapideUsinesDec(tableauUsines, 0, compteur - 1);

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d;%f\n", tableauUsines[i]->val, tableauUsines[i]->max, tableauUsines[i]->capte, tableauUsines[i]->traite);
    }

    fclose(fichierSortie);
}

// pour volume traite
void insererTrierPetitesTraite(AVL_USINE** tableau, int* compteur, int maxUsines, AVL_USINE* usine) {

    int i = *compteur - 1;

    if (*compteur < maxUsines) {
        (*compteur)++;
    }
    else if (usine->traite >= tableau[*compteur - 1]->traite) {
        return;
    }

    while (i >= 0 && tableau[i]->traite > usine->traite) {
        if (i + 1 < maxUsines) {
            tableau[i + 1] = tableau[i];
        }
        i--;
    }

    tableau[i + 1] = usine;
}

void parcoursInfixeInsertionPetitesTraite(AVL_USINE* noeud, AVL_USINE** tableauUsines, int* compteur) {

    if (noeud == NULL)
        return;

    parcoursInfixeInsertionPetitesTraite(noeud->pGauche, tableauUsines, compteur);
    insererTrierPetitesTraite(tableauUsines, compteur, 50, noeud);
    parcoursInfixeInsertionPetitesTraite(noeud->pDroit, tableauUsines, compteur);
}

void fichier50PlusPetitesTraite(AVL_USINE* racine, char* nomFichierSortie) {

    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL)
        exit(53);

    AVL_USINE* tableauUsines[50];
    int compteur = 0;

    parcoursInfixeInsertionPetitesTraite(racine, tableauUsines, &compteur);
    triRapideUsinesDec(tableauUsines, 0, compteur - 1);

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d;%f\n", tableauUsines[i]->val, tableauUsines[i]->max, tableauUsines[i]->capte, tableauUsines[i]->traite);
    }

    fclose(fichierSortie);
}


//fichier avec les 10 plus grandes usines volume max

//pour volume max

void insererTrieGrandesMax(AVL_USINE** tableau, int* compteur, int maxUsines, AVL_USINE* usine) {
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

void parcoursInfixeInsertionGrandesMax(AVL_USINE* noeud, AVL_USINE** tableauUsines, int* compteur) {
    if (noeud == NULL) {
        return;
    }

    parcoursInfixeInsertionGrandesMax(noeud->pGauche, tableauUsines, compteur);
    insererTrieGrandesMax(tableauUsines, compteur, 10, noeud);
    parcoursInfixeInsertionGrandesMax(noeud->pDroit, tableauUsines, compteur);
}

void fichier10PlusGrandesMax(AVL_USINE* racine, char* nomFichierSortie) {
    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL) {
        exit(53);
    }

    AVL_USINE* tableauUsines[10];
    int compteur = 0;

    parcoursInfixeInsertionGrandesMax(racine, tableauUsines, &compteur);
    triRapideUsinesDec(tableauUsines, 0, compteur - 1);

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d;%f\n", tableauUsines[i]->val, tableauUsines[i]->max, tableauUsines[i]->capte, tableauUsines[i]->traite);
    }

    fclose(fichierSortie);
}

//pour volume capte

void insererTrieGrandesCapte(AVL_USINE** tableau, int* compteur, int maxUsines, AVL_USINE* usine) {
    int i = *compteur - 1;

    if (*compteur < maxUsines) {
        (*compteur)++;
    }
    else if (usine->capte <= tableau[*compteur - 1]->capte) {
        return;
    }

    while (i >= 0 && tableau[i]->capte < usine->capte) {
        if (i + 1 < maxUsines) {
            tableau[i + 1] = tableau[i];
        }
        i--;
    }

    tableau[i + 1] = usine;
}

void parcoursInfixeInsertionGrandesCapte(AVL_USINE* noeud, AVL_USINE** tableauUsines, int* compteur) {
    if (noeud == NULL) {
        return;
    }

    parcoursInfixeInsertionGrandesCapte(noeud->pGauche, tableauUsines, compteur);
    insererTrieGrandesCapte(tableauUsines, compteur, 10, noeud);
    parcoursInfixeInsertionGrandesCapte(noeud->pDroit, tableauUsines, compteur);
}

void fichier10PlusGrandesCapte(AVL_USINE* racine, char* nomFichierSortie) {
    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL) {
        exit(53);
    }

    AVL_USINE* tableauUsines[10];
    int compteur = 0;

    parcoursInfixeInsertionGrandesCapte(racine, tableauUsines, &compteur);
    triRapideUsinesDec(tableauUsines, 0, compteur - 1);

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d;%f\n", tableauUsines[i]->val, tableauUsines[i]->max, tableauUsines[i]->capte, tableauUsines[i]->traite);
    }

    fclose(fichierSortie);
}

//pour volume traite

void insererTrieGrandesTraite(AVL_USINE** tableau, int* compteur, int maxUsines, AVL_USINE* usine) {
    int i = *compteur - 1;

    if (*compteur < maxUsines) {
        (*compteur)++;
    }
    else if (usine->traite <= tableau[*compteur - 1]->traite) {
        return;
    }

    while (i >= 0 && tableau[i]->traite < usine->traite) {
        if (i + 1 < maxUsines) {
            tableau[i + 1] = tableau[i];
        }
        i--;
    }

    tableau[i + 1] = usine;
}

void parcoursInfixeInsertionGrandesTraite(AVL_USINE* noeud, AVL_USINE** tableauUsines, int* compteur) {
    if (noeud == NULL) {
        return;
    }

    parcoursInfixeInsertionGrandesTraite(noeud->pGauche, tableauUsines, compteur);
    insererTrieGrandesTraite(tableauUsines, compteur, 10, noeud);
    parcoursInfixeInsertionGrandesTraite(noeud->pDroit, tableauUsines, compteur);
}

void fichier10PlusGrandesTraite(AVL_USINE* racine, char* nomFichierSortie) {
    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL) {
        exit(53);
    }

    AVL_USINE* tableauUsines[10];
    int compteur = 0;

    parcoursInfixeInsertionGrandesTraite(racine, tableauUsines, &compteur);
    triRapideUsinesDec(tableauUsines, 0, compteur - 1);

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "%s;%d;%d;%f\n", tableauUsines[i]->val, tableauUsines[i]->max, tableauUsines[i]->capte, tableauUsines[i]->traite);
    }

    fclose(fichierSortie);
}

//fonctions principales
void creerFichiersVMax(AVL_USINE* racine) {
    fichier50PlusPetitesMax(racine, "vol_max_50_petites.csv");
    fichier10PlusGrandesMax(racine, "vol_max_10_grandes.csv");
}

void creerFichiersVCapte(AVL_USINE* racine) {
    fichier50PlusPetitesCapte(racine, "vol_capte_50_petites.csv");
    fichier10PlusGrandesCapte(racine, "vol_capte_10_grandes.csv");
}

void creerFichiersVTraite(AVL_USINE* racine) {
    fichier50PlusPetitesTraite(racine, "vol_traite_50_petites.csv");
    fichier10PlusGrandesTraite(racine, "vol_traite_10_grandes.csv");
}