#!/bin/bash

start_time=$(date +%s%3N)

erreur_sortie() {
    echo "Erreur: $1" >&2
    end_time=$(date +%s%3N)
    echo "Durée totale: $((end_time - start_time)) ms"
    exit 1
}

#verif arg
if [ "$#" -ne 3 ]; then
    erreur_sortie "Usage 1 : $0 <fichier_donnees> histo <max|src|real>
        Usage 2 : $0 <fichier_donnees> leaks <ID_Usine>"
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


{
    grep -E "^-;[^-;]+;-;" "$DATA_FILE"
    grep -E "^-;[^;]*;[^-;]*;[^-;]*;[^;]*" "$DATA_FILE"
} > "$TMP_FILE"

if [ ! -s "$TMP_FILE" ]; then
    erreur_sortie "Aucune donnée valide après filtrage"
fi

#execution programme
./codeC/run histo "$TYPE" "$TMP_FILE"
RET=$?

rm -f "$TMP_FILE"

if [ "$RET" -ne 0 ]; then
    cd codeC || erreur_sortie "Impossible d'entrer dans codeC pour clean"
    make clean > /dev/null 2>&1
    cd - > /dev/null || true
    erreur_sortie "Le programme C a retourné une erreur"
fi

cd codeC || erreur_sortie "Impossible d'entrer dans codeC pour clean"
make clean > /dev/null 2>&1
cd - > /dev/null || true

case "$TYPE" in
    max)
        FILE_SMALL="vol_max_50_petites.csv"
        FILE_BIG="vol_max_10_grandes.csv"
        COL=2
        TITLE="Capacité maximale de traitement"
        ;;
    src)
        FILE_SMALL="vol_capte_50_petites.csv"
        FILE_BIG="vol_capte_10_grandes.csv"
        COL=3
        TITLE="Volume total capté"
        ;;
    real)
        FILE_SMALL="vol_traite_50_petites.csv"
        FILE_BIG="vol_traite_10_grandes.csv"
        COL=4
        TITLE="Volume réellement traité"
        ;;
esac

gnuplot << EOF
set terminal pngcairo size 1600,800
set output "${FILE_SMALL%.csv}.png"

set datafile separator ";"

set style fill solid 1.0
set style line 1 lc rgb "#1f77b4"
set boxwidth 0.9

set yrange [0:*]
set grid ytics
set key off

set title "${TITLE} – 50 plus petites usines"
set xlabel "Usines"
set ylabel "Volume (M.m³ / an)"

set xtics rotate by -45 font ",8"

plot "${FILE_SMALL}" using 0:(\$${COL}/1000):xtic(1) every ::1 \
     with boxes ls 1
EOF

gnuplot << EOF
set terminal pngcairo size 1400,800
set output "${FILE_BIG%.csv}.png"

set datafile separator ";"

set style fill solid 1.0
set style line 1 lc rgb "#1f77b4"
set boxwidth 0.9

set yrange [0:*]
set grid ytics
set key off

set title "${TITLE} – 10 plus grandes usines"
set xlabel "Usines"
set ylabel "Volume (M.m³ / an)"

set xtics rotate by -30 font ",10"

plot "${FILE_BIG}" using 0:(\$${COL}/1000):xtic(1) every ::1 \
     with boxes ls 1
EOF

#fin
end_time=$(date +%s%3N)
echo "Traitement terminé avec succès"
echo "Durée totale: $((end_time - start_time)) ms"

exit 0