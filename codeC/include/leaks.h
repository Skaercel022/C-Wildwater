#ifndef LEAKS_H
#define LEAKS_H



typedef struct liste{
    void* ptr; // char* conserve l'Id
    struct liste* next;
}Liste;

typedef struct {
    Liste* liste;
    char* Suivant;
    int nb_fuite;
}Arbre;



#endif // LEAKS_H
