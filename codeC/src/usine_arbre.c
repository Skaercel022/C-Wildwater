#include "../include/include.h"

// faire le coeffficient de fuites
// Vérifier la fonction InsertionAVL

// Création des structures :

LignesCSV* creerLigneCSV() {
    LignesCSV* ligne=malloc(sizeof(LignesCSV));
    if(ligne == NULL){
        return NULL;
    }
    strcpy(ligne->id, "");
    strcpy(ligne->id_amont, "");
    strcpy(ligne->id_aval, "");
    ligne->Volume = 0.0;
    ligne->fuite = 0.0;
    return ligne;
}

Arbre_liste* constructeurArbre(LignesCSV* ligne){
    Arbre_liste* Arbre=malloc(sizeof(Arbre_liste));
    Arbre->liste=NULL;
    Arbre->nb_enfant=0;
    Arbre->coefficient_parent=1.0;
    Arbre->Volume_parent=ligne->Volume;
    // Copie de l'id
    Arbre->id=malloc(strlen(ligne->id)+1);
    strcpy(Arbre->id, ligne->id);
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
    nouveau->id = malloc(strlen(ligne->id) + 1);
    if (nouveau->id == NULL){
        free(nouveau);
        exit(1);
    }
    strcpy(nouveau->id, ligne->id);
    nouveau->fuite = ligne->fuite;
    nouveau->pGauche = NULL;
    nouveau->pDroit = NULL;
    nouveau->equilibre = 0;
    nouveau->ptr = NULL;
    return nouveau;
}
// Fin création des structures

Code_Erreur ajouter_enfant(Arbre_liste* parent, Arbre_liste* enfant){
    if (parent == NULL || enfant == NULL) {
        return Erreur_Pointeur_Nul;
    }
    Liste* nouveau=constructeurListe(enfant);
    if (nouveau == NULL){
        return Erreur_Allocation;
    }
    nouveau->next=parent->liste; // Insertion en tête de liste
    parent->liste=nouveau;
    parent->nb_enfant+=1;
    return Parsing_OK;
}

Code_Erreur lireEtParserLigne(FILE* fichier, LignesCSV* resultat) {
    // Vérifie si le pointeur du fichier et le résultat sont valides
    if (fichier == NULL || resultat == NULL) {
        return Erreur_Pointeur_Nul;
    }

    // Taille initiale du buffer
    unsigned int capacite = 256;
    char* buffer = malloc(capacite);
    if (buffer == NULL) {
        return Erreur_Allocation;
    }

    unsigned int position = 0;
    int c;

    // Lecture caractère par caractère
    while ((c = fgetc(fichier)) != EOF) {
        if (position >= capacite - 1) {
            capacite *= 2;
            char* tmp = realloc(buffer, capacite);
            if (tmp == NULL) {
                free(buffer);
                return Erreur_Allocation;
            }
            buffer = tmp;
        }
        if (c == '\n') {
            break;
        }
        buffer[position++] = c;
    }

    // Si on arrive à EOF sans avoir lu un seul caractère
    if (position == 0 && c == EOF) {
        free(buffer);
        return Erreur_Format_Token;
    }

    // Terminer la chaîne de caractères
    buffer[position] = '\0';

    // Parsing de la ligne
    char* ligne_copy = strdup(buffer);
    if (ligne_copy == NULL) {
        free(buffer);
        return Erreur_Allocation;
    }

    char* token[5];
    const char delimiteur[] = ";";
    int colonne_count = 0;
    char* token_courant = strtok(ligne_copy, delimiteur);

    while (token_courant != NULL && colonne_count < 5) {
        token[colonne_count] = token_courant;
        colonne_count++;
        token_courant = strtok(NULL, delimiteur);
    }

    free(buffer);

    if (colonne_count != 5) {
        free(ligne_copy);
        return Erreur_NB_colonnes;
    }

    // Remplissage des champs
    strcpy(resultat->id, "-");
    strcpy(resultat->id_amont, "-");
    strcpy(resultat->id_aval, "-");
    resultat->Volume = 0.0;
    resultat->fuite = 0.0;

    strncpy(resultat->id, token[0], LONGUEUR_ID - 1);
    resultat->id[LONGUEUR_ID - 1] = '\0';

    strncpy(resultat->id_amont, token[1], LONGUEUR_ID - 1);
    resultat->id_amont[LONGUEUR_ID - 1] = '\0';

    strncpy(resultat->id_aval, token[2], LONGUEUR_ID - 1);
    resultat->id_aval[LONGUEUR_ID - 1] = '\0';

    if (strcmp(token[3], "-") != 0) {
        resultat->Volume = atof(token[3]);
    }

    if (strcmp(token[4], "-") != 0) {
        resultat->fuite = atof(token[4]);
    }

    free(ligne_copy);
    return Parsing_OK;
}

// Fonctions pour l'AVL
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
    int comparaison=strcmp(ligne->id, racine->id);
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

// Fin de fonctions AVL


Arbre_liste* rechercheliste(Liste* liste, Arbre_liste* Id){
    while (liste != NULL){
        if (strcmp(liste->enfant->id, Id->id) == 0){
            return liste->enfant;
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
        return rechercheArbre(racine->pGauche, id);
    }
    else if(strcmp(id, racine->id) > 0){
        return rechercheArbre(racine->pDroit, id);
    }
    else{
        return NULL;
    }
}

Liste* creationNoeudArbre(AVL_FUITES* racine, LignesCSV* ligne, Liste* liste_arbres){
    if(racine == NULL || ligne == NULL){
        return liste_arbres;
    }
    Arbre_liste* arbre_amont=rechercheArbre(racine, ligne->id_amont);
    if(arbre_amont == NULL){
        return liste_arbres;
    }
    Arbre_liste* parent=rechercheArbre(racine, ligne->id_amont); // je récupère le noeud parent
    Arbre_liste* nouvel_arbre=constructeurArbre(ligne); // je crée le noeud enfant
    if (nouvel_arbre == NULL){
        return liste_arbres;
    }
    if(parent!=NULL){
        Code_Erreur erreur=ajouter_enfant(parent, nouvel_arbre);
        // Petite Verification d'erreur
        if(erreur != Parsing_OK){
            free(nouvel_arbre);
            return liste_arbres;
        }
        return liste_arbres;
    }
    else{
        Liste* nouveau_noeud = constructeurListe(nouvel_arbre); // usine principale donc pas de parent dans le fichier
        if (nouveau_noeud == NULL) {
            free(nouvel_arbre);
            return liste_arbres;
        }
        nouveau_noeud->next = liste_arbres;
        liste_arbres = nouveau_noeud;
    }
    return NULL;
}

// Calcule des fuites totales

double calculer_fuites_rec(Arbre_liste* noeud, double volume_entrant) {
    if (noeud == NULL || volume_entrant <= 0.0 || noeud->nb_enfant == 0) {
        return 0.0;
    }

    double total_fuites = 0.0;
    double volume_par_enfant = volume_entrant / noeud->nb_enfant;
    // Parcourir la liste des enfants
    Liste* liste = noeud->liste;
    while (liste != NULL) {
        if(liste->enfant !=NULL){
            double fuite_enfant = volume_par_enfant * (liste->enfant->coefficient_parent /100.0);
            total_fuites += fuite_enfant;
            // Appel récursif pour les enfants
            total_fuites += calculer_fuites_rec(liste->enfant, volume_par_enfant - fuite_enfant);
        }
        liste = liste->next;
    }
    return total_fuites;
}



double calcule_fuites(const char* nom_fichier, const char* id){
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1.0;
    }
    AVL_FUITES* racine = NULL;
    Liste* liste_arbres = NULL;
    double volume_depart = 0.0;
    int *h = 0;

    LignesCSV* ligne = creerLigneCSV();
    if (ligne == NULL) {
        fclose(fichier);
        return -1.0;
    }
    while(lireEtParserLigne(fichier, ligne) == Parsing_OK){
        h = 0;
        racine = InsertionAVL(racine, ligne, &h);
        Arbre_liste* arbre = rechercheArbre(racine, ligne->id);
        if (arbre != NULL){
            // Mise à jour des informations de l'arbre
            arbre->Volume_parent = ligne->Volume;
            arbre->coefficient_parent = ligne->fuite;

            if (strcmp(ligne->id_amont, id) == 0) {
                volume_depart = ligne->Volume; // Usine principale
            }
        }

        AVL_FUITES* NoeudAVL = InsertionAVL(racine, ligne, NULL);
        if (NoeudAVL == NULL) {
            free(ligne);
            fclose(fichier);
            return -1.0;
        }
        racine = NoeudAVL; // mise à jour de la racine de l'AVL apres une potentielle rotation
        Arbre_liste* arbre = constructeurArbre(ligne); // je construit l'arbre de fuites
        if (arbre == NULL) {
            free(ligne);
            fclose(fichier);
            return -1.0;
        }
        NoeudAVL->ptr = arbre; // je lie l'arbre à son noeud AVL
        liste_arbres = creationNoeudArbre(racine, ligne, liste_arbres); // je crée les liens entre les arbres
        if (liste_arbres == NULL) {
            free(ligne);
            fclose(fichier);
            return -1.0;
        }
    }
    free(ligne);
    fclose(fichier);
    // Recherche de l'arbre correspondant à l'ID donné
    Arbre_liste* arbre_cible = rechercheArbre(racine, (char*) id); // on passe d'un const char a un char*
    if (arbre_cible == NULL) {

    }
    return 0.0;
}