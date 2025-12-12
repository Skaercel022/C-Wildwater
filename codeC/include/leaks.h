#ifndef LEAKS_H
#define LEAKS_H



typedef struct liste{
    void* ptr; // char* conserve l'adresse d'un autre Arbre_liste 
    struct liste* next;
}Liste;

typedef struct {
    Liste* liste;
    char* Suivant;
    int nb_fuite;
}Arbre_liste;



#endif // LEAKS_H
