#include "../include/include.h"



// Création des structures :

Arbre_liste* constructeurArbre(char* id, double fuite){
    Arbre_liste* Arbre=malloc(sizeof(Arbre_liste));
    Arbre->liste=NULL;
    Arbre->nb_enfant=0;
    Arbre->coefficient_fuite= fuite;
    Arbre->Volume_parent=0.0;
    // Copie de l'id
    Arbre->id=strdup(id);
    if (Arbre->id == NULL){
        free(Arbre);
        exit(200);
    }
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

int LireetParser(char* id_usine, char* id_amont, char* id_aval, double* volume, double*  fuite){
    if (!id_amont || !id_aval || !volume || !fuite || !id_usine){
        return 0;
    }
    char buffer[1024];
    while(fgets(buffer,sizeof(buffer),stdin)){
        char* token = strtok(buffer, ";");
        id_usine = strdup(token);
        token = strtok(NULL,";");

        id_amont = strdup(token);
        token = strtok(NULL,";");

        id_aval = strdup(token);
        token = strtok(NULL,";");

        *volume = atof(token);
        token = strtok(NULL,";");

        *fuite = atof(token);
    }

    return 1;
}

// Fonctions pour l'AVL
AVL_FUITES* RotationDroite_FUITES(AVL_FUITES* racine) {
    if (!racine || !racine->pGauche) return racine;

    AVL_FUITES* nouvelle_racine = racine->pGauche;
    racine->pGauche = nouvelle_racine->pDroit;
    nouvelle_racine->pDroit = racine;

    // Mise à jour des facteurs d'équilibre
    racine->equilibre = racine->equilibre - 1 - max(nouvelle_racine->equilibre, 0);
    nouvelle_racine->equilibre = nouvelle_racine->equilibre - 1 + min(racine->equilibre, 0);

    return nouvelle_racine;
}

AVL_FUITES* RotationGauche_FUITES(AVL_FUITES* racine) {
    if (!racine || !racine->pDroit) return racine;

    AVL_FUITES* nouvelle_racine = racine->pDroit;
    racine->pDroit = nouvelle_racine->pGauche;
    nouvelle_racine->pGauche = racine;

    // Mise à jour des facteurs d'équilibre
    racine->equilibre = racine->equilibre + 1 - min(nouvelle_racine->equilibre, 0);
    nouvelle_racine->equilibre = nouvelle_racine->equilibre + 1 + max(racine->equilibre, 0);

    return nouvelle_racine;
}

AVL_FUITES* doubleRotationGauche_FUITES(AVL_FUITES* racine){
    racine->pGauche = RotationDroite_FUITES(racine->pGauche);
    return RotationGauche_FUITES(racine);
}

AVL_FUITES* doubleRotationDroite_FUITES(AVL_FUITES* racine){
    racine->pDroit = RotationGauche_FUITES(racine->pDroit);
    return RotationDroite_FUITES(racine);
}


AVL_FUITES* InsertionAVL(AVL_FUITES* racine, Arbre_liste* Noeud, int* h) {
    if (racine == NULL) {
        *h = 1;
        return constructeurAVL(Noeud);
    }

    // Vérification des pointeurs
    if (Noeud == NULL || Noeud->id == NULL || racine->id == NULL) {
        *h = 0;
        return racine;
    }

    int comparaison = strcmp(Noeud->id, racine->id);

    if (comparaison < 0) {
        racine->pGauche = InsertionAVL(racine->pGauche, Noeud, h);
        if (*h != 0) {
            racine->equilibre--;
        }
    }
    else if (comparaison > 0) {
        racine->pDroit = InsertionAVL(racine->pDroit, Noeud, h);
        if (*h != 0) {
            racine->equilibre++;
        }
    }
    else {
        *h = 0;
        return racine;
    }

    // Rééquilibrage
    if (racine->equilibre == 0) {
        *h = 0;
        return racine;
    }
    else if (racine->equilibre == -1 || racine->equilibre == 1) {
        *h = 1;
        return racine;
    }
    else {
        *h = 0;

        // Déséquilibre gauche (-2)
        if (racine->equilibre < -1) {
            // Vérification de l'existence de racine->pGauche
            if (racine->pGauche == NULL) {
                return racine;
            }
            if (racine->pGauche->equilibre > 0) {
                return doubleRotationGauche_FUITES(racine);
            } else {
                return RotationDroite_FUITES(racine);
            }
        }
        // Déséquilibre droite (+2)
        else {
            // Vérification de l'existence de racine->pDroit
            if (racine->pDroit == NULL) {
                return racine;
            }
            if (racine->pDroit->equilibre < 0) {
                return doubleRotationDroite_FUITES(racine);
            } else {
                return RotationGauche_FUITES(racine);
            }
        }
    }
}

// Fin de fonctions AVL


Arbre_liste* rechercheliste(Liste* liste, Arbre_liste* Id) {
    if (liste == NULL || Id == NULL || Id->id == NULL) {
        return NULL;
    }

    int securite = 0; // Compteur de sécurité
    
    while (liste != NULL) {
        // PROTECTION ANTI-BOUCLE INFINIE
        if (securite++ > 10000) { 
            fprintf(stderr, "ERREUR CRITIQUE : Cycle détecté dans la liste des enfants de %s !\n", Id->id);
            exit(505); // On arrête tout pour que tu voies l'erreur
        }

        if (liste->enfant != NULL && liste->enfant->id != NULL) {
            if (strcmp(liste->enfant->id, Id->id) == 0) {
                return liste->enfant;
            }
        }
        liste = liste->next;
    }
    return NULL;
}

Arbre_liste* rechercheArbre(AVL_FUITES* racine, char* id){
    if (racine == NULL || id == NULL){
        return NULL;
    }
    if (racine->id == NULL) {
        return NULL;
    }
    int comparaison = strcmp(id, racine->id);
    //printf("Compare [%s] (len %d) avec [%s] (len %d)\n", id, (int)strlen(id), racine->id, (int)strlen(racine->id));
    if (comparaison == 0) {
        return racine->ptr;
    } else if (comparaison < 0) {
        return rechercheArbre(racine->pGauche, id);
    } else {
        return rechercheArbre(racine->pDroit, id);
    }
}

// ajout d'un volume depuis une source
void ajouterVolumeArbre(double fuite, double Volume, Arbre_liste* racine){
        double fuite_locale = (fuite/100.0)*Volume;
        racine->Volume_parent+=fuite_locale;
        racine->coefficient_fuite=0;
}




void ajouterNoeudArbre(AVL_FUITES** racine_AVL, Arbre_liste** racine_physique, char* id_amont, char* id_aval, double volume, double fuite) {
    if (racine_AVL == NULL || racine_physique == NULL) {
        return;
    }

    int h = 0;

    // --- 1. GÉRER LE PARENT (AMONT) ---
    Arbre_liste* Noeud_amont = rechercheArbre(*racine_AVL, id_amont);
    if (Noeud_amont == NULL) {
        Noeud_amont = constructeurArbre(id_amont, 0.0);
        *racine_AVL = InsertionAVL(*racine_AVL, Noeud_amont, &h);
        if (*racine_physique == NULL) {
            *racine_physique = Noeud_amont;
        }
    }
    if (strcmp(id_amont, id_aval) == 0) {
    return;
    }
    // --- 2. GÉRER L'ENFANT (AVAL) ---
    Arbre_liste* Noeud_aval = rechercheArbre(*racine_AVL, id_aval);
    if (Noeud_aval == NULL) {
        Noeud_aval = constructeurArbre(id_aval,fuite);
        h = 0;
        *racine_AVL = InsertionAVL(*racine_AVL, Noeud_aval, &h);
    }
    else{
        Noeud_aval->coefficient_fuite = fuite;
    }

   
    // IMPORTANT : On n'ajoute l'enfant que s'il n'est pas déjà dans la liste du parent
    if (rechercheliste(Noeud_amont->liste, Noeud_aval) == NULL) {
        ajouter_enfant(Noeud_amont, Noeud_aval);
    }

    if (volume > 0.0) {
        Noeud_aval->Volume_parent += volume;
    }
}

// Calcule des fuites totales

double calculer_fuites_rec(Arbre_liste* noeud, double volume_entrant) {
    if (noeud == NULL || volume_entrant <= 0.0) {
        return 0.0;
    }
    double fuite_locale = (noeud->coefficient_fuite / 100.0) * volume_entrant;
    double total_fuites = fuite_locale;

    // Compter le nombre d'enfants
    int nb_enfant = 0;
    Liste* tmp = noeud->liste;
    while (tmp != NULL) {
        if (tmp->enfant != NULL) {
            nb_enfant++;
        }
        tmp = tmp->next;
    }

    if (nb_enfant == 0) {
        return 0.0; // Pas d'enfants, pas de fuites
    }

    Liste* liste = noeud->liste;
    while (liste != NULL) {
        if (liste->enfant != NULL) {
            double volume_restant = volume_entrant - fuite_locale;
            // Calcul des fuites locales
            double volume_par_enfant = volume_restant / nb_enfant;
            // Volume restant après fuite
            // Appel récursif
            total_fuites +=  calculer_fuites_rec(liste->enfant, volume_par_enfant);
        }
        liste = liste->next;
    }
    return total_fuites;
}

double calculer_fuites(AVL_FUITES* racine_AVL, char* id_usine) {
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
    suppression_AVL_FUITES(racine->pGauche);
    suppression_AVL_FUITES(racine->pDroit);
    free(racine); 
}