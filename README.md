# To [bee](https://media.istockphoto.com/vectors/honey-bee-isolated-on-white-vector-id1141216018?k=20&m=1141216018&s=612x612&w=0&h=kVyne-5M4x-fvo1cw1mOYrrit-VfDbXcWSvm2pMezYU=) or not to bee
### Projet BA2 - Programmation orientée objet (SV)
Réalisé par **Johann Clausen** et **Victor Galhaud** - Groupe 86

## Description :

Cette simulation informatique vise à modéliser l'évolution de **ruches** et de leurs **abeilles** dans un **environnement** simulé contenant des **fleurs**. 

Dans le cadre de notre projet, nous nous sommes limités à un monde constitué **d'herbe**, **d'eau** et **de roche**. Ces différents types de terrains ont des interactions particulières avec les autres objets de la simulation. Par exemple, une fleur ne peut pousser dans que sur de l'herbe, et une abeille ne peut survoler de la roche.

On peut charger le monde depuis un fichier, en générer un aléatoire et même le sauvegarder dans un fichier.

On compte également deux types **d'abeilles** aux fonctions différentes :

- les **éclaireuses** (`ScoutBee`) : leur rôle est de parcourir la carte aléatoirement à la recherche d'une fleur. D'une fois qu'elles ont repéré une fleur à butiner, elles retournent à la ruche et attendent une butineuse pour transmettre la position de la fleur qu'elles ont retenues. Après avoir transmis l'information et s'être nourrie, l'éclaireuse recommence un cycle.
- les **butineuses** (`WorkerBee`) : elles doivent attendre dans la ruche une information venant d'une éclaireuse. Dès la réception d'une position, elles vont directement vers la fleur pour la butiner, et ramener le pollen extrait à la ruche, où elles le déposeront. Après s'être nourrie, la butineuse recommence un cycle.

Tous les objets de la simulation sont actualisés fréquemment :

- les **fleurs** gagnent ou perdent du pollen en fonction de l'humidité de leur position, et disparaissent si elles n'en ont plus.
- les **abeilles** consomment de l'énergie en fonction de leur mode de déplacement, et disparaissent si elles n'en ont plus.

Les **statistiques** d'évolution de la simulation sont visibles dans l'onglet en dessous de la fenêtre de simulation.

Le codage du projet a été réalisé en adéquation avec l'énoncé (pas de modifications personnelles d'éléments de conception suggérés). Des éléments supplémentaires ont été ajoutés sous la rubrique "extensions".

## Compilation et exécution :
Ce projet a été réalisé à l'aide de [**CMake**](https://cmake.org/) pour la compilation. 

- En **ligne de commande** : 
	- dans le dossier build : `cmake ../src` pour compiler
	- `make *nom_cible*` pour la génération de la cible
	- lancer l'exécutable pour démarrer la simulation
- Dans **QTCreator** : 
	-  configuration du projet : ouvrir le fichier ` src/CMakeLists.txt`
	-  compiler le projet
	-  choisir la cible à exécuter pour démarrer la simulation

### Cibles principales
- **application** : correspond à l'application finale
- **WorldTest** : permet de tester la génération de mondes
- **FlowerTest** : permet de tester la génération de fleurs
- **CollaborationTest** : permet de tester la collaboration entre une éclaireuse et une butineuse (avec seulement 2 abeilles dans la ruche)

## Simulation (fichiers de configuration) :
Pour les conditions initiales du .json, la simulation se déroule bien jusqu'à un certain point critique, où les **workers** disparaissent plus vite que les **scout**, et se reproduisent moins. Cela peut mener à un blocage où il ne reste que des **scouts** dans la ruche. Une des solutions est d'adapter la probabilité d'apparition selon la population (voir extension #3), ou de changer d'autres paramètres du .json (augmenter `worker_energy_to_leave_hive` par exemple)

On remarque aussi que pour une carte plus grande (`app"0".json`), la survie est plus difficile car le nombre de fleurs maximales reste constant alors que l'espace à parcourir est plus vaste. L'énergie des abeilles baisse donc plus rapidement.

Il faut aussi remarquer qu'avec la conception de l'énoncé, il peut y avoir des problèmes dans les cas limites de `Collider::isInside()`. En effet, si les valeurs à comparer sont les mêmes, alors la méthode retournera `false`. Ceci peut poser des problèmes d'exécution avec `app.json`, où le `Collider` des butineuses ont exactement la même taille que celui de la ruche et ne peuvent pas y rentrer.

## Contrôles :
- L : reset de la simulation et chargement du monde à partir du fichier
- O : sauvegarde du monde dans un fichier
- R : reset de la simulation et génération d'un nouveau monde <br> <br>
- F : création d'une nouvelle fleur (et affichage de la zone)
- H : création d'une nouvelle ruche (et affichage de la zone) <br> <br>
- W : affichage des niveaux d'humidité
- D : activation du mode debug (informations supplémentaires)
- C : recharge la configuration à partir du fichier .json (dans /res) <br> <br>
- PageUp : augmente le contrôle actuel (par défaut les statistiques)
- PageDown : diminue le contrôle actuel (par défaut les statistiques) <br> <br>
- espace : met la simulation en pause
- flèches directionnelles : déplacer la fenêtre de simulation
- ESC : quitte la simulation

Les commandes pour l'utilisation d'une cible particulière sont données dans un panneau d'aide à droite de la fenêtre de simulation.

## Bonus :
- Sauvegarde du monde généré dans un fichier .map
	- `World::saveToFile()` 
- Affichage du debug d'humidité
	- `World::drawOn(sf::RenderTarget& target)`
- Affichage du carré de prévisualisation (torique) de la position de la ruche 
	- `World::isHiveable(Vec2d const& pos, double rad)`
	- `World::indexesForRect(Vec2d const& top, Vec2d const& bot)`
	- `Env::drawHiveableZone(sf::RenderTarget& target, Vec2d const& pos)`
- Affichage de statistiques
	- `class Stat`
	- méthodes supplémentaires pour accéder aux données dans `Env, Hive, Bee `

## Extensions :
1. Les ruches disparaissent si elles ne contiennent plus d'abeilles ni de pollen 
	- Getter `Hive::isDead()`
2. Affichages supplémentaires de debug pour les fleurs et les abeilles (appels commentés aux lignes `Flower.cpp:34` et `Bee.cpp:89`)
	- `Flower::advancedDebugText(sf::RenderTarget&) const` pour le niveau de pollen d'une fleur
	- `Bee::advancedDebugText(sf::RenderTarget&) const` pour l'état de la mémoire et de la cible (pointeurs), ainsi que l'énergie pour rentrer (extension #4)
3. Probabilité de reproduction dynamique selon la population, pour optimiser la survie de la ruche
	-  Dans `Hive::update(sf::Time dt)`, calcul de la probabilité d'appartition d'une éclaireuse ou butineuse adapté à la populattion actuelle de la ruche à chaque update.
4. Calcul de l'énergie nécessaire pour rentrer à la ruche en fonction de la position de l'abeille
	- `Bee::computeEnergy()` retourne cette valeur, qui est maintenant utilisée dans les conditions de passage à l'état suivant dans `ScoutBee::onState(State)` et `WorkerBee::onState(State)`.
	- **ATTENTION** : présent uniquement dans la partie 7. Pour qu'il soit pleinement fonctionnel, il faut modifier dans le .json le `worker_energy_to_leave_hive` pour le mettre à la même valeur que `scout_energy_to_leave_hive`, i.e. à 3.

## Autres fichiers
- [Journal](JOURNAL-86.md)
- [Réponses](REPONSES.md)





