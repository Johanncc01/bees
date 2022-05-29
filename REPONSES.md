# Projet programmation orientée objet (SSV)

## REPONSES du groupe *86* (*Johann Clausen* & *Victor Galhaud*)

## Partie 1

**************************************************************
### Q1.1
Pour éviter la duplication de code, il faut mettre `clamp()` dans une méthode de la classe. 
Comme cette méthode ne sera appelée qu'à l'intérieur de la classe, il serait judicieux de la prototyper en `private` (détail d'implémentation).
**************************************************************
### Q1.2
En utilisant deux boucles imbriquées avec des indices de -1 à 1, on peut parcourir toutes les combinaisons de vecteurs de l'espace torique.
Il suffit de comparer la distance entre le nouveau vecteur et le vecteur max à chaque tour de boucle.
**************************************************************
### Q1.3
Il faut passer les `Collider` et les `Vec2d` en référence constante, car ce sont des classes complexes.
Pour les `double`, le passage par valeur suffit.
**************************************************************
### Q1.4
Les méthodes affichant la distance et la direction n'ont pas besoin de modifier les attributs privés.
Pour la fonction move, elle déplace le `Collider`, et doit donc accéder à `center`. Elle ne doit pas être déclarée comme `const`.
**************************************************************
### Q1.5
Il suffit de return les résultats des fonctions rédigées auparavant.
**************************************************************
### Q1.6
Pour tous les opérateurs (sauf <<), il faut utiliser la surcharge interne. Les opérateurs sont "proches de la classe", ils ont besoin d'accès internes (+= modifie l'objet). 
Pour l'opérateur d'affichage, il faut le faire en externe car on ne peut pas redéfinir la classe iostream, et que `Collider` n'est pas l'opérande de gauche.
**************************************************************
### Q1.7
Comme auparavant, il faut passer les classes (`Collider`, `Vec2d`) en référence constante, les types de base (`double`) par valeur.
Notons qu'il est nécessaire de passer le `ostream` par référence (non constante), car il s'agit d'un flot.
**************************************************************
### Q1.8
Les seules méthodes qui peuvent modifier l'instance de `Collider` devraient être l'opérateur "=" et "+=".
Toutes les autres méthodes peuvent être déclarées comme `const`.
**************************************************************


## Partie 2

**************************************************************
### Q2.1
Le nombre de cellules totales dans le `vector` est `nb_cells` * `nb_cells`.
En effet, il ne s'agit pas de `world_ size` au carré, car cette valeur sera utilisée par SFML pour afficher nos cellules de largeur `cell_size`.
**************************************************************
### Q2.2
`world_cells : "simulation", "world", "default cells"`
`world_size : "simulation", "world", "default size"`
**************************************************************
### Q2.3
Il faut d'abord appeler `reloadConfig()`, qui s'occupera des arguments `nb_cells`, `cell_size`, et du tableau `cells_ `.
Ensuite, `reloadCacheStructure()` utilisera les nouvelles valeurs des attributs pour initialiser les vertexes et `renderingCache_ `.
Enfin, `updateCache()`, afin d'appliquer les changements dans `renderingCache_`.
**************************************************************
### Q2.4
Le changement de fichier source pour le terrain peut se faire sans recompiler le programme en entier.
On peut, à l'aide de paramètres de ligne de commande, modifier le fichier désiré pour le chargement (info retrouvée à l'aide de `getApp()` et `getAppConfig()`)
**************************************************************
### Q2.5
Elle doit appeler les fonctions : `reloadCacheStructure()` et `updateCache()`.
Ces appels permettront d'initialiser les `vertexes` et `renderingCache_`, ainsi que de d'actualiser les changements.
**************************************************************
### Q2.6 
Nous avons choisi de représenter l'ensemble `seeds_` par un vector de `Seeds`.
Nous avons privilégié le type `vector` au type `array` car nous ne connaissons pas la taille de cet ensemble au moment où nous l'initialisons.
**************************************************************


## Partie 3

**************************************************************
### Q3.1
Pour le moment, comme notre `Env` ne contient que le terrain, il suffit d'appeler les méthodes correspondantes dans la classe `world`. C'est à dire respectivement les méthodes ` updateCache(), drawOn() et reset()` contenues dans la classe `World`.
**************************************************************
### Q3.2
Il s'agit d'une méthode qui pour l'attribut terrain de `Env`, appelle sa méthode `loadFromFile()` codée à la partie précédente. Cette fonction met déjà à jour l'affichage du monde. On peut donc remplacer les appels dans le constructeur de `Env`.

Le fichier utilisé est déterminé dans la méthode ` World::loadFromFile()`, où il est repris depuis le .json de configuration sélectionné.

L'appel de `Env::reset()` lors de l'appui sur la touche 'r' est réalisé dans ` Application.cpp`, plus précisément aux lignes 519 à 529.
**************************************************************
### Q3.3
Tous les endroits où une graine transmet son type à ` cells_`, i.e. dans : 

 - l'initialisation des graines dans `World::reset()` 
 - l'avancement des graines dans ` World::step()`
 - le lissage dans `World::smooth()`

Nous avons créé une nouvelle méthode `World::humidityImpact(int id)`, qui prend en argument l'id de la cellule c1 qui devient une cellule d'eau, et qui met à jour le taux d'humidité des cellules influencées par c1. Elle sera appelée juste après la modification de `cells`.
**************************************************************
### Q3.4
Il suffit d'intégrer à la boucle existante le traitement de l'humidité, i.e. calculer `niveau bleu` pour chaque cellule et l'assigner au ` humidityVertexes` pour chaque indice des quads (au même endroit que l'herbe, l'eau et la pierre).
**************************************************************
### Q3.5
Il faut faire en sorte que `Flower` hérite de la classe `Collider`, pour que les méthodes déjà définies pour la gestion des collisions soient disponibles pour cette nouvelle classe.
**************************************************************
### Q3.6
Pour que le choix de la texture se fasse uniquement à la création de la fleur, nous pourrions déplacer les traitements nécessaires dans le constructeur de `Flower`. Ainsi, la texture serait déterminée dès le début de vie et ne changerait pas.
**************************************************************
### Q3.7
Pour prévoir les différentes sortes de fleurs, qui peuvent avoir des traitements différents selon leur nature, il faut définir la collection de fleurs de façon à pouvoir utiliser le polymorphisme, i.e. comme une collection de pointeurs sur `Flower`.

**************************************************************
### Q3.8
Lors de la destruction d'un `Env`, il faut penser à également détruire tous les éléments qui ne peuvent pas vivre en dehors de lui, ici les fleurs. Ce traitement sera spécifié dans le destructeur de `Env`.
**************************************************************
### Q3.9
De placer ces suppressions dans les instructions liées au destructeur de `Env`, qui prendra donc soin de supprimer à la fois les fleurs et le terrain.
**************************************************************
### Q3.10
`Env::drawOn` doit maintenant appeler `Flower::drawOn` pour chaque fleur faisant partie de l'environnement, en plus de l'appel existant à `World::drawOn`.
**************************************************************
### Q3.11
Comme la méthode doit avoir accès au tableau des valeurs d'humidités, il faut la placer dans `World`, puis l'appeler à partir d'une nouvelle méthode de `Env` (accessible grâce à `getAppEnv()`).
**************************************************************
### Q3.12
Il faut faire en sorte que la méthode `Env::update(sf::Time dt)` appelle pour chaque fleur de la collection de fleurs sa méthode `Flower::update(sf::Time dt)`. Attention à itérer sur l'ensemble initial et pas sur les fleurs nouvellement ajoutées.
**************************************************************
### Q3.13
Après l'appel de `Flower::update(sf::Time dt)` dans ` Env::update(sf::Time dt)`, si la quantité de pollen de la fleur en cours est nulle, il faut alors la supprimer (via `delete`) et assigner son pointeur dans le tableau à `nullptr`. 

A la fin de la boucle, l'instruction donnée dans l'énoncé permet de supprimer tous les `nullptr` présents dans le vector de fleurs de `Env`.
**************************************************************
### Q3.14
Les classes `Drawable` et ` Updatable` possèdent respectivement des méthodes virtuelles pures ` virtual void drawOn(sf::RenderTarget) const = 0` et ` virtual void update(sf::Time) const = 0`. Il faut donc faire hériter de ces classes toutes nos classes qui ont une méthode `drawOn` ou ` update`, i.e. :

- `Env` : les deux
- `Flower` : les deux
- `World` : juste `Drawable`

L'avantage conceptionnel est que nous sommes forcés à redéfinir ces méthodes dans nos sous-classes.
**************************************************************
### Q3.15
Il faut penser à supprimer le constructeur de copie, et l'opérateur d'affectation dans `World` et ` Env` avec le mot clé `delete`.
**************************************************************
### Q3.16
Il faut maintenant appeler la méthode `FlowerGenerator::update` dans `Env::reset`.
**************************************************************


## Partie 4

**************************************************************
### Q4.1
Notre classe `Bee` sera certainement une super-classe dont vont hériter tous les différents types d'abeilles que nous allons implémenter. Pour utiliser le polymorphisme, nous aurons donc besoin d'utiliser un vecteur de pointeurs sur `Bee` dans les ruches où nous allons les stocker.
**************************************************************
### Q4.2
`Hive` héritera de :

- ` Collider`, pour avoir directement les tests de collision 
- `Drawable` car elle contient une méthode `drawOn` 
- `Updatable` car elle contient une méthode `update`

Elle contiendra évidemment un vector de `Bees` et un double de pollen, pour modéliser les fonctions comme demandé dans l'énoncé.
**************************************************************
### Q4.3
Nous supprimerons le constructeur de copie, et l'opérateur d'affectation dans `Hive` avec le mot clé `delete`, comme à la question 3.15.
**************************************************************
### Q4.4
Dans le destructeur de `Hive`, il ne faut pas oublier de `delete` tous les éléments pointés par le vector `bees` de `Hive`, ainsi que de le vider.
**************************************************************
### Q4.5
Non, elles doivent disparaître dans le destructeur de `Env`.
**************************************************************
### Q4.6
Il faut modifier `World::isGrowable` pour qu'elle détecte la présence de ruches, en utilisant `Env::getCollidingHive` via `getAppEnv`.
**************************************************************
### Q4.7
Il faut ajouter une itération sur les pointeurs sur `Hive` pour détruire leur contenu comme fait pour les fleurs précédemment.
**************************************************************
### Q4.8
`Bee` héritera de :

- ` Collider`, pour avoir directement les tests de collision 
- `Drawable` car elle contient une méthode `drawOn` 
- `Updatable` car elle contient une méthode `update`

Elle contiendra donc une référence sur une `Hive`, un vecteur vitesse et un `double` d'énergie. On déclare également les méthodes `Bee::move(sf::Time st)` (déplacement sur un temps donné) et `Bee::isDead()` (teste si l'énergie est nulle ou négative).
**************************************************************
### Q4.9
Il faut penser à la déclarer comme virtuelle, à l'aide du mot-clé `virtual` en début de ligne dans le `.hpp`.
**************************************************************
### Q4.10
Si on utilise la méthode polymorphique `Bee::getConfig()`, on obtient un tableau contenant toutes les caractéristiques concernant le type d'abeille avec lequel on travaille actuellement. On peut retrouver 
**************************************************************
### Q4.11
Il faut utiliser cette tournure, plutôt que les raccourcis d'écriture, car ces derniers ne pourront pas être redéfinis dans les sous-classes de manière polymorphique. Avec la méthode `Bee::getConfig()`, on peut choisir de quel type d'abeille on veut retrouver la configuration.
**************************************************************
### Q4.12
Adaptons déjà les méthodes `Hive::drawOn(sf::RenderTarget& target)` et `Hive::update(sf::Time dt)`, respectivement pour qu'elles affichent toutes les abeilles du vector du pointeur de `Bees`, et qu'elles les actualisent. Rajoutons également l'actualisation des ruches dans `Env::update(sf::Time dt)`, qui appellera l'actualisation des abeilles.
Le dessin des ruches (et donc de leurs abeilles) était déjà implémenté dans `Env`.
**************************************************************
### Q4.13
Le test de disparition des abeilles se fera dans `Hive::update(sf::Time dt)`, à l'aide de `Bee::isDead()`. Si elle n'a plus d'énergie, nous la supprimerons du vector de pointeur sur `Bee`. Ce n'est donc pas dans `Env` que ce traitement aura lieu, car il ne contient pas le vector. 
**************************************************************
### Q4.14
Car la méthode `Hive::addBee()` est `protected` et non publique, et on ne pourrait pas l'appeler depuis l'extérieur de la classe. C'est pourquoi on utilise une sous-classe qui se chargera de l'appeler.
Ce mécanisme permet aussi de vérifier que seules les ruches peuvent créer des abeilles, et non pas l'environnement par exemple (car méthode non publique).

## Partie 5

**************************************************************
### Q5.1
- Il faut les redéfinir dans les sous-classes `ScoutBee` et `WorkerBee`, avec le mot clé `override`, de sorte qu'elles renvoient un tableau contenant les configurations des éclaireuses et des butineuses respectivement.
- Pour empêcher l'instanciation d'une abeille générique, la méthode `Bee::getConfig()` doit être déclarée virtuelle pure, ce qui oblige la redéfinition dans les sous-classes.
- Pour drawOn, les méthodes n'ont pas besoin d'être redéfinies, car l'appel à `getConfig()` existe déjà et sera adapté en fonction de la sous-classe qui les appelle.

**************************************************************
### Q5.2
Car lors de la construction de la sous-classe, la méthode `getConfig()` n'est pas encore redéfinie, et c'est celle de la super-classe Bee qui sera appelée. Or, ce n'est pas le résultat souhaité à travers le polymorphisme. Il est donc plus simple d'utiliser un appel unique à `getAppConfig()` pour le constructeur.

**************************************************************
### Q5.3
Les constructeurs de `ScoutBee` et `WorkerBee` doivent donc être modifiés pour fournir un vector contenant l'état `IN_HIVE` (pour l'instant), car le constructeur de `Bee` a été adapté après l'héritage de `CFSM`.

**************************************************************
### Q5.4
Comme une abeille peut oublier une position, un pointeur sur un Vec2d paraît judicieux. En effet, lorsque l'attribut mémoire sera à `nullptr`, c'est que l'abeille n'aura rien en mémoire (≠ vecteur nul, qui la ramènerait à l'origine). Comme la mémoire est commune à toutes les abeilles, on peut l'implémenter dans la super-classe `Bee`, et son constructeur devra l'initialiser à `nullptr`.

**************************************************************
### Q5.5
Les méthodes qui doivent être redéfinies dans les sous-classes pour qu'elles soient instanciables sont celles qui sont virtuelles pures dans les super-classes, i.e. :

- `virtual void onState(State, sf::Time) = 0;` , héritée de CFSM
- `virtual void onEnterState(State) = 0;` , héritée de CFSM
- `virtual j::Value const& getConfig() const = 0;` , héritée de Bee

**************************************************************
### Q5.6
On crée un type énuméré pour les modes de déplacement possible, et on ajoute un attribut qui représente l'état actuel. Il faut l'initialiser dans le constructeur au repos. La cible sera modélisée comme un pointeur sur un Vec2d, car on peut ne pas avoir de cible (nullptr).

**************************************************************
### Q5.7
Car on fait appel au polymorphisme via la méthode virtuelle `getConfig()`, qui redéfinit les valeurs dans les sous-classes.

**************************************************************
### Q5.8
Il faut créer les 3 variables statiques de classe pour `ScoutBee` représentant les différents états. Ceci sera fait par des déclarations au début de `ScoutBee.cpp`, puis en transmettant au constructeur de `ScoutBee` un vector contenant ces états, qui sera transféré au constructeur de `CFSM`.

**************************************************************
### Q5.9
1. De inHive à lookingForFlower ; elle doit avoir oublié la fleur qu'elle vient de transmettre.
2. De lookingForFlower à backToHive, car le début du déplacement ciblé demande une cible à viser.

**************************************************************
### Q5.10
Les traitements peuvent se faire dans la fonction `onState()` de ScoutBee, en appelant la méthode codée `Env::getCollidingFlowerPosition(Collider)`.
Au travers du mot réservé const, on ne pourra pas modifier la variable à travers ce pointeur, ce qui semble logique car la mémoire n'est pas sensée changer jusqu'à sa destruction.

**************************************************************
### Q5.11
1. De toFlower à collectPollen, elle n'a plus besoin de cibler la fleur si elle y est
2. De inHive à toFlower, pour le bon fonctionnement du déplacement ciblé
3. De collectPollen à backToHive, car le début du déplacement ciblé demande une cible à viser.
4. Le déplacement aléatoire ne sera pas utilisé pour les `WorkerBee`, mais le déplacement ciblé interviendra au début de toFlower et de backToHive

**************************************************************
### Q. ICC
Si la vérification de la possibilité d'interaction se fait dans l'appel de `interact()`, on sait déjà que l'on itère sur des combinaisons d'abeilles valables, i.e. on n'a pas de risque de tomber sur une des deux abeilles à l'extérieur de la ruche et on économise des appels de fonctions qui pourraient ralentir l'exécution.


Cela permet de réduire la complexité en moyenne, car il est quand même possible que toutes les abeilles soient dans la ruche (pire cas).

**************************************************************
### Q5.12
Les tests de type sont mauvais, car ils sont difficiles à implémenter de manière générique, il faut les définir à l'infini, et ils nuisent à l'abstraction visée par la programmation orientée objet.

**************************************************************
### Q5.13
Dans la méthode `Hive::update(sf::Time dt)`, pour pouvoir d'une partie ajouter des abeilles, mais aussi pour avoir accès à la taille du vector de bees.


## Partie 6

**************************************************************
### Q6.1
Comme nos éléments sont associés à un identifiant entier, on utilise des `maps` avec comme type de clé `int`, i.e.

- ```std::map<int, unique_ptr<Graph>> ```, appelé Graphs via un `typedef`
- ```std::map<int, std::string> ```, appelé Strings via un `typedef`

L'avantage de cette solution est que chaque élément pourra être retrouvé directement, et que l'ordre des ids n'a pas à être linéaire.

**************************************************************
### Q6.2
Il faudra ajouter les méthodes :

- dans Env :
	- ```std::unordered_map<std::string, double> fetchData(std::string) const;```
	- ```size_t getHivesScoutNumber() const;``` et ```size_t getHivesWorkerNumber() const;```, qui parcourent les ruches et somment leurs nombres d'abeilles
- dans Hive :
	- ```size_t getScoutNumber() const;``` et ```size_t getWorkerNumber() const;```, getters pour l'attribut correspondant au nombre d'abeilles
	- ```void changeScoutNumber(bool);```et ```void changeWorkerNumber(bool);```, qui seront appelées dans le constructeur et le destructeur des abeilles pour soit augmenter, soit diminuer l'attribut correspondant.

## Autres fichiers
- [README](README.md)
- [Journal](JOURNAL-86.md)
