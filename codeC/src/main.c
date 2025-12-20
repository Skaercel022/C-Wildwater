#include "include.h"


int main(int argc, char** argv) {
    
    if (strcmp(argv[1],"histo") == 0){
        if (argc != 4){
            exit(99);
        }
        FILE* fichier = ouvrirFichier(argv[3]);
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
        else if(strcmp(argv[2],"all") ==0){
            creerFichiersAll(racine);
       }
        else{
            suppressionCompleteAVL_USINE(racine);
            exit(99);
        }
        suppressionCompleteAVL_USINE(racine);
    }
    else if(strcmp(argv[1],"leaks") == 0){
        if(argc != 3){
            exit(99);
        }
        char* id_recherche = strdup(argv[2]);
        printf("DEBUG: Je recherche l'ID exact suivant : [%s]\n", id_recherche);


        AVL_FUITES* index_avl = NULL;
        Arbre_liste* racine_physique = NULL;
        char id_usine[256];
        char id_amont[256];
        char id_aval[256];
        double volume;
        double fuite;  

        // Construction
        while (LireetParser(id_usine, id_amont, id_aval, &volume, &fuite)) {
            ajouterNoeudArbre(&index_avl, &racine_physique, id_amont, id_aval, volume, fuite);
        }
        Arbre_liste* noeud_depart= rechercheArbre(index_avl, id_recherche);
        if (noeud_depart == NULL) {
            printf("%s;-1\n", id_recherche);
        } else {
            double pertes = calculer_fuites_rec(noeud_depart, noeud_depart->Volume_parent);
            // Conversion de milliers de m3 vers millions de m3
            printf("%s;%.3f\n", id_recherche, pertes / 1000.0);
        }

        liberer_arbre_physique(racine_physique);
        suppression_AVL_FUITES(index_avl);
    }
    
    else{
        exit(99);
    }


    return 0;

}
