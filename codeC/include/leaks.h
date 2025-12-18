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
    float coefficient_parent;  // pour gestion fuite
    double Volume_parent;
};

typedef struct avl_fuites{
    char* id;
    double fuite;
    int equilibre;
    Arbre_liste* ptr; // pointeurs vers l'arbre de fuites
    struct avl_fuites* pGauche;
    struct avl_fuites* pDroit;
   
}AVL_FUITES;


typedef struct {
    char id[LONGUEUR_ID];
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
    Erreur_Pointeur_Nul,
}Code_Erreur;

LignesCSV* creerLigneCSV();
Arbre_liste* constructeurArbre(LignesCSV* ligne);
Liste* constructeurListe(Arbre_liste* enfant);
AVL_FUITES* constructeurAVL(LignesCSV* ligne);
Code_Erreur lireEtParserLigne(FILE* fichier, LignesCSV* resultat);
AVL_FUITES* InsertionAVL(AVL_FUITES* racine, LignesCSV* ligne, int* h);
AVL_FUITES* constructeurAVL(LignesCSV* ligne);
Arbre_liste* rechercheArbre(AVL_FUITES* racine, char* id);





#endif // LEAKS_H
