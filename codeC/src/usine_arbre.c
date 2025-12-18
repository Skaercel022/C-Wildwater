#include "../include/include.h"


// Création des structures :

LignesCSV creerLigneCSV() {
    LignesCSV ligne;
    strcpy(ligne.id_usine, "");
    strcpy(ligne.id_amont, "");
    strcpy(ligne.id_aval, "");
    ligne.Volume = 0.0;
    ligne.fuite = 0.0;
    return ligne;
}

Arbre_liste* constructeurArbre(LignesCSV* ligne){
    Arbre_liste* Arbre=malloc(sizeof(Arbre_liste));
    Arbre->liste=NULL;
    Arbre->nb_enfant=0;
    Arbre->coefficient_parent=1.0;
    Arbre->Volume_parent=ligne->Volume;
    // Copie de l'id_usine
    Arbre->id_usine=malloc(strlen(ligne->id_usine)+1);
    strcpy(Arbre->id_usine, ligne->id_usine);
    return Arbre;
}

Liste* constructeurListe(Arbre_liste* enfant){
    Liste* nouveau=malloc(sizeof(Liste));
    nouveau->next=NULL;
    nouveau->enfant=enfant;
    return nouveau;
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


Code_Erreur ajouter_enfant(Arbre_liste* parent, Arbre_liste* enfant){
    if (parent == NULL || enfant == NULL) {
        return Erreur_Pointeur_Nul;
    }
    Liste* nouveau=constructeurListe(enfant);
    if (nouveau == NULL){
        return Erreur_Allocation;
    }
    nouveau->next=parent->liste;
    parent->liste=nouveau;
    parent->nb_enfant+=1;
    return Parsing_OK;
}

Code_Erreur SegmentationLigneCSV(const char* ligne, LignesCSV* resultat){
    // Sépare la ligne en tokens et remplit le struct LignesCSV
    // Vérifie si les pointeurs sont valides
    if (ligne == NULL || resultat == NULL) return Erreur_Format_Token;

    // Vérifie si la ligne est vide
    char* ligne_copy = strdup(ligne); // Crée une copie en allouant directement de la mémoire
    if (ligne_copy == NULL) {
        return Erreur_Allocation;
    }

    char* token[5];
    const char delimiteur[]= ";";
    int colonne_count = 0;
    // Utilisation de strtok pour diviser la ligne en tokens
    // strtok modifie la chaîne d'origine, donc on travaille sur une copie
    token = strtok(ligne_copy, delimiteur);

    while (token!=NULL && colonne_count < 5) {
        switch (colonne_count) {
            case 0: // Id usine
                strncpy(resultat->id_usine, token, LONGUEUR_ID - 1);
                resultat->id_usine[LONGUEUR_ID - 1] = '\0'; // Assure que la chaîne est terminée par un caractère nul
                break;
            case 1:
                strncpy(resultat->id_amont, token, LONGUEUR_ID - 1);
                resultat->id_amont[LONGUEUR_ID - 1] = '\0';
                break;
            case 2:
                strncpy(resultat->id_aval, token, LONGUEUR_ID - 1);
                resultat->id_aval[LONGUEUR_ID - 1] = '\0';
                break;
            case 3:
                resultat->Volume = atof(token); // Conversion en double d'une chaîne de caractères
                break;
            case 4:
                resultat->fuite = atof(token);
                break;
            default:
                free(ligne_copy);
                return Erreur_NB_colonnes;
        }
        token = strtok(NULL, delimiteur);  // Passe au token suivant si possible NULL=> pas de token suivant
        colonne_count++;
    }

    free(ligne_copy);

    if (colonne_count != 5 ) {
        return Erreur_NB_colonnes;
    }

    return Parsing_OK;
}

AVL_FUITES* RotationDroite(AVL_FUITES* racine){
    AVL_FUITES* nouvelle_racine = racine->pGauche;
    racine->pGauche = nouvelle_racine->pDroit;
    nouvelle_racine->pDroit = racine;

    // Mise à jour des facteurs d'équilibre
    racine->equilibre = racine->equilibre + 1 - (nouvelle_racine->equilibre < 0 ? nouvelle_racine->equilibre : 0);
    nouvelle_racine->equilibre = nouvelle_racine->equilibre + 1 + (racine->equilibre > 0 ? racine->equilibre : 0);

    return nouvelle_racine;
}

AVL_FUITES* rotationgauche_FUITES(AVL_FUITES* racine){
    AVL_FUITES* nouvelle_racine = racine->pDroit;
    racine->pDroit = nouvelle_racine->pGauche;
    nouvelle_racine->pGauche = racine;

    // Mise à jour des facteurs d'équilibre
    racine->equilibre = racine->equilibre - 1 - (nouvelle_racine->equilibre > 0 ? nouvelle_racine->equilibre : 0);
    nouvelle_racine->equilibre = nouvelle_racine->equilibre - 1 + (racine->equilibre < 0 ? racine->equilibre : 0);

    return nouvelle_racine;
}

AVL_FUITES* doubleRotationGauche_FUITES(AVL_FUITES* racine){
    racine->pGauche = rotationgauche_FUITES(racine->pGauche);
    return RotationDroite(racine);
}

AVL_FUITES* doubleRotationDroite_FUITES(AVL_FUITES* racine){
    racine->pDroit = RotationDroite(racine->pDroit);
    return rotationgauche_FUITES(racine);
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
                return RotationDroite(racine);
            }
        } 
        // Déséquilibre droite (+2)
        else { 
            if (racine->pDroit->equilibre < 0) {
                return doubleRotationDroite_FUITES(racine);
            } else {
                return rotationgauche_FUITES(racine); 
            }
        }
    }
}

Liste* rechercheliste(Liste* liste, Arbre_liste* id){
    while (liste != NULL){
        if (strcmp(liste->enfant->id_usine, id->id_usine) == 0){
            return liste;
        }
        liste = liste->next;
    }
    return NULL;
}

Arbre_liste* rechercheArbre(AVL_FUITES* racine, char* id){
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

Liste* creationArbre(AVL_FUITES* racine, LignesCSV* ligne, Liste* liste_usines){
    if(racine == NULL || ligne == NULL){
        return liste_usines;
    }
    // Recherche de l'arbre de l'usine amont
    Liste* liste_amont=rechercheliste(liste_usines, rechercheArbre(racine, ligne->id_amont));
    if (liste_amont == NULL){
        // L'usine amont n'existe pas encore dans la liste, on la crée
}
