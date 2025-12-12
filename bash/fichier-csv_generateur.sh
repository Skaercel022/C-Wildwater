
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
	histo)
		case "$mode" in  

    			max)
        			echo "Usine;TraitementMax" > vol_max.dat
        			val=$( grep -E '^-;[^-;]+;-;' c-wildwater_v0.dat | cut -d ';' -f2,4)
    				echo "$val" > vol_max.dat
        			echo "Fichiers généré : vol_max.dat" 
        			;;

    			src)
				echo "Usine/source;TraitementMax;Fuite" > source.csv
        			val=$(grep -E '^-;[^-;]+;' $fichier | cut -d ';' -f2,4,5 )
        			echo "$val" > source.csv
        			echo "Fichier généré : source.csv"
       				;;

    			prelevement)
				echo "Usine/source;TraitementMax;Fuite" > prelevement.csv
        			val=$(grep -E '^-;[^-;]+;' $fichier | cut -d ';' -f2,4,5 )
        			echo "$val" > prelevement.csv
        			echo "Fichier généré : prelevement.csv"
        			;;

    			*)
        			echo "Mode inconnu : $mode"
        			echo "Modes disponibles : max / source / prelevement"
    				exit 1
        			;;
		esac
	
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
	










