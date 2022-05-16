#Programmation Orientée-objets (SV)

##JOURNAL du groupe *86* (*Johann Clausen* & *Victor Galhaud*)

*************************************************
## 21.03 (lundi)

- inscription du groupe
- téléchargement de la partie 1
- lecture de la partie 1
- début du codage de <tt> Collider.hpp/.cpp </tt>
- Victor a eu une très bonne idée pour la double boucle

*************************************************
## 22.03 (mardi - TP)

- fin du codage de la classe <tt> Collider</tt> lors de la première série du projet 
- commentaires du code de la partie 1
- réponses aux questions

*************************************************
## 25.03 (vendredi)

- téléchargement partie 2
- répartition des tâches
- codage de <tt> World.hpp/.cpp</tt>, sans grand succès pour l'instant...

*************************************************
## 28.03 (lundi)

- débugging de la partie 2.1 (résolution problème <tt>World.hpp/.cpp</tt>)
- ajout d'un raccourci pour <tt>world_textures</tt> dans <tt>config.hpp/.cpp</tt> par Johann
- début de la partie 2.2 avec <tt>loadFromFile()</tt>

*************************************************
## 29.03 (mardi - TP)

- partie 2.3 sur la génération aléatoire en série :
	- <tt>step()</tt> terminée
	- <tt>smooth()</tt> ne fonctionne pas
- réponses aux questions

*************************************************
## 30.03 (mercredi)

- finalisation de la partie 2.3 :
	- debugging de <tt>smooth()</tt> (division entière non désirée, on a remplacé les ints par des double)
	- commentaire du code
- rédaction du journal_1
- début de la réflexion sur le codage de la méthode <tt>saveToFile()</tt>

*************************************************
## 01.04 (vendredi)
- finalisation de <tt>saveToFile()</tt> par Johann
- commentaires des fichiers déjà codés par Victor
- téléchargement partie 3

*************************************************
## 04.04 (lundi)
- création de la classe <tt>Env</tt>, définition des premières méthodes reprennant celles de <tt>World</tt>

*************************************************
## 05.04 (mardi - TP)
- Partie 3.1 : 
	- codage de la fonction <tt>World::humidityImpact()</tt>, qui s'occupe de remplir le tableau des valeurs d'humidité
	- affichage des niveaux d'humidité si <tt>showHumidity</tt> est à true (appui sur w) codé
- codage du débug de la fin de la partie 3.1

*************************************************
## 08.04 (vendredi)
- partie 3.2 : 
	- création de <tt>Flower.hpp/.cpp</tt>
	- ébauche de code des méthodes d'affichage (<tt>drawOn</tt>), d'ajout des fleurs (<tt>addFlowerAt</tt>), ...
- Les tests semblent passer sur <tt>app5.json</tt>, mais le programme est trop lent sur les autres fichiers de configuration... Il faut demander si il y a un problème au prochain TP.

*************************************************
## 12.04 (mardi - TP)
- fin de la partie 3.3 :
	- création et codage de <tt>FlowerGenerator.hpp/.cpp</tt>
	- introduction de l'héritage multiple avec <tt>Drawable</tt> et <tt>Updatable</tt> pour toutes les classes codées jusqu'à présent
	- tests fontionnels : la génération est automatique !
- Reste à faire :
	- optimiser le programme pour gagner en rapidité, surtout sur les boucles imbriquées (peut-être dans <tt>World::smooth</tt> ?)
	- commenter toute la partie 3 selon la norme demandée, i.e. avec @brief, @argument, @return.

*************************************************
## 15.04 (vendredi)
- Commentaires partie 3
- Multiples tests pour améliorer la réactivité du programme comme conseillé en TP par l'assistant
	- solution trouvée sur piazza (pas d'appel à <tt>World::updateCache()</tt> dans <tt>Env::update()</tt> car le monde n'a pas besoin d'être reaffiché tout le temps)
- téléchargement et début de la partie 4.1 (jusqu'au test 15)

*************************************************
## 18.04 - 24.04 (pendant les vacances)
- Ajustement de toutes les inclusions : ordre, dépendances circulaires,...
- Commentaires terminés dans les classes

*************************************************
## 25.04 (lundi)
- Codage en commun du bonus de la partie 4.1
	- <tt>World::indexesForRect()</tt> très compliqué à réaliser (+ visualiser les traitements dans le monde torique)
	- mais la fonction est opérationnelle

*************************************************
## 26.04 (mardi - TP)
- fin de la partie 4.2
	- reste à commenter <tt>Bee.hpp/.cpp</tt>
	- compréhension des fonctions avec .json difficile
- problème connu : crash intempestif lors de l'exécution, au bout d'un long moment
	- la cause est difficile à détecter avec le débugger, mais peut être un problème de destruction des objets (Flower, Hive, Bee) -> à investiguer

*************************************************
## 27.04 (mercredi)
- commentaires <tt>Bee.hpp/.cpp</tt>
- rendu intermédiaire
	- Toutes les parties codées jusqu'ici fonctionnent.
- mise à jour du journal

*************************************************
## 03.05 (mardi - TP)
- téléchargement de la partie 5
- partie 5.1 terminée en TP
	- création des classes <tt>ScoutBee</tt>, <tt>WorkingBee</tt>, <tt>CFSM</tt>
	- adaptation de la classe <tt>Bee</tt> pour la rendre virtuelle
- partie 5.2 terminée l'après-midi
	- codage spécifique des <tt>ScoutBee.hpp/cpp</tt>, au travers des méthodes 
		- <tt>ScoutBee::onState()</tt> et <tt>ScoutBee::onEnterState()</tt> qui héritent de <tt>CFSM</tt> (redéfinition) pour les comportements liés aux états
		- <tt>ScoutBee::drawOn()</tt>, qui héritent de <tt>Bee</tt> (redéfintion) pour le dessin du mode debug
- reste à commenter le code ajouté

*************************************************
## 04.05 (mercredi)
- résolution du bug des crashs intempestifs :
	- l'appel de Env::update() se faisait sur les fleurs nouvellement crées par division, ce qui causait des crashs du programme
	- la solution est d'itérer seulement sur les fleurs existantes, à l'aide d'un indice fixé au début de la méthode
- partie 5.3 terminée ensemble :
	- codage de <tt>WorkerBee.hpp/cp</tt>, et des comportements à travers les méthodes <tt>WorkerBee::onState()</tt> et <tt>WorkerBee::onEnterState()</tt>

*************************************************
## 06.05 (vendredi)
- partie 5.4
	- collaborationTest commencé
	- le test d'interaction dans la ruche avec <tt>isColliderInside</tt> ne fonctionne pas toujours
	- il faut encore commenter le code

*************************************************
## 10.05 (mardi - TP)
- partie 5.4 : terminée
	- final application fonctionnel
	- réponse au différentes questions de la partie 5

*************************************************
## 13.05 (vendredi)
- rendu intermédiaire :
	- lecture des commentaires du coach (merci!!!)
	- adaptation du code en conséquence 
		- destructeur de Flower et de Bee en virtuel, pour le redéfinir dans les sous-classes
		- rempalcement des structures if-condition par return-condition
- mise à jour du journal

*************************************************
## 17.05 (mardi - TP)
- partie 6
	- téléchargement
	- codage de la classe <tt>Stats</tt>, affichant des statistiques sur la fenêtre de simulation
- commentaires restants des parties précédentes

		