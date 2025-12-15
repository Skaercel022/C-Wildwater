#ifndef LEAKS_H
#define LEAKS_H



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


typedef enum{
    Parsing_OK,
    NB_COLONNES_INCORRECT,
    FORMAT_INCORRECT,
}Code_Error;

typedef struct _avl{
    char* id;
    Arbre_liste* parents; // liste des parents
    float Volume;
    float fuites;
    struct _avl* pGauche;
    struct _avl* pDroit;
    int equilibre;
}AVL_Fuite;

Liste* creerListe(void* ptr, char* id);
Arbre_liste* CreerArbre_liste(void* ptr, char* id);
Arbre_liste* insererArbre_liste(Arbre_liste* arbre, void* ptr, char* id);
Arbre_liste* creationAbre_liste(Arbre_liste* arbre, void* ptr, char* id);



#endif // LEAKS_H
