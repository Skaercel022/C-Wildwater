if [ "$mode" = "max" ]; then
    echo "Usine,traitementmax">>vol_max.dat
    val=$( grep -E '^-;[^-;]+;-;' c-wildwater_v0.dat | cut -d ';' -f2,4)
    echo "$val" >>valmax.csv	
    return valmax.csv  # Retourne un code de sortie
fi
    
if [ "$mode" = "src" ]; then
	echo "Usine,prelevement">> vol_capation.txt"
