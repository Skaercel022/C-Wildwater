#include "include.h"
//objectif du main : 2 cas : histo ou leaks
//histo : ouvre un fichier, cree l'avl_usine, cree les fichiers de sortie


int main(){
    FILE* fichier = ouvrirFichierEcriture("c-wildwater_v3.dat");
    AVL_USINE* racine = NULL;
    lectureFichierVersAVL(fichier, &racine);
    fermerFichier(fichier);

    creerFichiersVMax(racine);
    //creerFichiersVTraite(racine);
    //creerFichiersVCapte(racine);

    suppressionCompleteAVL_USINE(racine);

    return 0;
}