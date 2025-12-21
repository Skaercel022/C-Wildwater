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


// --- Gestion des lignes CSV ---
int LireetParser(char* id_usine, char* id_amont, char* id_aval, double* Volume, double*  fuite, char* buffer);

//  Gestion de l'Arbre Physique 
Arbre_liste* constructeurArbre(char* id, double fuite);
Liste* constructeurListe(Arbre_liste* enfant);
void ajouter_enfant(Arbre_liste* parent, Arbre_liste* enfant);
void ajouterVolumeArbre(double fuite, double Volume, Arbre_liste* racine);
void ajouterNoeudArbre(AVL_FUITES** racine_AVL, char* id_amont, char* id_aval, double volume, double fuite);

// Gestion de l'Index AVL
AVL_FUITES* constructeurAVL(Arbre_liste* Noeud);
AVL_FUITES* InsertionAVL(AVL_FUITES* racine, Arbre_liste* Noeud, int* h);

// Recherche : Utilisation de 'const' pour accepter les IDs en lecture seule
Arbre_liste* rechercheArbre(AVL_FUITES* racine, char* id);

// Calculs 
double calculer_fuites_rec(Arbre_liste* noeud, double volume_entrant);
double calculer_fuites(AVL_FUITES* racine_AVL, char* id_usine);

// Rotations AVL
AVL_FUITES* RotationDroite_FUITES(AVL_FUITES* racine);
AVL_FUITES* RotationGauche_FUITES(AVL_FUITES* racine);
AVL_FUITES* doubleRotationGauche_FUITES(AVL_FUITES* racine);
AVL_FUITES* doubleRotationDroite_FUITES(AVL_FUITES* racine);

// Libération mémoire
void liberer_arbre_physique(Arbre_liste* noeud);
void suppression_AVL_FUITES(AVL_FUITES* racine);
int ecriture_fichier(char* id, double volume);

#endif // LEAKS_H
