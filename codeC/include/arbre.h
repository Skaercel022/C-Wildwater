//liste chainée
typedef struct liste{
    char* id;
    struct liste* pSuivant;
}Liste;

//arbre
typedef struct {
    double V_amont;
    float fuite_amont;
    Liste* enfant;
}Arbre;