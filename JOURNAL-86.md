# Programmation Orientée-objets (SV)

## JOURNAL du groupe *86* (*Johann Clausen* & *Victor Galhaud*)

*************************************************
## 21.03 (lundi)

- inscription du groupe
- téléchargement de la partie 1
- lecture de la partie 1
- début du codage de `Collider.hpp/.cpp`
- Victor a eu une très bonne idée pour la double boucle

*************************************************
## 22.03 (mardi - TP)

- fin du codage de la classe `Collider` lors de la première série du projet 
- commentaires du code de la partie 1
- réponses aux questions

*************************************************
## 25.03 (vendredi)

- téléchargement partie 2
- répartition des tâches
- codage de `World.hpp/.cpp`, sans grand succès pour l'instant...

*************************************************
## 28.03 (lundi)

- debugging de la partie 2.1 (résolution problème `World.hpp/.cpp`)
- ajout d'un raccourci pour `world_textures` dans `config.hpp/.cpp` par Johann
- début de la partie 2.2 avec `World::loadFromFile()`

*************************************************
## 29.03 (mardi - TP)

- partie 2.3 sur la génération aléatoire en série :
	- `World::step()` terminée
	- `World::smooth()` ne fonctionne pas
- réponses aux questions

*************************************************
## 30.03 (mercredi)

- finalisation de la partie 2.3 :
	- debugging de `World::smooth()` (division entière non désirée, on a remplacé les `int` par des `double`)
	- commentaire du code
- rédaction du premier journal
- début de la réflexion sur le codage de la méthode `World::saveToFile()`

*************************************************
## 01.04 (vendredi)
- finalisation de `World::saveToFile()` par Johann
- commentaires des fichiers déjà codés par Victor
- téléchargement partie 3

*************************************************
## 04.04 (lundi)
- création de la classe `Env`, définition des premières méthodes reprenant celles de `World`

*************************************************
## 05.04 (mardi - TP)
- Partie 3.1 : 
	- codage de la fonction `World::humidityImpact()`, qui s'occupe de remplir le tableau des valeurs d'humidité
	- affichage des niveaux d'humidité si `showHumidity` est à `true (appui sur w) codé
- codage du debug de la fin de la partie 3.1

*************************************************
## 08.04 (vendredi)
- partie 3.2 : 
	- création de `Flower.hpp/.cpp`
	- ébauche de code des méthodes d'affichage (`Env::drawOn`), d'ajout des fleurs (`Env::addFlowerAt`), ...
- Les tests semblent passer sur `app5.json`, mais le programme est trop lent sur les autres fichiers de configuration... Il faut demander s'il y a un problème au prochain TP.

*************************************************
## 12.04 (mardi - TP)
- fin de la partie 3.3 :
	- création et codage de `FlowerGenerator.hpp/.cpp`
	- introduction de l'héritage multiple avec `Drawable` et `Updatable` pour toutes les classes codées jusqu'à présent
	- tests fonctionnels : la génération est automatique !
- Reste à faire :
	- optimiser le programme pour gagner en rapidité, surtout sur les boucles imbriquées (peut-être dans `World::smooth`?)
	- commenter toute la partie 3 selon la norme demandée, i.e. avec `@brief`, `@argument`, `@return`.

*************************************************
## 15.04 (vendredi)
- Commentaires partie 3
- Multiples tests pour améliorer la réactivité du programme comme conseillé en TP par l'assistant
	- solution trouvée sur piazza (pas d'appel à `World::updateCache()` dans `Env::update()` car le monde n'a pas besoin d'être réaffiché tout le temps)
- téléchargement et début de la partie 4.1 (jusqu'au test 15)

*************************************************
## 18.04 - 24.04 (pendant les vacances)
- Ajustement de toutes les inclusions : ordre, dépendances circulaires,...
- Commentaires terminés dans les classes

*************************************************
## 25.04 (lundi)
- Codage en commun du bonus de la partie 4.1
	- `World::indexesForRect()` très compliqué à réaliser (+ visualiser les traitements dans le monde torique)
	- mais la fonction est opérationnelle

*************************************************
## 26.04 (mardi - TP)
- fin de la partie 4.2
	- reste à commenter `Bee.hpp/.cpp`
	- compréhension des fonctions avec .json difficile
- problème connu : crash intempestif lors de l'exécution, au bout d'un long moment
	- la cause est difficile à détecter avec le débugger, mais peut être un problème de destruction des objets (`Flower`, `Hive`, `Bee`) -> à investiguer

*************************************************
## 27.04 (mercredi)
- commentaires `Bee.hpp/.cpp`
- rendu intermédiaire
	- Toutes les parties codées jusqu'ici fonctionnent.
- mise à jour du journal

*************************************************
## 03.05 (mardi - TP)
- téléchargement de la partie 5
- partie 5.1 terminée en TP
	- création des classes `ScoutBee`, `WorkingBee`, `CFSM`
	- adaptation de la classe `Bee` pour la rendre virtuelle
- partie 5.2 terminée l'après-midi
	- codage spécifique des `ScoutBee.hpp/cpp`, au travers des méthodes 
		- `ScoutBee::onState()` et `ScoutBee::onEnterState()` qui héritent de `CFSM` (redéfinition) pour les comportements liés aux états
		- `ScoutBee::drawOn()`, qui héritent de `Bee` (redéfinition) pour le dessin du mode debug
- reste à commenter le code ajouté

*************************************************
## 04.05 (mercredi)
- résolution du bug des crashs intempestifs :
	- l'appel de `Env::update()` se faisait sur les fleurs nouvellement crées par division, ce qui causait des crashs du programme
	- la solution est d'itérer seulement sur les fleurs existantes, à l'aide d'un indice fixé au début de la méthode
- partie 5.3 terminée ensemble :
	- codage de `WorkerBee.hpp/cpp`, et des comportements à travers les méthodes `WorkerBee::onState()` et `WorkerBee::onEnterState()`

*************************************************
## 06.05 (vendredi)
- partie 5.4
	- `collaborationTest` commencé
	- le test d'interaction dans la ruche avec `isColliderInside` ne fonctionne pas toujours
	- il faut encore commenter le code

*************************************************
## 10.05 (mardi - TP)
- partie 5.4 : terminée
	- `final application` fonctionnel
	- réponse aux différentes questions de la partie 5

*************************************************
## 13.05 (vendredi)
- rendu intermédiaire :
	- lecture des commentaires du coach (merci!!!)
	- adaptation du code en conséquence 
		- destructeur de `Flower` et de `Bee` en virtuel, pour le redéfinir dans les sous-classes
		- remplacement  des structures if-condition par return-condition
- mise à jour du journal

*************************************************
## 17.05 (mardi - TP)
- partie 6
	- téléchargement
	- codage de la classe `Stats`, affichant des statistiques sur la fenêtre de simulation
	- problème : le test ne fonctionne pas (il faut peut-être utiliser une map indexée par des strings pour `Graphs`, à tester)
- commentaires restants des parties précédentes

*************************************************
## 18.05 (mercredi)
- partie 6
	- résolution de l'incompréhension : il fallait utiliser des `maps` indexées avec les ids (entiers) pour `Graphs` et `Strings`.

*************************************************
## 23.05 (lundi)
- fin de la partie 6
	- graphs `s::GENERAL` et `s::HIVES` implémentés et fonctionnels
	- méthodes `Env::fetchData()`, `Env::getHivesIds()`, et d'autres méthodes dans Hive et Bee pour accéder aux données lors de l'actualisation
- reste à faire :
	- terminer les commentaires des parties précédentes
	- revoir dans `Collider` et `Bee` pour que tous les attributs soient privés (et non protected) pour l'encapsulation

*************************************************
## 25.05 (mercredi)
- commentaires de toutes les classes `.hpp` terminés : reste à voir si les `.cpp` sont assez commentés
- code archivé pour la partie 6 et préparé pour le rendu (toutes les parties dans différents dossiers)
- à voir si on a le temps + envie de faire des extensions

*************************************************
## 27.05 (vendredi)
- Encapsulation réglée : plus aucun attribut en protected (ni dans `Collider`, ni dans `Bee`)
- Quelques petites extensions codées (adaptation du code à la simulation, voir `README`)
- Rédaction du `README`

*************************************************
## 30.05 (lundi)
- Dernières vérifications et rendu final effectué
	- les parties 1 à 6 fonctionnent


## Autres fichiers
- [README](README.md)
- [Réponses](REPONSES.md)
		