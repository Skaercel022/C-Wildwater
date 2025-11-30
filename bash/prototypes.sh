# liste des prototypes utiles (fichier temporaire)

#comparaison de nombre
if [ "$a" -eq "$b" ]; then ... fi    # Égal
if [ "$a" -ne "$b" ]; then ... fi    # Différent
if [ "$a" -lt "$b" ]; then ... fi    # Inférieur
if [ "$a" -le "$b" ]; then ... fi    # Inférieur ou égal
if [ "$a" -gt "$b" ]; then ... fi    # Supérieur
if [ "$a" -ge "$b" ]; then ... fi    # Supérieur ou égal

#comparaison de chaine
if [ "$str1" = "$str2" ]; then ... fi      # Égal (attention aux espaces)
if [ "$str1" != "$str2" ]; then ... fi     # Différent
if [ -z "$str" ]; then ... fi              # Chaîne vide
if [ -n "$str" ]; then ... fi              # Chaîne non vide

#comparaison de fichier
if [ -e "$fichier" ]; then ... fi    # Existe
if [ -f "$fichier" ]; then ... fi    # Est un fichier régulier
if [ -d "$fichier" ]; then ... fi    # Est un dossier
if [ -r "$fichier" ]; then ... fi    # Lisible
if [ -w "$fichier" ]; then ... fi    # Inscrivible
if [ -x "$fichier" ]; then ... fi    # Exécutable

#récupération des args
echo "Premier argument : $1"
echo "Deuxième argument : $2"
echo "Tous les arguments : $@"
echo "Nombre d'arguments : $#"
echo "Nom du script : $0"

#fonction
#syntaxe de base 
ma_fonction() {
    echo "Appel de ma_fonction"
    return 0  # Retourne un code de sortie
}

#avec argument
saluer() {
    echo "Bonjour, $1 !"
}
saluer "Alice"  # Appel avec argument

#retourner des valeurs
calculer() {
    local resultat=$(( $1 + $2 ))
    echo "$resultat"  # Retourne la valeur via echo
}
somme=$(calculer 5 3)  # Récupère le retour

#boucle
#for 
for i in {1..5}; do
    echo "Itération $i"
done

#while
compteur=0
while [ $compteur -lt 5 ]; do
    echo "Compteur : $compteur"
    ((compteur++))
done

#until (nouveau)
compteur=0
until [ $compteur -eq 5 ]; do
    echo "Compteur : $compteur"
    ((compteur++))
done

#tableaux
mon_tableau=("valeur1" "valeur2" "valeur3")
echo "${mon_tableau[0]}"  # Accès au premier élément
echo "${mon_tableau[@]}" # Tous les éléments
echo "${#mon_tableau[@]}" # Taille du tableau

#gestion des erreurs
#">" sortie standard (stdout) (écrase le contenu)
#"2>" sortie d'erreur (stderr)
#">>" ajoute à la fin d'un fichier
#"&>" redirige erreur + contenu