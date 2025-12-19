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

    }
    else{
        exit(99);
    }


    return 0;
}