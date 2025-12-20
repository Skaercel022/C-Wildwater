#include "../include/include.h"



// Création des structures :

Arbre_liste* creerArbreListe(const char* id, double volume, double fuite){
    Arbre_liste* a = malloc(sizeof(Arbre_liste));
    if (!a){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    a->id = strdup(id);
    a->liste = NULL;
    a->nb_enfant = 0;
    a->coefficient_fuite = fuite;
    a->Volume_parent = volume;

    return a;
}

void ajouterEnfant(Arbre_liste* parent, Arbre_liste* enfant){
    if (!parent || !enfant) return;

    Liste* l = malloc(sizeof(Liste));
    if (!l){
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    l->enfant = enfant;
    l->next = parent->liste;
    parent->liste = l;
    parent->nb_enfant++;
}

double propagation_fuites(Arbre_liste* a, double volume){
    if (!a || volume <= 0.0) return 0.0;

    double perte = volume * (a->coefficient_fuite / 100.0);
    double restant = volume - perte;
    double total = perte;

    if (a->nb_enfant > 0){
        double part = restant / a->nb_enfant;
        Liste* l = a->liste;

        while (l){
            l->enfant->Volume_parent = part;
            total += propagation_fuites(l->enfant, part);
            l = l->next;
        }
    }
    return total;
}

void libererArbreListe(Arbre_liste* a){
    if (!a) return;

    Liste* l = a->liste;
    while (l){
        Liste* tmp = l;
        libererArbreListe(l->enfant);
        l = l->next;
        free(tmp);
    }

    free(a->id);
    free(a);
}

/* ================= AVL DES FUITES ================= */

AVL_FUITES* creerAVL_Fuites(const char* id, Arbre_liste* ptr){
    if (!id || !ptr) {
        printf("Debug ptr nul creeavl_fuite");
        return NULL;
    }

    AVL_FUITES* n = malloc(sizeof(AVL_FUITES));
    if (!n){
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    n->id = strdup(id);
    if (!n->id) {
        perror("strdup");
        free(n);
        exit(EXIT_FAILURE);
    }
    n->ptr = ptr;
    n->equilibre = 0;
    n->pGauche = NULL;
    n->pDroit = NULL;

    return n;
}

AVL_FUITES* rotationGaucheAVL(AVL_FUITES* A){
    if(A == NULL){
        printf("Le Noeud AVL est null\n");
        exit(200);
    }
    AVL_FUITES* ptr;
    int eq_a;
    int eq_p;
    ptr = A->pDroit;
    if (!ptr) {
        fprintf(stderr, "Rotation gauche impossible (pDroit NULL)\n");
        return A;
    }
    A->pDroit = ptr->pGauche;
    ptr->pDroit = A;
    eq_a = A->equilibre;
    eq_p = ptr->equilibre;
    A->equilibre = eq_a - max(eq_p,0) -1;
    ptr->equilibre = min(min(eq_a-2,eq_a+ eq_p-2), eq_p-1);
    A=ptr;
    return A;
}

AVL_FUITES* rotationDroiteAVL(AVL_FUITES* A){
    if(A == NULL){
        printf("Le Noeud AVL est null\n");
        exit(200);
    }
    AVL_FUITES* ptr;
    int eq_a;
    int eq_p;
    ptr = A->pGauche;
    if (!ptr) {
        fprintf(stderr, "Rotation gauche impossible (pDroit NULL)\n");
        return A;
    }
    A->pGauche = ptr->pDroit;
    ptr->pDroit = A;
    eq_a = A->equilibre;
    eq_p = ptr->equilibre;
    A->equilibre = eq_a - min(eq_p,0)+1;
    ptr->equilibre = max(max(eq_a+2,eq_a+eq_p+2), eq_p+1);
    A=ptr;
    return A;
}

AVL_FUITES* equilibreAVL(AVL_FUITES* A) {
    if (!A) return NULL;

    // Cas déséquilibre à droite
    if (A->equilibre >= 2) {
        if (A->pDroit && A->pDroit->equilibre >= 0) {
            // Rotation simple gauche
            A = rotationGaucheAVL(A);
        } else if (A->pDroit) {
            // Rotation double droite-gauche
            A->pDroit = rotationDroiteAVL(A->pDroit);
            A = rotationGaucheAVL(A);
        }
    }
    // Cas déséquilibre à gauche
    else if (A->equilibre <= -2) {
        if (A->pGauche && A->pGauche->equilibre <= 0) {
            // Rotation simple droite
            A = rotationDroiteAVL(A);
        } else if (A->pGauche) {
            // Rotation double gauche-droite
            A->pGauche = rotationGaucheAVL(A->pGauche);
            A = rotationDroiteAVL(A);
        }
    }

    return A;
}

AVL_FUITES* insertionAVL_Fuites(
    AVL_FUITES* a,
    Arbre_liste* ptr,
    const char* id,
    int* h
){
    printf("Debug insertion\n");
    if (!a){
        *h = 1;
        return creerAVL_Fuites(id, ptr);
    }

    if (strcmp(id, a->id) < 0){
        a->pGauche = insertionAVL_Fuites(a->pGauche, ptr, id, h);
        *h = -*h;
    }
    else if (strcmp(id, a->id) > 0){
        a->pDroit = insertionAVL_Fuites(a->pDroit, ptr, id, h);
    }
    else{
        *h = 0;
        return a;
    }

    if (*h){
        a->equilibre += *h;
        a = equilibreAVL(a);

        *h = (a->equilibre == 0) ? 0 : 1;
    }
    return a;
}

Arbre_liste* rechercherAVL_Fuites(AVL_FUITES* a, const char* id){
    if (!a || !id) {
        printf("Debug recherche avl fuite\n");
        return NULL;
    }
    printf("1Debug recherche avl fuite\n");
    printf("ID %s\n", id);
    printf("A->ID %s\n", a->id);
    int c = strcmp(id, a->id);
    printf("c %d\n", c);
    if (c == 0) return a->ptr;
    else if (c < 0) return rechercherAVL_Fuites(a->pGauche, id);
    else {
        return rechercherAVL_Fuites(a->pDroit, id);
    }
}

double calcul_fuites_AVL(AVL_FUITES* a, const char* id){
    Arbre_liste* n = rechercherAVL_Fuites(a, id);
    if (!n) return -1.0;
    return propagation_fuites(n, n->Volume_parent);
}

void libererAVL_Fuites(AVL_FUITES* a){
    if (!a) return;

    libererAVL_Fuites(a->pGauche);
    libererAVL_Fuites(a->pDroit);

    free(a->id);
    free(a);
}

int traitement_ligne_fuite(
    const char* buffer,
    char* parent,
    char* enfant,
    char* service,
    double* valeur,
    double* fuite,
    double* somme
) {
    if (!buffer || !parent || !enfant || !service || !valeur || !fuite || !somme){
        return 0;
    }
    char valeur_str[64];
    char fuite_str[64];

    int nb = sscanf(buffer,"%63[^;];%63[^;];%63[^;];%63[^;];%63[^;\n]",parent, enfant, service, valeur_str, fuite_str);

    if (nb != 5) {
        return 0;
    }
    //Vérif si source
    if (strcmp(parent,"-")==0 && strcmp(valeur_str, "-") !=0 && strcmp(fuite_str, "-") != 0) {
        if(somme) {
            double volume = atof(valeur_str);
            double fuite  = atof(fuite_str) / 100.0;

            *somme += volume * (1.0 - fuite);

        }
        
    }
    if (strcmp(valeur_str, "-") == 0) {
        *valeur = 0.0;
    } else {
        *valeur = atof(valeur_str);
    }

    // Traitement du champ fuite
    if (strcmp(fuite_str, "-") == 0) {
        *fuite = 0.0;
    } else {
        *fuite = atof(fuite_str);
    }

    return 1;
}    

void ecriture_fichier_leaks(char* id_usine, double volume){
    FILE* fichier_data;
    double volume_lu;
    int trouve = 0;
    char id_lu[256];
    fichier_data = fopen("fuites.csv", "a+");
    if (fichier_data == NULL) {
        return;   
    }
    rewind(fichier_data);
    //verif si ligne deja lu
    while (fscanf(fichier_data, "%49[^;];%lf;\n", id_lu, &volume_lu) != EOF) {
        if (strcmp(id_usine, id_lu) == 0) {
            trouve = 1;
            break;
        }
    }
    if (!trouve) {
        fprintf(fichier_data, "%s;%.4f k.m3;\n", id_usine, volume);
    }
    else{
        fprintf(fichier_data, "%s;%d \n", id_usine, -1);
    }
    fclose(fichier_data);     
}