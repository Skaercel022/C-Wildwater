#ifndef histo_h
#define histo_h

FILE* ouvrirFichierEcriture(char* nom_fichier);
void fermerFichier(FILE* fichier);
int estVide(const char *s);
char* lireLigne(FILE* fichier);
void lectureFichierVersAVL(FILE* fichier, AVL_USINE** racine);

void creerFichiersVMax(AVL_USINE* racine);
void creerFichiersVTraite(AVL_USINE* racine);
void creerFichiersVCapte(AVL_USINE* racine);

void fichier10PlusGrandesMax(AVL_USINE* racine, char* nomFichierSortie);
void fichier50PlusPetitesMax(AVL_USINE* racine, const char* nomFichierSortie);
void fichier10PlusGrandesTraite(AVL_USINE* racine, const char* nomFichierSortie);
void fichier50PlusPetitesTraite(AVL_USINE* racine, const char* nomFichierSortie);
void fichier10PlusGrandesCapte(AVL_USINE* racine, const char* nomFichierSortie);
void fichier50PlusPetitesCapte(AVL_USINE* racine, const char* nomFichierSortie);


#endif // histo_h