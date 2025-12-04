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
