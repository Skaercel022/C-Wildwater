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

