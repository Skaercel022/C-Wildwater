#ifndef ARBRE_H
#define ARBRE_H


//liste chainée
typedef struct liste_arbre{
    char* id;
    struct liste_arbre* pSuivant;
}Liste_arbre;

//arbre
typedef struct {
    double V_amont;
    float fuite_amont;
    Liste_arbre* enfant;
}Arbre;

//liste arbre
Liste_arbre* creerListeArbre(char* id);
//void ajouterEnfant(Arbre* arbre, char* id_enfant);
void suppListeArbre(Liste_arbre* liste);

//arbre
Arbre* creerArbre(double volume, float fuite);
void suppNoeud(Arbre* arbre);


#endif // ARBRE_H