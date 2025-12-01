#!/bin/bash

#verifie qu'il y'ait au moins un argument
if [ $# -lt 1 ]; then
    echo "Erreur : aucune commande fournie."
    echo "4 Usage possibles :"
    echo "Usage 1 : $0 histo max"
    echo "Usage 2 : $0 histo src"
    echo "Usage 3 : $0 histo real"
    echo "Usage 4 : $0 leaks "ID_USINE""
    exit 1
fi

cmd="$1"

#verif commandes histo
if [ "$cmd" = "histo" ]; then
    if [ $# -ne 2 ]; then
        echo "Erreur : usage -> $0 histo max|src|real"
        exit 1
    fi

    mode="$2"

    if [ "$mode" != "max" ] && [ "$mode" != "src" ] && [ "$mode" != "real" ]; then
        echo "Erreur : mode histo invalide : $mode"
        echo "Modes valides : max | src | real"
        exit 1
    fi

#verif commandes leaks
elif [ "$cmd" = "leaks" ]; then

    if [ $# -ne 2 ]; then
        echo "Erreur : usage -> $0 leaks "ID_USINE""
        exit 1
    fi

    usine="$2"

#commandes inconnues
else
    echo "Erreur : commande inconnue '$cmd'"
    echo "Commandes valides : histo | leaks"
    exit 1
fi
