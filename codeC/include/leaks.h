#ifndef LEAKS_H
#define LEAKS_H

#define LONGUEUR_ID 64

// Déclarations anticipées
typedef struct liste Liste;
typedef struct arbre_liste Arbre_liste;

// Définition des structures
struct liste {
    Liste* next;
    Arbre_liste* enfant;  // pointeur vers un Arbre_liste
};

struct arbre_liste {
    Liste* liste;
    char* id;
    int nb_enfant;
    float coefficient_fuite;  // pour gestion fuite
    double Volume_parent;
};

typedef struct avl_fuites{
    char* id;
    int equilibre;
    Arbre_liste* ptr; // pointeurs vers l'arbre de fuites
    struct avl_fuites* pGauche;
    struct avl_fuites* pDroit;
   
}AVL_FUITES;


Arbre_liste* creerArbreListe(const char* id, double volume, double fuite);
void ajouterEnfant(Arbre_liste* parent, Arbre_liste* enfant);
double propagation_fuites(Arbre_liste* a, double volume);
void libererArbreListe(Arbre_liste* a);

/* ================= PROTOTYPES AVL FUITES ================= */

AVL_FUITES* creerAVL_Fuites(const char* id, Arbre_liste* ptr);
AVL_FUITES* rotationGaucheAVL(AVL_FUITES* a);
AVL_FUITES* rotationDroiteAVL(AVL_FUITES* a);
AVL_FUITES* insertionAVL_Fuites(AVL_FUITES* a, Arbre_liste* ptr, const char* id, int* h);
Arbre_liste* rechercherAVL_Fuites(AVL_FUITES* a, const char* id);
double calcul_fuites_AVL(AVL_FUITES* a, const char* id);
void libererAVL_Fuites(AVL_FUITES* a);
void ecriture_fichier_leaks(char* id_usine, double volume);
int traitement_ligne_fuite(
    const char* buffer,
    char* parent,
    char* enfant,
    char* service,
    double* valeur,
    double* fuite,
    double* somme
);

#endif // LEAKS_H
