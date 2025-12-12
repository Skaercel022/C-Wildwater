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

int comparerUsines(const void* a, const void* b) {
    AVL_USINE* ua = *(AVL_USINE**)a;
    AVL_USINE* ub = *(AVL_USINE**)b;
    return -strcmp(ua->val, ub->val); //decroissant
}

int getMax(AVL_USINE* u)    { return u->max; }
int getCapte(AVL_USINE* u)  { return u->capte; }
int getTraite(AVL_USINE* u) { return u->traite; }

//fichier avec les 50 plus petites usines en volume max

// Insère une usine dans un tableau trié selon le champ max
void insererTrierPetites(AVL_USINE** tableau, int* compteur, int maxUsines, AVL_USINE* usine, GetVolumeFunc getVolume) {

    int volume = getVolume(usine);
    int i = *compteur - 1;

    //si on a moins de 50 elements
    if (*compteur < maxUsines) {
        (*compteur)++;
    }
    //sinon si l’usine actuelle a un volume plus grand ou égal que la dernière
    else if (volume >= getVolume(tableau[*compteur - 1])) {
        return;
    }

    //decale vers la droite si l’usine inseree a un volume plus petit
    while (i >= 0 && getVolume(tableau[i]) > volume) {
        if (i + 1 < maxUsines) {
            tableau[i + 1] = tableau[i];
        }
        i--;
    }

    tableau[i + 1] = usine;
}

void parcoursInfixeInsertionPetites(AVL_USINE* noeud, AVL_USINE** tableauUsines, int* compteur, GetVolumeFunc getVolume) {

    if (noeud == NULL)
        return;

    parcoursInfixeInsertionPetites(noeud->pGauche, tableauUsines, compteur, getVolume);
    insererTrierPetites(tableauUsines, compteur, 50, noeud, getVolume);
    parcoursInfixeInsertionPetites(noeud->pDroit, tableauUsines, compteur, getVolume);
}

void fichier50PlusPetites(AVL_USINE* racine, const char* nomFichierSortie,
                          GetVolumeFunc getVolume) {

    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL)
        exit(53);

    AVL_USINE* tableauUsines[50];
    int compteur = 0;

    // sélection basée sur volume max/capte/traite
    parcoursInfixeInsertionPetites(racine, tableauUsines, &compteur, getVolume);

    // tri alphabetique croissant
    qsort(tableauUsines, compteur, sizeof(AVL_USINE*), comparerUsines);

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "#%s;%d;%d;%d\n", tableauUsines[i]->val, tableauUsines[i]->max, tableauUsines[i]->capte, tableauUsines[i]->traite);
    }

    fclose(fichierSortie);
}

//fichier avec les 10 plus grandes usines volume max

void insererTrieGrandes(AVL_USINE** tableau, int* compteur, int maxUsines, AVL_USINE* usine, GetVolumeFunc getVolume) {

    int volume = getVolume(usine); // <- dynamique !

    int i = *compteur - 1;

    if (*compteur < maxUsines) {
        (*compteur)++;
    }
    else if (volume <= getVolume(tableau[*compteur - 1])) {
        return;
    }

    while (i >= 0 && getVolume(tableau[i]) < volume) {
        if (i + 1 < maxUsines) {
            tableau[i + 1] = tableau[i];
        }
        i--;
    }

    tableau[i + 1] = usine;
}

void parcoursInfixeInsertionGrandes(AVL_USINE* noeud, AVL_USINE** tableauUsines, int* compteur, GetVolumeFunc getVolume) {
    if (noeud == NULL)
        return;

    parcoursInfixeInsertionGrandes(noeud->pGauche, tableauUsines, compteur, getVolume);
    insererTrieGrandes(tableauUsines, compteur, 10, noeud, getVolume);
    parcoursInfixeInsertionGrandes(noeud->pDroit, tableauUsines, compteur, getVolume);
}

void fichier10PlusGrandes(AVL_USINE* racine, const char* nomFichierSortie, GetVolumeFunc getVolume) {
    FILE* fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL) {
        exit(53);
    }

    AVL_USINE* tableauUsines[10];
    int compteur = 0;

    parcoursInfixeInsertionGrandes(racine, tableauUsines, &compteur, getVolume);
    qsort(tableauUsines, compteur, sizeof(AVL_USINE*), comparerUsines);

    fprintf(fichierSortie, "ID_Usine;Volume_Max;Volume_Capte;Volume_Traite\n");

    for (int i = 0; i < compteur; i++) {
        fprintf(fichierSortie, "#%s;%d;%d;%d\n", tableauUsines[i]->val, tableauUsines[i]->max, tableauUsines[i]->capte, tableauUsines[i]->traite);
    }

    fclose(fichierSortie);
}

void creerFichiersVMax(AVL_USINE* racine) {
    fichier50PlusPetites(racine, "vol_max_50_petites.csv", getMax);
    fichier10PlusGrandes(racine, "vol_max_10_grandes.csv", getMax);
}

void creerFichiersVCapte(AVL_USINE* racine) {
    fichier50PlusPetites(racine, "vol_capte_50_petites.csv", getCapte);
    fichier10PlusGrandes(racine, "vol_capte_10_grandes.csv", getCapte);
}

void creerFichiersVTraite(AVL_USINE* racine) {
    fichier50PlusPetites(racine, "vol_traite_50_petites.csv", getTraite);
    fichier10PlusGrandes(racine, "vol_traite_10_grandes.csv", getTraite);
}
