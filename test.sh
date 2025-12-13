#!/bin/bash

START_TIME=$(date +%s%3N)

erreur_sortie() {
    echo "Erreur: $1" >&2
    END_TIME=$(date +%s%3N)
    echo "Durée totale: $((END_TIME - START_TIME)) ms"
    exit 1
}

#verif arg
if [ "$#" -ne 3 ]; then
    erreur_sortie "Usage 1 : $0 <fichier_donnees> histo <max|src|real>"
fi

DATA_FILE="$1"
MODE="$2"
TYPE="$3"

if [ ! -f "$DATA_FILE" ]; then
    erreur_sortie "Fichier de données introuvable"
fi

if [ "$MODE" != "histo" ]; then
    erreur_sortie "Mode invalide (attendu: histo)"
fi

case "$TYPE" in
    max|src|real)
        ;;
    *)
        erreur_sortie "Type histogramme invalide (max | src | real)"
        ;;
esac

#compilation
if [ ! -d "codeC" ]; then
    erreur_sortie "Dossier codeC introuvable"
fi

cd codeC || erreur_sortie "Impossible d'entrer dans codeC"

if [ ! -x "./run" ]; then
    echo "Compilation du programme C..."
    make || erreur_sortie "Échec de la compilation"
fi

cd - > /dev/null || erreur_sortie "Erreur retour dossier"

#filtrage données avec grep
TMP_FILE="/tmp/cwildwater_filtered_$$.csv"

grep -E "^-;[^;]+;[^-;]+;[^-;]+;[^;]+" "$DATA_FILE" > "$TMP_FILE"

if [ ! -s "$TMP_FILE" ]; then
    erreur_sortie "Aucune donnée valide après filtrage"
fi

#execution programme
./codeC/run histo "$TYPE" "$TMP_FILE"
RET=$?

rm -f "$TMP_FILE"

if [ "$RET" -ne 0 ]; then
    erreur_sortie "Le programme C a retourné une erreur"
fi

cd codeC || erreur_sortie "Impossible d'entrer dans codeC pour clean"
make clean > /dev/null 2>&1
cd - > /dev/null || true

#fin
END_TIME=$(date +%s%3N)
echo "Traitement terminé avec succès"
echo "Durée totale: $((END_TIME - START_TIME)) ms"

exit 0