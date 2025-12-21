#include "../include/include.h"

//toutes les fonctions de construct
Arbre_liste* constructeurArbre(char* id, double fuite){
    Arbre_liste* Arbre=malloc(sizeof(Arbre_liste));
    if(Arbre == NULL){
        printf("Erreur allocation memoire constructeurArbre\n");
        exit(200);
    }
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
    if(enfant == NULL){
        printf("Erreur : enfant NULL dans constructeurListe\n");
        exit(250);
    }
    Liste* nouveau=malloc(sizeof(Liste));
    if(nouveau == NULL){
        printf("Erreur allocation memoire constructeurListe\n");
        exit(250);
    }
    nouveau->next=NULL;
    nouveau->enfant=enfant;
    return nouveau;
}

AVL_FUITES* constructeurAVL(Arbre_liste* Noeud){
    if(!Noeud){
        printf("Racine de base nulle dans constructeurAVL\n");
        exit(250);
    }
    AVL_FUITES* nouveau = malloc(sizeof(AVL_FUITES));
    if (nouveau == NULL) {
        printf("Erreur allocation memoire constructeurAVL\n");
        exit(1);
    }
    nouveau->id = strdup(Noeud->id);
    nouveau->pGauche = NULL;
    nouveau->pDroit = NULL;
    nouveau->equilibre = 0;
    nouveau->ptr = Noeud;
    return nouveau;
}

void ajouter_enfant(Arbre_liste* parent, Arbre_liste* enfant){
    if (parent == NULL || enfant == NULL) {
        exit(200);
    }
    Liste* nouveau=constructeurListe(enfant);
    if (nouveau == NULL){
        exit(200);
    }
    nouveau->next=parent->liste;
    parent->liste=nouveau;
    parent->nb_enfant+=1;
}
//lire ligne et la parser
int LireetParser(char* id_usine, char* id_amont, char* id_aval, double* volume, double*  fuite, char* buffer){
    if (!id_amont || !id_aval || !volume || !fuite || !id_usine || !buffer){
        return 0;
    }
    char volbuffer[256];
    char futbuffer[256];
    int nb = sscanf(buffer, "%255[^;];%255[^;];%255[^;];%255[^;];%255[^;]", id_usine, id_amont, id_aval, volbuffer, futbuffer);
    if(nb != 5){
        return 0;
    }
    
    *volume = atof(volbuffer);
    *fuite = atof(futbuffer);
    return 1;
}

AVL_FUITES* RotationGauche_FUITES(AVL_FUITES* racine) {
    if (racine == NULL || racine->pDroit == NULL){
        return racine;
    } 

    AVL_FUITES* pivot = racine->pDroit;
    racine->pDroit = pivot->pGauche;
    pivot->pGauche = racine;

    int eq_racine = racine->equilibre;
    int eq_p = pivot->equilibre;

    racine->equilibre = eq_racine - max(eq_p, 0) - 1;
    pivot->equilibre = min(min(eq_racine - 2, eq_racine + eq_p - 2), eq_p - 1);

    racine = pivot;
    return racine;
}

AVL_FUITES* RotationDroite_FUITES(AVL_FUITES* racine){
    //rotation
    if(racine == NULL || racine->pGauche == NULL){
        return racine;
    }
    AVL_FUITES* pivot = racine->pGauche;
    racine->pGauche = pivot->pDroit;
    pivot->pDroit = racine;

    int eq_racine = racine->equilibre;
    int eq_pivot = pivot->equilibre;

    racine->equilibre = eq_racine - min(eq_pivot, 0) + 1;
    pivot->equilibre = max(max(eq_racine + 2, eq_racine + eq_pivot + 2), eq_pivot + 1);
    racine = pivot;
    return racine;
}    

AVL_FUITES* doubleRotationGauche_FUITES(AVL_FUITES* racine){
    racine->pGauche = RotationDroite_FUITES(racine->pGauche);
    return RotationGauche_FUITES(racine);
}

AVL_FUITES* doubleRotationDroite_FUITES(AVL_FUITES* racine){
    racine->pDroit = RotationGauche_FUITES(racine->pDroit);
    return RotationDroite_FUITES(racine);
}

AVL_FUITES* equilibrerAVL(AVL_FUITES* racine){
    if(!racine) return racine;
    if(racine->equilibre >= 2){
        if (racine->pDroit && racine->pDroit->equilibre >= 0){
            return RotationGauche_FUITES(racine);
        }
        else{
            return doubleRotationGauche_FUITES(racine);
        }
    }
    else if(racine->equilibre <= -2){
        if(racine->pGauche && racine->pGauche->equilibre <= 0){
            return RotationDroite_FUITES(racine);
        }
        else{
            return doubleRotationDroite_FUITES(racine);
        }
    }
    return racine;
}


AVL_FUITES* InsertionAVL(AVL_FUITES* racine_avl, Arbre_liste* Noeud, int* h) {
    if (racine_avl == NULL) {
        *h = 1;
        return constructeurAVL(Noeud);
    }

    if (Noeud == NULL || Noeud->id == NULL || racine_avl->id == NULL) {
        *h = 0;
        return racine_avl;
    }

    int comparaison = strcmp(Noeud->id, racine_avl->id);

    if (comparaison < 0) {
        racine_avl->pGauche = InsertionAVL(racine_avl->pGauche, Noeud, h);
        *h = - *h;
    }
    else if (comparaison > 0) {
        racine_avl->pDroit = InsertionAVL(racine_avl->pDroit, Noeud, h);
    }
    else {
        *h = 0;
        return racine_avl;
    }
    if (*h != 0){
        racine_avl->equilibre += *h;
        racine_avl = equilibrerAVL(racine_avl);
        if(racine_avl->equilibre == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return racine_avl;
}



Arbre_liste* rechercheliste(Liste* liste, Arbre_liste* arbre) {
    if (liste == NULL || arbre == NULL || arbre->id == NULL) {
        return NULL;
    }
    Liste* p1 = liste;
    while (p1 != NULL) {
        if (p1->enfant != NULL && p1->enfant->id != NULL) {
            if (strcmp(p1->enfant->id, arbre->id) == 0) {
                return p1->enfant;
            }
        }
        p1 = p1->next;
    }
    return NULL;
}

Arbre_liste* rechercheArbre(AVL_FUITES* racine, char* id){
    if (racine == NULL){
        printf("Debug racine null\n");
        return NULL;
    }
    if( id == NULL) {
        return NULL;
    }
    if (racine->id == NULL ) {
        return NULL;
    }
    int comparaison = strcmp(id, racine->id);
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




void ajouterNoeudArbre(AVL_FUITES** racine_AVL, char* id_amont, char* id_aval, double volume, double fuite) {
    int h=0;
    if(id_amont == NULL){
        exit(205);
    }
    double volume_vers_usine =0.0;
    //gestion d'une source : 
    if(volume>0.0 && fuite != 0.0){
        volume_vers_usine += volume * (fuite /100.0);
        Arbre_liste* nouveau = constructeurArbre(id_aval,volume_vers_usine);
        nouveau->Volume_parent += volume_vers_usine;
        *racine_AVL = InsertionAVL(*racine_AVL,nouveau,&h);
        if(*racine_AVL == NULL){
            exit(220);
        }
    }

    else{
        Arbre_liste* nouveau = constructeurArbre(id_aval,volume_vers_usine);
        Arbre_liste* parent = rechercheArbre(*racine_AVL,id_amont);
        if(parent == NULL){
            exit(205);
        }
        *racine_AVL = InsertionAVL(*racine_AVL,nouveau,&h);
        ajouter_enfant(parent,nouveau);
        
        if(*racine_AVL == NULL){
            exit(220);
        }

        
    }
}


//Calcul des fuites totales
double calculer_fuites_rec(Arbre_liste* noeud, double volume_entrant) {
    if (noeud == NULL || volume_entrant <= 0.0) {
        return 0.0;
    }
    double fuite_locale = (noeud->coefficient_fuite / 100.0) * volume_entrant;
    double total_fuites = fuite_locale;

    int nb_enfant = 0;
    Liste* tmp = noeud->liste;
    while (tmp != NULL) {
        if (tmp->enfant != NULL) {
            nb_enfant++;
        }
        tmp = tmp->next;
    }

    if (nb_enfant == 0) {
        return total_fuites;
    }

    Liste* liste = noeud->liste;
    while (liste != NULL) {
        if (liste->enfant != NULL) {
            double volume_restant = volume_entrant - fuite_locale;
            double volume_par_enfant = volume_restant / nb_enfant;
            total_fuites +=  calculer_fuites_rec(liste->enfant, volume_par_enfant);
        }
        liste = liste->next;
    }
    return total_fuites;
}

double calculer_fuites(AVL_FUITES* racine_AVL, char* id_usine) {
    if (racine_AVL == NULL || id_usine == NULL) {
        return -1.0;
    }
    Arbre_liste* noeud_usine = rechercheArbre(racine_AVL, id_usine);
    if (noeud_usine == NULL) {
        return -1.0;   
    }

    if(noeud_usine->Volume_parent<=0.0){
        return 0.0;
    }
    return calculer_fuites_rec(noeud_usine, noeud_usine->Volume_parent);
}

void liberer_arbre_physique(Arbre_liste* noeud) {
    if (noeud == NULL) return;

    Liste* actuel = noeud->liste;
    while (actuel != NULL) {
        Liste* a_supprimer = actuel;
        liberer_arbre_physique(actuel->enfant);
        
        actuel = actuel->next;
        free(a_supprimer);
    }

    if (noeud->id != NULL) {
        free(noeud->id);
    }
    
    free(noeud);
}

void suppression_AVL_FUITES(AVL_FUITES* racine) {
    if (racine == NULL) return;
    suppression_AVL_FUITES(racine->pGauche);
    suppression_AVL_FUITES(racine->pDroit);
    free(racine); 
}

int ecriture_fichier(char* id, double volume){
    if(id == NULL){
        return 1;
    }
    FILE* f = fopen("./fuites.csv","w+");
    if( f == NULL){
        return 1;
    }
    fprintf(f,"%s;%.3f\n",id, volume);
    fclose(f);
    return 0;
}