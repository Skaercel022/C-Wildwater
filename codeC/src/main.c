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
    else if (strcmp(argv[1], "leaks") == 0) {
    printf("Traitement des leaks '%s'...\n", argv[2]);

    char* id_usine = argv[2];
    AVL_FUITES* glossaire_fuites = NULL;
    int h = 0;
    char buffer[1024];
    char dash[8];
    char parent_name[256];
    char enfant_name[256];
    double somme = 0.0;
    double valeur = 0.0;
    double fuite = 0.0;

    while (fgets(buffer, sizeof(buffer), stdin)) {
        if (traitement_ligne_fuite(buffer, dash, parent_name, enfant_name, &valeur, &fuite, &somme)) {
            Arbre_liste* noeud_parent = NULL;
            Arbre_liste* noeud_enfant = NULL;

            // --- Traitement du parent ---
            if (strcmp(parent_name, "-") != 0) {
                noeud_parent = rechercherAVL_Fuites(glossaire_fuites, parent_name);

                if (!noeud_parent) {
                    // Création seulement si le nœud n'existe pas
                    noeud_parent = creerArbreListe(parent_name, valeur, fuite);
                    printf("DEBUG creerarbreliste\n");
                    h = 0;
                    glossaire_fuites = insertionAVL_Fuites(glossaire_fuites, noeud_parent, parent_name, &h);
                } else if (valeur > 0) {
                    // Mise à jour des valeurs existantes
                    noeud_parent->Volume_parent = valeur;
                    noeud_parent->coefficient_fuite = fuite;
                }
            }
            printf("DEBUG entre enfant et parent\n");
            // --- Traitement de l'enfant ---
            if (strcmp(enfant_name, "-") != 0) {
                noeud_enfant = rechercherAVL_Fuites(glossaire_fuites, enfant_name);

                if (!noeud_enfant) {
                    // Création seulement si le nœud n'existe pas
                    noeud_enfant = creerArbreListe(enfant_name, valeur, fuite);
                    printf("DEBUG creerarbreliste\n");
                    h = 0;
                    glossaire_fuites = insertionAVL_Fuites(glossaire_fuites, noeud_enfant, enfant_name, &h);
                } else if (valeur > 0) {
                    noeud_enfant->Volume_parent = valeur;
                    noeud_enfant->coefficient_fuite = fuite;
                }
            }

            // --- Liaison parent-enfant ---
            if (noeud_parent && noeud_enfant) {
                // Vérification doublon
                int exists = 0;
                Liste* liste_arbre = noeud_parent->liste;
                while (liste_arbre) {
                    printf("Je rentre dans le while\n");
                    if (liste_arbre->enfant == noeud_enfant){ 
                        exists = 1;
                        printf("Dans le if\n");
                        break;
                    }    
                    liste_arbre = liste_arbre->next;
                }
                if (!exists) {
                    printf("Debug ajouterenfant\n");
                    ajouterEnfant(noeud_parent, noeud_enfant);
                }
            }
        }
    }

    if(!glossaire_fuites){
        printf("DEBUG glossaire nul\n");
    }

    // --- Calcul des fuites ---
    Arbre_liste* usine_node = rechercherAVL_Fuites(glossaire_fuites, id_usine);
    if (!usine_node) {
        double total_fuites = -1;
        ecriture_fichier_leaks(id_usine, total_fuites);
    } else {
        usine_node->Volume_parent = somme;
        double total_fuites = calcul_fuites_AVL(glossaire_fuites, id_usine);
        ecriture_fichier_leaks(id_usine, total_fuites);
    }

    // --- Libération mémoire ---
    libererAVL_Fuites(glossaire_fuites);
    // Ne pas libérer usine_node individuellement, car déjà libéré par l'AVL
}

    
    else{
        exit(99);
    }


    return 0;

}
