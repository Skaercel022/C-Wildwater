#include "../include/include.h"



// Création des structures :

LignesCSV* creerLigneCSV() {
    LignesCSV* ligne=malloc(sizeof(LignesCSV));
    if(ligne == NULL){
        return NULL;
    }
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
    Arbre->coefficient_fuite=1.0;
    Arbre->Volume_parent=ligne->Volume;
    // Copie de l'id
    Arbre->id=strdup(ligne->id_aval);
    return Arbre;
}

Liste* constructeurListe(Arbre_liste* enfant){
    Liste* nouveau=malloc(sizeof(Liste));
    nouveau->next=NULL;
    nouveau->enfant=enfant;
    return nouveau;
}

AVL_FUITES* constructeurAVL(Arbre_liste* Noeud){
    AVL_FUITES* nouveau = malloc(sizeof(AVL_FUITES));
    if (nouveau == NULL){
        exit(1);
    }
    nouveau->id = strdup(Noeud->id);
    nouveau->pGauche = NULL;
    nouveau->pDroit = NULL;
    nouveau->equilibre = 0;
    nouveau->ptr = Noeud;
    return nouveau;
}
// Fin création des structures

void ajouter_enfant(Arbre_liste* parent, Arbre_liste* enfant){
    if (parent == NULL || enfant == NULL) {
        exit(200);
    }
    Liste* nouveau=constructeurListe(enfant);
    if (nouveau == NULL){
      exit(200);
    }
    // Empiler :
    nouveau->next=parent->liste;
    parent->liste=nouveau;
    parent->nb_enfant+=1;
}

Code_Erreur lireEtParserLigne(FILE* fichier, LignesCSV* resultat) {
    if (fichier == NULL || resultat == NULL) {
        return Erreur_Pointeur_Nul;
    }
    // Buffer pour lire la ligne
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), fichier) == NULL) {
        return Erreur_Format_Token;
    }
    // Supprimer le '\n' final si présent
    buffer[strcspn(buffer, "\n")] = '\0';
    // Parsing
    char* token[4];
    const char delimiteur[] = ";";
    char* ligne_copy = strdup(buffer);
    if (ligne_copy == NULL) {
        return Erreur_Allocation;
    }
    int colonne_count = 0;
    char* token_courant = strtok(ligne_copy, delimiteur);
    while (token_courant != NULL && colonne_count < 4) {
        token[colonne_count++] = token_courant;
        token_courant = strtok(NULL, delimiteur);
    }
    free(ligne_copy);
    if (colonne_count != 4) {
        return Erreur_NB_colonnes;
    }
    // Remplir les champs
    strncpy(resultat->id_amont, token[0], LONGUEUR_ID - 1);
    resultat->id_amont[LONGUEUR_ID - 1] = '\0';
    strncpy(resultat->id_aval, token[1], LONGUEUR_ID - 1);
    resultat->id_aval[LONGUEUR_ID - 1] = '\0';
    resultat->Volume = atof(token[2]);
    resultat->fuite = atof(token[3]);
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


AVL_FUITES* InsertionAVL(AVL_FUITES* racine, Arbre_liste* Noeud, int* h){
    
    if (racine == NULL){
        *h = 1;
        return constructeurAVL(Noeud);
    }
    int comparaison=strcmp(Noeud->id, racine->id);
    if (comparaison < 0){
        racine->pGauche = InsertionAVL(racine->pGauche, Noeud, h);
        if(*h!=0){
            racine->equilibre--;
        }
    }
    else if (comparaison > 0){
        racine->pDroit = InsertionAVL(racine->pDroit, Noeud, h);
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

Arbre_liste* rechercheArbre(AVL_FUITES* racine, const char* id){
    if (racine == NULL || id == NULL){
        return NULL;
    }
    int comparaison = strcmp(id, racine->id);
    printf("Compare [%s] (len %d) avec [%s] (len %d)\n", id, (int)strlen(id), racine->id, (int)strlen(racine->id));
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

// ajout d'un volume depuis une source
void ajouterVolumeArbre(LignesCSV* ligne, Arbre_liste* racine){
        double fuite = (ligne->fuite/100.0)*ligne->Volume;
        racine->Volume_parent+=fuite;
        racine->coefficient_fuite=0;
}




void ajouterNoeudArbre(AVL_FUITES** racine_AVL, LignesCSV* ligne, Arbre_liste** racine_physique) {
    if (ligne == NULL) return;
    int h = 0;

    // --- 1. GÉRER LE PARENT (AMONT) ---
    Arbre_liste* Noeud_amont = rechercheArbre(*racine_AVL, ligne->id_amont);
    if (Noeud_amont == NULL) {
        // On crée manuellement le parent car on ne l'a pas encore vu
        Noeud_amont = malloc(sizeof(Arbre_liste));
        if(Noeud_amont == NULL){
            exit(666);
        }
        Noeud_amont->liste = NULL;
        Noeud_amont->nb_enfant = 0;
        Noeud_amont->coefficient_fuite = 0.0; // Valeur par défaut
        Noeud_amont->Volume_parent = 0.0;
        
        // On alloue l'ID avec l'AMONT
        Noeud_amont->id = malloc(strlen(ligne->id_amont) + 1);
        strcpy(Noeud_amont->id, ligne->id_amont);
        
        *racine_AVL = InsertionAVL(*racine_AVL, Noeud_amont, &h);
        
        // Premier nœud vu = racine potentielle
        if (*racine_physique == NULL) *racine_physique = Noeud_amont;
    }

    // --- 2. GÉRER L'ENFANT (AVAL) ---
    Arbre_liste* Noeud_aval = rechercheArbre(*racine_AVL, ligne->id_aval);
    if (Noeud_aval == NULL) {
        // On utilise ton constructeur habituel
        Noeud_aval = constructeurArbre(ligne); 
        h = 0;
        *racine_AVL = InsertionAVL(*racine_AVL, Noeud_aval, &h);
    }

    // --- 3. LIAISON ET DONNÉES ---
    // On met à jour les données de l'enfant avec les infos de la ligne actuelle
    Noeud_aval->coefficient_fuite = (float)ligne->fuite;
    
    // IMPORTANT : On n'ajoute l'enfant que s'il n'est pas déjà dans la liste du parent
    // pour éviter les boucles et les Segfaults de récursion
    if (rechercheliste(Noeud_amont->liste, Noeud_aval) == NULL) {
        ajouter_enfant(Noeud_amont, Noeud_aval);
    }

    if (ligne->Volume > 0.0) {
        // Utilise directement l'addition au lieu de ta fonction qui met la fuite à 0
        Noeud_aval->Volume_parent += ligne->Volume;
    }
}

// Calcule des fuites totales

double calculer_fuites_rec(Arbre_liste* noeud, double volume_entrant) {
    if (noeud == NULL || volume_entrant <= 0.0) {
        return 0.0;
    }
    double total_fuites = 0.0;

    // compter le nombre d'enfants
    int nb_enfant =  0;
    Liste *tmp = noeud->liste;
    while (tmp != NULL) {
        nb_enfant++;
        tmp = tmp->next;
    }
    if (nb_enfant == 0) {
        return 0.0; // Pas d'enfants, pas de fuites
    }
    Liste* liste = noeud->liste;
    while (liste != NULL) {
        // Calcul des fuites locales
        double volume_par_enfant = volume_entrant / nb_enfant;
        // Volume restant après fuite
        Arbre_liste* enfant_noeud =  liste->enfant;
        double fuite_locale = volume_par_enfant * (enfant_noeud->coefficient_fuite /100);
        // Appel récursif
        total_fuites += fuite_locale + calculer_fuites_rec(liste->enfant, volume_par_enfant - fuite_locale);
        liste = liste->next;
    }
    return total_fuites;
}

double calculer_fuites(AVL_FUITES* racine_AVL, const char* id_usine) {
    if (racine_AVL == NULL || id_usine == NULL) {
        exit(210);
    }
    // Rechercher le noeud de l'usine
    Arbre_liste* noeud_usine = rechercheArbre(racine_AVL, id_usine);
    if (noeud_usine == NULL) {
        exit(210);
    }
    // Calculer les fuites totales
    return calculer_fuites_rec(noeud_usine, noeud_usine->Volume_parent);
}

void liberer_arbre_physique(Arbre_liste* noeud) {
    if (noeud == NULL) return;

    Liste* actuel = noeud->liste;
    while (actuel != NULL) {
        Liste* a_supprimer = actuel;
        liberer_arbre_physique(actuel->enfant);
        
        actuel = actuel->next;
        free(a_supprimer); // Libère le maillon de la liste
    }

    if (noeud->id != NULL) {
        free(noeud->id); // Libère la chaîne de caractères allouée par strdup/malloc
    }
    
    free(noeud); // Libère la structure Arbre_liste
}

void suppression_AVL_FUITES(AVL_FUITES* racine) {
    if (racine == NULL) return;

    // Parcours post-fixe pour libérer les feuilles avant les parents
    suppression_AVL_FUITES(racine->pGauche);
    suppression_AVL_FUITES(racine->pDroit);

    // Note importante : 
    // On ne fait PAS free(racine->id) ici car c'est le même pointeur que noeud->id
    // qui a déjà été libéré par liberer_arbre_physique.
    
    free(racine); 
}