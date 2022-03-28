# Projet programmation orientée objet (SSV)

## REPONSES du groupe *86* (*Johann Clausen* & *Victor Galhaud*)

## Partie 1

**************************************************************
### /Q1.1\
Pour éviter la duplication de code, il faut mettre le clamping dans une méthode de la classe. 
Comme cette méthode ne sera appelée qu'à l'intérieur de la classe, il serait judicieux de la prototyper en private (détail d'implémentation).
**************************************************************
### /Q1.2\
En utilisant deux boucles imbriquées avec des indices de -1 à 1, on peut parcourir toutes les combinaisons de vecteurs de l'espace torique.
Il suffit de comparer la distance entre le nouveau vecteur et le vecteur max à chaque tour de boucle.
**************************************************************
### /Q1.3\
Il faut passer les "Collider" et les "Vec2d" en référence constante, car c'est des classes complexes.
Pour les "double", le passage par valeur suffit.
**************************************************************
### /Q1.4\
Les méthodes affichant la distance et la direction n'ont pas besoin de modifier les attributs privés.
Pour la fonction move, elle déplace le "Collider", et doit donc accéder à "center". Elle ne doit pas être déclarée comme const.
**************************************************************
### /Q1.5\
Il suffit de return les résultats des fonctions rédigées auparavant.
**************************************************************
### /Q1.6\
Pour tous les opérateurs (sauf <<), il faut utiliser la surcharge interne. Les opérateurs sont "proches de la classe", ils ont besoin d'accès internes (+= modifie l'objet). 
Pour l'opérateur d'affichage, il faut le faire en externe car on ne peut pas redéfinir la classe iostream, et que "Collider" n'est pas l'opérande de gauche.
**************************************************************
### /Q1.7\
Comme auparavant, il faut passer les classes ("Collider", "Vec2d") en référence constante, les types de base ("double") par valeur.
Notons qu'il est nécesssaire de passer le "ostream" par référence (non constante), car il s'agit d'un flot.
**************************************************************
### /Q1.8\
Les seules méthodes qui peuvent modifier l'instance de "Collider" devraient être l'opérateur "=" et "+=".
Toutes les autres méthodes peuvent être déclarées comme "const".
**************************************************************



## Partie 2

**************************************************************
### /Q2.1\
Le nombre de cellules totales dans le vector est nb_cells * nb_cells.
En effet, il ne s'agit pas de world_size au carré, car cette valeur sera utilisée par SFML pour afficher nos cellules de largeur cell_size.
**************************************************************
### /Q2.2\
world_cells : "simulation", "world", "default cells" <br>
world_size : "simulation", "world", "default size"
**************************************************************
### /Q2.3\
Il faut d'abord appeler reloadConfig(), qui s'occupera des arguments nb_cells, cell_size, et du tableau cells_.
Ensuite, reloadCacheStructure() utilisera les nouvelles valeurs des attributs pour initialiser les vertexes et renderingCache_.
Enfin, updateCache(), afin d'appliquer les changements dans renderingCache_.
**************************************************************
### /Q2.4\
Le changement de fichier source pour le terrain peut se faire sans recompiler le programme en entier.
On peut, à l'aide de paramètres de ligne de commande, modifier le fichier désiré pour le chargement (info retrouvée à l'aide de getApp() et getAppConfig())
**************************************************************
### /Q2.5\
Elle doit appeler les fonctions : reloadCacheStructure() et updateCache().
Ces appels permettront d'initialiser les vertexes et renderingCache_, ainsi que de d'actualiser les changements.
**************************************************************
### /Q2.6\ 
Nous avons choisi de représenter l'ensemble "seeds_" par un vector de Seeds.
Nous avons privilégié le type vector au type array car nous ne connaissons pas la taille de cet ensemble au moment où nous l'initialisons.



**************************************************************



## Partie 3
