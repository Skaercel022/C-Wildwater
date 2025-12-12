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
    int coefficient;  // pour gestion fuite
};

typedef struct avl_usine {
    char* val;
    struct avl_usine* pDroit;
    struct avl_usine* pGauche;
    int equilibre;
    int max;
    int capte;
    int traite;
}AVL_USINE;


Liste* creerListe(void* ptr, char* id);
Arbre_liste* CreerArbre_liste(void* ptr, char* id);
Arbre_liste* insererArbre_liste(Arbre_liste* arbre, void* ptr, char* id);
Arbre_liste* creationAbre_liste(Arbre_liste* arbre, void* ptr, char* id);



#endif // LEAKS_H
