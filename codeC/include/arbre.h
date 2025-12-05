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