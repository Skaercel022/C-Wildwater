
#!/bin/bash


# Vérification fichier
if [ ! -f "$fichier" ]; then 
    echo "Erreur : fichier introuvable : $fichier"
    exit 1
fi  

case "$mode" in  

    max)
        echo "Usine;TraitementMax;Source;Prelevement" > vol_max.dat
        val=$(grep -E '^-;[^-;]+;' | cut -d ';' -f2,4,5 )
        echo "$val" > valmax.dat
        echo "Fichiers généré : vol_max.dat 
        ;;

    source)
		echo "Usine;TraitementMax;Source;Prelevement" > source.csv
        val=$(grep -E '^-;[^-;]+;' | cut -d ';' -f2,4,5 )
        echo "$val" > source.csv
        echo "Fichier généré : source.csv"
        ;;

    prelevement)
		echo "Usine;TraitementMax;Source;Prelevement" > prelevement.csv
        val=$(grep -E '^-;[^-;]+;' | cut -d ';' -f2,4,5 )
        echo "$val" > prelevement.csv
        echo "Fichier généré : prelevement.csv"
        ;;

    *)
        echo "Mode inconnu : $mode"
        echo "Modes disponibles : max / source / prelevement"
        exit 1
        ;;
esac






"if [ "$mode" = "max" ]; then
    echo "Usine,traitementmax">>vol_max.dat
    val=$( grep -E '^-;[^-;]+;-;' c-wildwater_v0.dat | cut -d ';' -f2,4)
    echo "$val" >>valmax.csv	
    return valmax.csv  # Retourne un code de sortie
fi
    
if [ "$mode" = "src" ]; then
	echo "Usine,prelevement">> vol_capation.txt"

if [ ! -f "$fichier" ] ; then 
	echo "Erreur fichier introuvable:$fichier"
	exit 1
 fi  
fichier="$1"
case "$mode" in  
	max)
		echo "Usine,traitementmax,source,prelevement,">>vol_max.dat
   	 	val=$( grep -E '^-;[^-;]+;-;' $ficher | cut -d ';' -f2,4)
    		echo "$val" >>valmax.cs
    		;;
    	src)
    		echo 
grep -E '^-;[^-;]+;' c-wildwater_v0.dat | cut -d ';' -f2,4,5



