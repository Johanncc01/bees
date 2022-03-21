#Projet programmation orientée objet (SSV)

##REPONSES du groupe *No groupe* (*Johann Clausen* & *Victor Galhaud*)


**************************************************************
## Q1.1
Pour éviter la duplication de code, il faut mettre le clamping dans une méthode de la classe. 
Comme cette méthode ne sera appelée qu'à l'intérieur de la classe, il serait judicieux de la prototyper en private (détail d'implémentation).
**************************************************************
## Q1.2
En utilisant deux boucles imbriquées avec des indices de -1 à 1, on peut parcourir toutes les combinaisons de vecteurs de l'espace torique.
Il suffit de comparer la distance entre le nouveau vecteur et le vecteur max à chaque tour de boucle.
**************************************************************
## Q1.3
Il faut passer les Collider en référence constante, car c'est des classes.
Pas besoin en revanche pour les Vec2d.
**************************************************************
## Q1.4
Les méthodes affichant la distance et la direction n'ont pas besoin de modifier les attributs privés.
Pour la fonction move, elle déplace le Collider, et doit donc accéder à "center". Elle ne doit pas être déclarée comme const.
**************************************************************
