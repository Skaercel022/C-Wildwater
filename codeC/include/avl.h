typedef struct avl {
    int val;
    struct avl* pDroit;
    struct avl* pGauche;
    int equilibre;
}AVL;

AVL* rotationDroite(AVL* pAVL);
