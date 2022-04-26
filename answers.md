# Projet programmation orientée objet (SSV)

## REPONSES du groupe *86* (*Johann Clausen* & *Victor Galhaud*)

## Partie 1

**************************************************************
### Q1.1
Pour éviter la duplication de code, il faut mettre <tt>clamp()</tt> dans une méthode de la classe. 
Comme cette méthode ne sera appelée qu'à l'intérieur de la classe, il serait judicieux de la prototyper en private (détail d'implémentation).
**************************************************************
### Q1.2
En utilisant deux boucles imbriquées avec des indices de -1 à 1, on peut parcourir toutes les combinaisons de vecteurs de l'espace torique.
Il suffit de comparer la distance entre le nouveau vecteur et le vecteur max à chaque tour de boucle.
**************************************************************
### Q1.3
Il faut passer les <tt>Collider</tt> et les <tt>Vec2d</tt> en référence constante, car c'est des classes complexes.
Pour les <tt>double</tt>, le passage par valeur suffit.
**************************************************************
### Q1.4
Les méthodes affichant la distance et la direction n'ont pas besoin de modifier les attributs privés.
Pour la fonction move, elle déplace le <tt>Collider</tt>, et doit donc accéder à <tt>center</tt>. Elle ne doit pas être déclarée comme <tt>const</tt>.
**************************************************************
### Q1.5
Il suffit de return les résultats des fonctions rédigées auparavant.
**************************************************************
### Q1.6
Pour tous les opérateurs (sauf <<), il faut utiliser la surcharge interne. Les opérateurs sont "proches de la classe", ils ont besoin d'accès internes (+= modifie l'objet). 
Pour l'opérateur d'affichage, il faut le faire en externe car on ne peut pas redéfinir la classe iostream, et que <tt>Collider</tt> n'est pas l'opérande de gauche.
**************************************************************
### Q1.7
Comme auparavant, il faut passer les classes (<tt>Collider</tt>, <tt>Vec2d</tt>) en référence constante, les types de base (<tt>double</tt>) par valeur.
Notons qu'il est nécesssaire de passer le <tt>ostream</tt> par référence (non constante), car il s'agit d'un flot.
**************************************************************
### Q1.8
Les seules méthodes qui peuvent modifier l'instance de <tt>Collider</tt> devraient être l'opérateur "=" et "+=".
Toutes les autres méthodes peuvent être déclarées comme <tt>const</tt>.
**************************************************************


## Partie 2

**************************************************************
### Q2.1
Le nombre de cellules totales dans le vector est <tt>nb_ cells</tt> * <tt>nb_ cells</tt>.
En effet, il ne s'agit pas de <tt>world_ size</tt> au carré, car cette valeur sera utilisée par SFML pour afficher nos cellules de largeur <tt>cell_size</tt>.
**************************************************************
### Q2.2
<tt> world_cells : "simulation", "world", "default cells" <br>
world_size : "simulation", "world", "default size"</tt>
**************************************************************
### Q2.3
Il faut d'abord appeler <tt>reloadConfig()</tt>, qui s'occupera des arguments <tt>nb_cells</tt>, <tt>cell_size</tt>, et du tableau <tt>cells_ </tt>.
Ensuite, <tt>reloadCacheStructure()</tt> utilisera les nouvelles valeurs des attributs pour initialiser les vertexes et <tt>renderingCache_ </tt>.
Enfin, <tt>updateCache()</tt>, afin d'appliquer les changements dans renderingCache_.
**************************************************************
### Q2.4
Le changement de fichier source pour le terrain peut se faire sans recompiler le programme en entier.
On peut, à l'aide de paramètres de ligne de commande, modifier le fichier désiré pour le chargement (info retrouvée à l'aide de <tt>getApp()</tt> et <tt>getAppConfig()</tt>)
**************************************************************
### Q2.5
Elle doit appeler les fonctions : <tt>reloadCacheStructure()</tt> et <tt>updateCache()</tt>.
Ces appels permettront d'initialiser les <tt>vertexes</tt> et <tt>renderingCache_</tt>, ainsi que de d'actualiser les changements.
**************************************************************
### Q2.6 
Nous avons choisi de représenter l'ensemble <tt>seeds_</tt> par un vector de <tt>Seeds</tt>.
Nous avons privilégié le type <tt>vector</tt> au type <tt>array</tt> car nous ne connaissons pas la taille de cet ensemble au moment où nous l'initialisons.
**************************************************************


## Partie 3

**************************************************************
### Q3.1
Pour le moment, comme notre <tt>Env</tt> ne contient que le terrain, il suffit d'appeler les méthodes correspondantes dans la classe <tt>world</tt>. C'est à dire respectivement les méthodes <tt> updateCache(), drawOn() et reset()</tt> contenues dans la classe <tt>World</tt>.
**************************************************************
### Q3.2
Il s'agit d'une méthode qui pour l'attribut terrain de <tt>Env</tt>, appelle sa méthode <tt>loadFromFile()</tt> codée à la partie précédente. Cette fonction met déjà à jour l'affichage du monde. On peut donc remplacer les appels dans le constructeur de <tt>Env</tt>. <br>
Le fichier utilisé est déterminé dans la méthode <tt> World::loadFromFile()</tt>, où il est repris depuis le json de configuration choisi. <br>
L'appel de <tt>Env::reset()</tt> lors de l'appui sur la touche 'r' est réalisé dans <tt> Application.cpp</tt>, plus précisement aux lignes 519 à 529.
**************************************************************
### Q3.3
Tous les endroits où une graine transmet son type à <tt> cells_</tt>, i.e. dans : 
<ul> - l'initialisation des graines dans <tt>World::reset()</tt> <br> - l'avancement des graines dans <tt> World::step()</tt> <br> - le lissage dans <tt>World::smooth()</tt> </ul>
Nous avons créé une nouvelle méthode <tt>World::humidityImpact(int id)</tt>, qui prend en argument l'id de la cellule c1 qui devient une cellule d'eau, et qui met à jour le taux d'humidité des cellules influencées par c1. Elle sera appelée juste après la modification de <tt>cells</tt>.
**************************************************************
### Q3.4
Il suffit d'intégrer à la boucle existante le traitement de l'humidité, i.e. calculer <tt>niveau bleu</tt> pour chaque cellule et l'assigner au <tt> humidityVertexes</tt> pour chaque indice des quads. (au même endroit que l'herbe, l'eau et la pierre)
**************************************************************
### Q3.5
Il faut faire en sorte que <tt>Flower</tt> hérite de la classe <tt>Collider</tt>, pour que les méthodes déjà définies pour la gestion des collisions soient disponibles pour cette nouvelle classe.
**************************************************************
### Q3.6
Pour que le choix de la texture se fasse uniquement à la création de la fleur, nous pourrions déplacer les traitements nécessaires dans le constructeur de <tt>Flower</tt>. Ainsi, la texture serait déterminée dès le début de vie et ne changerait pas.
**************************************************************
### Q3.7
Pour prévoir les différentes sortes de fleurs, qui peuvent avoir des traitements différents selon leur nature, il faut définir la collection de fleurs de façon à pouvoir utiliser le polymorphisme, i.e. comme une collection de pointeurs sur <tt>Flower</tt>.

**************************************************************
### Q3.8
Lors de la destruction d'un <tt>Env</tt>, il faut penser à également détruire tous les éléments qui ne peuvent pas vivre en dehors de lui, ici les fleurs. Ce traitement sera spécifié dans le destructeur de <tt>Env</tt>.
**************************************************************
### Q3.9
De placer ces suppressions dans les instructions liées au destructeur de <tt>Env</tt>, qui prendra donc soin de supprimer à la fois les fleurs et le terrain.
**************************************************************
### Q3.10
<tt>Env::drawOn</tt> doit maintenant appeler <tt>Flower::drawOn</tt> pour chaque fleur faisant partie de l'environnement, en plus de l'appel existant à <tt>World::drawOn</tt>.
**************************************************************
### Q3.11
Comme la méthode doit avoir accès au tableau des valeurs d'humidités, il faut la placer dans <tt>World</tt>, puis l'appeler à partir d'une nouvelle méthode de <tt>Env</tt>. (accessible grâce à <tt>getAppEnv()</tt>)
**************************************************************
### Q3.12
Il faut faire en sorte que la méthode <tt>Env::update(sf::Time dt)</tt> appelle pour chaque fleur de la collection de fleurs sa méthode <tt>Flower::update(sf::Time dt)</tt>. Attention à itérer sur l'ensemble initial et pas sur les fleurs nouvellement ajoutées.
**************************************************************
### Q3.13
Après l'appel de <tt>Flower::update(sf::Time dt)</tt> dans <tt> Env::update(sf::Time dt)</tt>, si la quantité de pollen de la fleur en cours est nulle, il faut alors la supprimer (via delete) et assigner son pointeur dans le tableau à <tt>nullptr</tt>. A la fin de la boucle, l'insctruction donnée dans l'énoncé permet de supprimer tous les <tt>nullptr</tt> présents dans le vector de fleurs de <tt>Env</tt>.
**************************************************************
### Q3.14
Les classes <tt>Drawable</tt> et <tt> Updatable</tt> possèdent respectivement des méhthodes virtuelles pures <tt> virtual void drawOn(sf::RenderTarget) const = 0</tt> et <tt> virtual void update(sf::Time) const = 0</tt>. <br>
Il faut donc faire hériter de ces classes toutes nos classes qui ont une méthode <tt>drawOn</tt> ou <tt> update</tt>, i.e. :
<ul> - <tt>Env</tt> : les deux  <br> - <tt>Flower</tt> : les deux <br> - <tt>World</tt> : juste <tt>Drawable</tt> </ul>
L'avantage conceptionnel est que nous sommes forcés à redéfinir ces méthodes dans nos sous-classes.
**************************************************************
### Q3.15
Il faut penser à supprimer le constructeur de copie, et l'opérateur d'affectation dans <tt>World</tt> et <tt> Env</tt> avec le mot clé <tt>delete</tt>.
**************************************************************
### Q3.16
Il faut maintenant appeller la méthode <tt>FlowerGenerator::update</tt> dans <tt>Env::reset</tt>.
**************************************************************


## Partie 4

**************************************************************
### Q4.1
Notre classe <tt>Bee</tt> sera certainement une super-classe dont vont hériter tout les différents types d'abaeilles que nous allons implémenter. Pour utiliser le polymorphisme, nous aurons donc besoin d'utiliser un vecteur de pointeurs sur <tt>Bee</tt> dans les ruches où nous allons les stocker.
**************************************************************
### Q4.2
<tt>Hive</tt> héritera de :
<ul> - <tt> Collider</tt>, pour avoir directement les tests de collision <br> - <tt>Drawable</tt> car elle contient une méthode <tt>drawOn</tt> <br> - <tt>Updatable</tt> car elle contient une méthode <tt>update</tt> </ul> 
Elle contiendra évidemment un vector de <tt>Bees</tt> et un double de pollen, pour modéliser les fonctions comme demandé dans l'énoncé.
**************************************************************
### Q4.3
Nous supprimerons le constructeur de copie, et l'opérateur d'affectation dans <tt>Hive</tt> avec le mot clé <tt>delete</tt>, comme à la question 3.15.
**************************************************************
### Q4.4
Dans le destructeur de <tt>Hive</tt>, il ne faut pas oublier de <tt>delete</tt> tous les éléments pointés par le vector <tt>bees</tt> de <tt>Hive</tt>, ainsi que de le vider.
**************************************************************
### Q4.5
Non, elles doivent disparaître dans le destructeur de <tt>Env</tt>
**************************************************************
### Q4.6
Il faut modifier <tt>World::isGrowable</tt> pour qu'elle détecte la présence de ruches, en utilisant <tt>Env::getCollidingHive</tt> via <tt>getAppEnv</tt>.
**************************************************************
### Q4.7
Il faut ajouter une itération sur les pointeurs sur <tt>Hive</tt> pour détruire leur contenu comme fait pour les fleurs précedemment.





