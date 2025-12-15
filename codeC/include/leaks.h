#ifndef LEAKS_H
#define LEAKS_H

#define LONGUEUR_ID 64

// Déclarations anticipées
typedef struct liste Liste;
typedef struct arbre_liste Arbre_liste;

// Définition des structures
struct liste {
    void* ptr;            // ok
    char* id;
    Liste* next;
    Arbre_liste* enfant;  // pointeur vers un Arbre_liste
};

struct arbre_liste {
    Liste* liste;
    int nb_fuite;
    float coefficient;  // pour gestion fuite
};

typedef struct avl_fuites{
    char* id;
    float fuite;
    struct avl_fuites* pGauche;
    struct avl_fuites* pDroit;
    int equilibre;
    Arbre_liste* ptr; // pointeurs vers l'arbre de fuites
}AVL_FUITES;


typedef struct {
    char id_usine[LONGUEUR_ID];
    char id_amont[LONGUEUR_ID];
    char id_aval[LONGUEUR_ID];
    double Volume;
    double fuite;
}LignesCSV;

typedef enum {
    Parsing_OK,
    Erreur_Allocation,
    Erreur_Format_Token,
    Erreur_Champ_Vide,
    Erreur_NB_colonnes,
}Code_Erreur;


Code_Erreur SegmentationLigneCSV(const char* ligne, LignesCSV* resultat);
AVL_FUITES* InsertionAVL(AVL_FUITES* racine, LignesCSV* ligne, int* h);
LignesCSV creerLigneCSV();
AVL_FUITES* constructeurAVL(LignesCSV* ligne);
Arbre_liste* recherche(AVL_FUITES* racine, char* id);





#endif // LEAKS_H
