#ifndef AVL_H
#define AVL_H

typedef struct arbre Arbre;

typedef struct avl {
    char* val;
    struct avl* pDroit;
    struct avl* pGauche;
    int equilibre;
    Arbre* arbre;


}AVL;



AVL* rotationDroite(AVL* pAVL);
AVL* rotationGauche(AVL* pAVL);
AVL* doubleRotationDroite(AVL* pAVL);
AVL* doubleRotationGache(AVL* pAVL);
AVL* equilibrerAVL(AVL* pAVL);
AVL* creerAVL(char* charactere);
int existeGauche(AVL* pAVL);
int existDroit(AVL* pAVL);
AVL* insertionAVL(AVL* pAVL, char* id, int* h);
AVL* suppMinAVL(AVL* pAVL, char* pe, int *h);
AVL* suppressionAVL(AVL* pAVL, char* id, int* h);
Arbre* rechercheAVL(AVL* pAVL, char* id);

#endif // AVL_H