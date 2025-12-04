typedef struct avl {
    int val;
    struct avl* pDroit;
    struct avl* pGauche;
    int equilibre;
}AVL;

AVL* rotationDroite(AVL* pAVL);
AVL* rotationGauche(AVL* pAVL);
AVL* doubleRotationDroite(AVL* pAVL);
AVL* doubleRotationGache(AVL* pAVL);
AVL* equilibrerAVL(AVL* pAVL);
AVL* creerAVL(int value);
int existeGauche(AVL* pAVL);
int existDroit(AVL* pAVL);
AVL* insertionAVL(AVL* pAVL, int e, int* h);
AVL* suppMinAVL(AVL* pAVL, int* pe, int *h);
AVL* suppressionAVL(AVL* pAVL, int e, int* h);
