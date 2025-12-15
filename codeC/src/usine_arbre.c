#include "../include/include.h"



LignesCSV creerLigneCSV() {
    LignesCSV ligne;
    strcpy(ligne.id_usine, "");
    strcpy(ligne.id_amont, "");
    strcpy(ligne.id_aval, "");
    ligne.Volume = 0.0;
    ligne.fuite = 0.0;
    return ligne;
}

AVL_FUITES* constructeurAVL(LignesCSV* ligne){
    AVL_FUITES* nouveau = malloc(sizeof(AVL_FUITES));
    if (nouveau == NULL){
        exit(1);
    }
    nouveau->id = malloc(strlen(ligne->id_usine) + 1);
    if (nouveau->id == NULL){
        free(nouveau);
        exit(1);
    }
    strcpy(nouveau->id, ligne->id_usine);
    nouveau->fuite = ligne->fuite;
    nouveau->pGauche = NULL;
    nouveau->pDroit = NULL;
    nouveau->equilibre = 0;
    nouveau->ptr = NULL;
    return nouveau;
}


Code_Erreur SegmentationLigneCSV(const char* ligne, LignesCSV* resultat){
    // Sépare la ligne en tokens et remplit le struct LignesCSV
    // Vérifie si les pointeurs sont valides
    if (ligne == NULL || resultat == NULL) {
        return Erreur_Format_Token;
    }
    // Vérifie si la ligne est vide
    char* ligne_copy = strdup(ligne); // Crée une copie en allouant directement de la mémoire
    if (ligne_copy == NULL) {
        return Erreur_Allocation;
    }

    char* token;
    const char delimiteur[]= ";";
    int colonne_count = 0;
    // Utilisation de strtok pour diviser la ligne en tokens
    // strtok modifie la chaîne d'origine, donc on travaille sur une copie
    token = strtok(ligne_copy, delimiteur);

    while (token!=NULL) {
        switch (colonne_count) {
            case 0: // Id usine
                strncpy(resultat->id_usine, token, MAX_ID_LENGTH - 1);
                resultat->id_usine[MAX_ID_LENGTH - 1] = '\0'; // Assure que la chaîne est terminée par un caractère nul
                break;
            case 1:
                strncpy(resultat->id_amont, token, MAX_ID_LENGTH - 1);
                resultat->id_amont[MAX_ID_LENGTH - 1] = '\0';
                break;
            case 2:
                strncpy(resultat->id_aval, token, MAX_ID_LENGTH - 1);
                resultat->id_aval[MAX_ID_LENGTH - 1] = '\0';
                break;
            case 3:
                resultat->Volume = atof(token); // Conversion en double d'une chaîne de caractères
                break;
            case 4:
                resultat->fuite = atof(token);
                break;
            default:
                free(ligne_copy);
                return Erreur_NB_colonne;
        }
        token = strtok(NULL, delimiteur);  // Passe au token suivant si possible NULL=> pas de token suivant
        colonne_count++;
    }

    free(ligne_copy);

    if (colonne_count -1 != 5 ) {
        return Erreur_NB_colonne;
    }

    return Parsing_OK;
}


AVL_FUITES* InsertionAVL(AVL_FUITES* racine, LignesCSV* ligne, int* h){
    int comparaison=strcmp(ligne->id_usine, racine->id);
    if (racine == NULL){
        *h = 1;
        return constructeurAVL(ligne);
    }
    if (comparaison < 0){
        racine->pGauche = InsertionAVL(racine->pGauche, ligne, h);
        if(*h!=0){
            racine->equilibre--;
        }
    }
    else if (comparaison > 0){
        racine->pDroit = InsertionAVL(racine->pDroit, ligne, h);
        if(*h!=0){
            racine->equilibre++;
        }
    }
    else{
        *h = 0;
        return racine;
    }
    // Rééquilibrage
    if (racine->equilibre == 0){
        // La hauteur de ce sous-arbre n'a pas changé
        *h = 0; 
        return racine;
    }
    else if (racine->equilibre == -1 || racine->equilibre == 1){
        // On change rien, la hauteur de ce sous-arbre a augmenté de 1.
        *h = 1; 
        return racine;
    }
    else { 
        *h = 0; 
        
        // Déséquilibre gauche (-2)
        if (racine->equilibre < -1) { 
            if (racine->pGauche->equilibre > 0) {
                return doubleRotationGauche_FUITES(racine); 
            } else {
                return rotationDroite_FUITES(racine);
            }
        } 
        // Déséquilibre droite (+2)
        else { 
            if (racine->pDroit->equilibre < 0) {
                return doubleRotationDroite_FUITES(racine);
            } else {
                return rotationGauche_FUITES(racine); 
            }
        }
    }
}


Arbre_liste* recherche(AVL_FUITES* racine, char* id){
    if (racine == NULL){
        return NULL;
    }
    if (strcmp(id, racine->id) == 0){
        return racine->ptr;
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