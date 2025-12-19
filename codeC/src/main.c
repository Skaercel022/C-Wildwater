#include "include.h"


int main(int argc, char** argv) {
    
    if (argc != 4){
        exit(99);
    }
        
    if (strcmp(argv[1],"histo") == 0){
        FILE* fichier = ouvrirFichierEcriture(argv[3]);
        AVL_USINE* racine = NULL;
        lectureFichierVersAVL(fichier, &racine);
        fermerFichier(fichier);

        if(strcmp(argv[2],"max") == 0){
            creerFichiersVMax(racine);
        }
        else if(strcmp(argv[2],"src") == 0){
            creerFichiersVCapte(racine);
        }
        else if(strcmp(argv[2],"real") == 0){
            creerFichiersVTraite(racine);
        }
        else{
            suppressionCompleteAVL_USINE(racine);
            exit(99);
        }
        suppressionCompleteAVL_USINE(racine);
    }
    else if(strcmp(argv[1],"leaks") == 0){
        const char* id_recherche = argv[2];
        const char* nom_fichier = argv[3];

        FILE* fp = fopen(nom_fichier, "r");
        if (fp == NULL) exit(101);

        AVL_FUITES* index_avl = NULL;
        Arbre_liste* racine_physique = NULL;
        LignesCSV* ligne_temp = creerLigneCSV();

        // Construction
        while (lireEtParserLigne(fp, ligne_temp) == Parsing_OK) {
            ajouterNoeudArbre(&index_avl, ligne_temp, &racine_physique);
        }
        double pertes = calculer_fuites(index_avl, id_recherche);
        if (pertes < -0.5) { // Si calculer_fuites a retourné -1
            printf("%s;-1\n", id_recherche);
        } else {
            // Conversion de milliers de m3 vers millions de m3
            printf("%s;%.3f\n", id_recherche, pertes / 1000.0);
        }
        liberer_arbre_physique(racine_physique);

        suppression_AVL_FUITES(index_avl);
        free(ligne_temp);
    }
    else{
        exit(99);
    }


    return 0;
}