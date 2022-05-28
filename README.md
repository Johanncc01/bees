# To [bee](https://media.istockphoto.com/vectors/honey-bee-isolated-on-white-vector-id1141216018?k=20&m=1141216018&s=612x612&w=0&h=kVyne-5M4x-fvo1cw1mOYrrit-VfDbXcWSvm2pMezYU=) or not to bee
### Projet BA2 - Programmation orientée objet
#### Johann Clausen et Victor Galhaud - Groupe 86

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

## Simulation :
- Pour les conditions initiales du .json, la simulation se déroule bien jusqu'à un certain point critique, où les workers disparaissent plus vite que les scout, et se reproduisent moins. Cela peut mener à un blocage où il ne reste que des scouts dans la ruche.
	- Une des solutions est d'adapter la probabilité d'apparition selon la population (voir extension #3), ou de changer d'autres paramètres du .json (augmenter worker.to leave hive par exemple)
- On remarque aussi que pour une carte plus grande (app"0".json), la survie est plus difficile car le nombre de fleurs maximal reste constant alors que l'espace à parcourir est plus vaste. L'énergie des abeilles baisse donc plus rapidemment.

## Bonus :
- Sauvegarde du monde généré dans un fichier .map
	- <tt>World::saveToFile()</tt> 
- Affichage du debug d'humidité
	- <tt>World::drawOn(sf::RenderTarget& target)</tt>
- Affichage du carré de prévisualisation (torique) de la position de la ruche 
	- <tt>World::isHiveable(Vec2d const& pos, double rad)</tt>
	- <tt>World::indexesForRect(Vec2d const& top, Vec2d const& bot)</tt>
	- <tt>Env::drawHiveableZone(sf::RenderTarget& target, Vec2d const& pos)</tt>
- Affichage de statistiques
	- <tt>class Stat</tt>
	- méthodes supplémentaires pour acccéder aux données dans <tt>Env, Hive, Bee </tt>

## Extensions :
1. Les ruches disparaissent si elles ne contiennent plus d'abeilles ni de pollen 
	- Getter <tt>Hive::isDead()</tt>
2. Affichages supplémentaires de debug pour les fleurs et les abeilles (appels commentés)
	- <tt>Flower::advancedDebugText(sf::RenderTarget&) const</tt> pour le niveau de pollen d'une fleur
	- <tt>Bee::advancedDebugText(sf::RenderTarget&) const</tt> pour l'état de la mémoire et de la cible (pointeurs)
3. Probabilité de reproduction dynamique selon la population, pour optimiser la survie de la ruche
	-  Dans <tt>Hive::update(sf::Time dt)</tt>, calcul de la probabilité adapté à chaque update.







