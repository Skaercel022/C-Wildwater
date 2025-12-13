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


#

#!/bin/bash





# Vérification fichier

fichier="$1"
cmd="$2"
mode="$3"



if [ ! -f "$fichier" ]; then 
    echo "Erreur : fichier introuvable : $fichier"
    exit 1
fi  
case "$cmd" in
	h	histo)
		case "$mode" in  
		
    			max)
        			echo "Usine;TraitementMax" > vol_max.dat
        			val=$( grep -E '^-;[^-;]+;-;' c-wildwater_v0.dat | cut -d ';' -f2,4)
    				echo "$val" > vol_max.dat
        			echo "Fichiers généré : vol_max.dat"
					input_files="vol_max.dat"
        			;;
    			src)
				    echo "Usine/source;TraitementMax;Fuite" > source.csv
        			val=$(grep -E '^-;[^-;]+;' $fichier | cut -d ';' -f2,4,5 )
        			echo "$val" > source.csv
        			echo "Fichier généré : source.csv"
					input_files="source.csv"
       				;;
    			prelevement)
				    echo "Usine/source;TraitementMax;Fuite" > prelevement.csv
        			val=$(grep -E '^-;[^-;]+;' $fichier | cut -d ';' -f2,4,5 )
        			echo "$val" > prelevement.csv
        			echo "Fichier généré : prelevement.csv"
					input_files="prelevement.csv"
        			;;

    			*)
        			echo "Mode inconnu : $mode"
        			echo "Modes disponibles : max / source / prelevement"
    				exit 1
        			;;

		esac

	

		esac



	 	Compilation et appel du programme C 

        if [ -f Makefile ]; then
            echo "Compilation du programme C..."
            make
        else
            echo "Makefile introuvable. erreur"
			exit(1)
        fi

		
		if [ ! -x "./run" ]; then
            echo "Erreur : l'exécutable du programme C n'existe pas."
            exit 1
        fi

        echo "Exécution du programme C pour le mode $mode..."
        ./run "$mode" $input_files
        echo "Traitement terminé pour le mode $mode."
        ;;

	

	leaks)
		echo "Identifiant;TraitementMax;fuite" > fuite.csv
		val=$(grep -E "ID_USINE" $fichier | cut -d ';' -f2,4,5 )
		echo "$val">fuite.csv

		echo "Fichier généré: fuite.csv"

		;;

	*)

	echo "commande inconnu : $cmd"

    	echo "commandes disponibles : histo / leaks"

    	exit 1

	;;

esac



   


















