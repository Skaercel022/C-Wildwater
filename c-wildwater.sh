#!/bin/bash

t_initial=$(date +%s%3N)


erreur_sortie() {
    echo "Erreur: $1" >&2
    t_final=$(date +%s%3N)
    echo "Durée totale: $((t_final - t_initial)) ms"
    exit 1
}

exit_c() {
    case "$1" in
        99) echo "Erreur : arguments invalides" ;;
        101) echo "Erreur : ouverture fichier" ;;
        102) echo "Erreur : fermeture fichier" ;;
        103) echo "Erreur : ligne csv mal formée" ;;
        104) echo "Erreur : allocation mémoire" ;;
    esac
    t_final=$(date +%s%3N)
    echo "Durée totale: $((t_final - t_initial)) ms"
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

if [[ "$MODE" != "histo" ]] && [[ "$MODE" != "leaks" ]]; then
    erreur_sortie "Mode invalide (attendu: histo ou leaks)"
fi

if [[ "$MODE" = "histo" ]]; then
    case "$TYPE" in
    max|src|real)
        ;;
    *)
        erreur_sortie "Type histogramme invalide (max | src | real)"
        ;;
    esac
fi

if [ ! -d "codeC" ]; then
    erreur_sortie "Dossier codeC introuvable"
fi

cd codeC || erreur_sortie "Impossible d'entrer dans codeC"

if [ ! -x "./run" ]; then
    echo "Compilation du programme C..."
    make || erreur_sortie "Échec de la compilation"
fi

cd - > /dev/null || erreur_sortie "Erreur retour dossier"

TMP_FILE="/tmp/cwildwater_filtered_$$.csv"


if [ "$MODE" = "histo" ]; then
    {
    grep -E "^-;[^-;]+;-;" "$DATA_FILE"
    grep -E "^-;[^;]*;[^-;]*;[^-;]*;[^;]*" "$DATA_FILE"
    } > "$TMP_FILE"

    if [ ! -s "$TMP_FILE" ]; then
        erreur_sortie "Aucune donnée valide après filtrage"
    fi

    ./codeC/run histo "$TYPE" "$TMP_FILE"
    RET=$?

    rm -f "$TMP_FILE"

    if [ "$RET" -ne 0 ]; then
        cd codeC || erreur_sortie "Impossible d'entrer dans codeC pour clean"
        make clean > /dev/null 2>&1
        cd - > /dev/null || true
        exit_c $RET
    fi

    cd codeC || erreur_sortie "Impossible d'entrer dans codeC pour clean"
    make clean > /dev/null 2>&1
    cd - > /dev/null || true

    case "$TYPE" in
        max)
            FILE_50PETITES="vol_max_50_petites.csv"
            FILE_10GRANDES="vol_max_10_grandes.csv"
            COL=2
            TITRE="Capacité maximale de traitement"
            ;;
        src)
            FILE_50PETITES="vol_capte_50_petites.csv"
            FILE_10GRANDES="vol_capte_10_grandes.csv"
            COL=3
            TITRE="Volume total capté"
            ;;
        real)
            FILE_50PETITES="vol_traite_50_petites.csv"
            FILE_10GRANDES="vol_traite_10_grandes.csv"
            COL=4
            TITRE="Volume réellement traité"
            ;;
    esac

    gnuplot << EOF
    set terminal pngcairo size 1600,800
    set output "${FILE_50PETITES%.csv}.png"

    set datafile separator ";"

    set style fill solid 1.0
    set style line 1 lc rgb "#1f77b4"
    set boxwidth 0.9

    set yrange [0:*]
    set grid ytics
    set key off

    set title "${TITRE} – 50 plus petites usines"
    set xlabel "Usines"
    set ylabel "Volume (M.m³ / an)"

    set xtics rotate by -45 font ",8"

    plot "${FILE_50PETITES}" using 0:(\$${COL}/1000):xtic(1) every ::1 \
        with boxes ls 1
EOF

    gnuplot << EOF
    set terminal pngcairo size 1400,800
    set output "${FILE_10GRANDES%.csv}.png"

    set datafile separator ";"

    set style fill solid 1.0
    set style line 1 lc rgb "#1f77b4"
    set boxwidth 0.9

    set yrange [0:*]
    set grid ytics
    set key off

    set title "${TITRE} – 10 plus grandes usines"
    set xlabel "Usines"
    set ylabel "Volume (M.m³ / an)"

    set xtics rotate by -30 font ",10"

    plot "${FILE_10GRANDES}" using 0:(\$${COL}/1000):xtic(1) every ::1 \
        with boxes ls 1
EOF
fi

if [ "$MODE" = "leaks" ]; then
    awk -F';' -v type="$TYPE" '$1 == type || $2 == type' "$DATA_FILE" > "$TMP_FILE"
    
    if [ ! -s "$TMP_FILE" ]; then
        erreur_sortie "L'usine avec l'ID spécifiée n'a pas été trouvée"
    fi

    ./codeC/run leaks "$TYPE" "$TMP_FILE"
    RET=$?

    rm -f "$TMP_FILE"

    if [ "$RET" -ne 0 ]; then
        cd codeC || erreur_sortie "Impossible d'entrer dans codeC pour clean"
        make clean > /dev/null 2>&1
        cd - > /dev/null || true
        exit_c $RET
    fi

    cd codeC || erreur_sortie "Impossible d'entrer dans codeC pour clean"
    make clean > /dev/null 2>&1
    cd - > /dev/null || true


fi

#fin
t_final=$(date +%s%3N)
echo "Traitement terminé avec succès"
echo "Durée totale: $((t_final - t_initial)) ms"

exit 0