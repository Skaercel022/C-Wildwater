#ifndef AVL_USINE_H
#define AVL_USINE_H


typedef struct avl_usine {
    char* val;
    struct avl_usine* pDroit;
    struct avl_usine* pGauche;
    int equilibre;
    int max;
    int capte;
    int traite;
}AVL_USINE;


AVL_USINE* rotationDroite(AVL_USINE* pAVL_USINE);
AVL_USINE* rotationGauche(AVL_USINE* pAVL_USINE);
AVL_USINE* doubleRotationDroite(AVL_USINE* pAVL_USINE);
AVL_USINE* doubleRotationGache(AVL_USINE* pAVL_USINE);
AVL_USINE* equilibrerAVL_USINE(AVL_USINE* pAVL_USINE);
AVL_USINE* creerAVL_USINE(char* charactere);
int existeGauche(AVL_USINE* pAVL_USINE);
int existDroit(AVL_USINE* pAVL_USINE);
AVL_USINE* insertionAVL_USINE(AVL_USINE* pAVL_USINE, char* id, int* h);
AVL_USINE* suppMinAVL_USINE(AVL_USINE* pAVL_USINE, char** pe, int *h);
AVL_USINE* suppressionAVL_USINE(AVL_USINE* pAVL_USINE, char* id, int* h);
AVL_USINE* rechercheAdresse(AVL_USINE* pAVL_USINE, char* id);
int rechercheAVL_USINE(AVL_USINE* pAVL_USINE, char* id);

#endif // AVL_USINE_H