#pragma once
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/World.hpp>
#include <Env/Flower.hpp>
#include <Env/Hive.hpp>
#include <Env/FlowerGenerator.hpp>

#include <vector>


// typedef des types complexes
typedef std::vector<Flower*> Flowers;
typedef std::vector<Hive*> Hives;
typedef std::unordered_map<std::string, double> Data;

class Env : public Drawable , public Updatable
{
public:
// Constructeur et destructeur

    /*!
     * @brief Constructeur par défaut, essaie de charger un monde et en cas d'échec, en génère un aléatoire
     */
    Env();

    /*!
     * @brief Destructeur de l'environnement, qui détruit les fleurs et les ruches
     */
    ~Env();

    // Interdit les copies et affectations pour éviter les erreurs
    Env(Env const&) = delete;
    Env operator=(Env const&) = delete;


// Getters

    /*!
     * @brief Getter de la taille de l'environnement
     *
     * @return le float de la taille du monde
     */
    float getSize() const;

    /*!
     * @brief Getter du niveau d'humidité d'une position donnée
     *
     * @param position de la cellule
     *
     * @return niveau d'humidité de la cellule
     */
    double getWorldHumidity(Vec2d const&) const;

    /*!
     * @brief Getter de isFlyable
     *
     * @param position de la cellule
     *
     * @return vrai si la cellule n'est pas de la roche
     */
    bool isWorldFlyable(Vec2d const&) const;


// Méthodes pures

    /*!
     * @brief Actualise l'environnement (fleurs + ruches + générateur)
     *
     * @param temps dt sur lequel il faut actualiser l'environnement
     */
    void update(sf::Time dt) override;

    /*!
     * @brief Dessine l'environnement (terrain, fleurs et ruches) sur une cible donnée en appeleant les méthodes drawOn()
     *
     * @param "RenderTarget" sur laquelle l'environnement est dessinée
     */
    void drawOn(sf::RenderTarget& target) const override;

    /*!
     * @brief Réinitialise l'environnement en appeleant les méthodes reset() (terrain et générateur), puis en supprimant les fleurs et ruches
     */
    void reset();

    /*!
     * @brief bonus pas encore codé
     */
    void resetControls();


// World
    /*!
     * @brief Charge un monde depuis un fichier, en initialisant tous les attributs nécessaires (et détruit les fleurs et ruches)
     */
    void loadWorldFromFile();
    /*!
     * @brief Sauvegarde le monde actuel dans un fichier, selon les conventions de loadFromFile()
     */
    void saveWorldToFile();


// Flower
    /*!
     * @brief Essaie d'ajouter une fleur dans l'environnement à une position donnée
     *
     * @param position de la fleur à ajouter
     *
     * @return vrai si l'ajout a pu avoir lieu
     */
    bool addFlowerAt(Vec2d const&);

    /*!
     * @brief dessine une zone colorée rouge ou verte en fonction de la disponbilité pour une fleur dans le monde
     *
     * @param "RenderTarget" sur laquelle le cercle est dessiné
     * @param position du curseur à vérifier et où afficher le cercle
     */
    void drawFlowerZone(sf::RenderTarget&, Vec2d const&) const;

    /*!
     * @brief recherche une fleur de l'environnement en collision avec un collider donné
     *
     * @param un collider dont on veut tester les collisions
     *
     * @return un pointeur sur la fleur correspondante, ou "nullptr" si aucun résultat
     */
    Flower* getCollidingFlower(Collider const&) const;

    /*!
     * @brief recherche la position d'une fleur de l'environnement en collision avec un collider donné (via getCollidingFlower())
     *
     * @param un collider dont on veut tester les collisions
     *
     * @return un pointeur sur la position de la fleur correspondante, ou "nullptr" si aucun résultat
     */
    Vec2d const* getCollidingFlowerPosition(Collider const&) const;


// Hive

    /*!
     * @brief Essaie d'ajouter une ruche dans l'environnement à une position donnée
     *
     * @param position de la ruche à ajouter
     *
     * @return vrai si l'ajout a pu avoir lieu
     */
    bool addHiveAt(Vec2d const&);

    /*!
     * @brief dessine un carré coloré rouge, bleu ou vert en fonction de la disponbilité pour une ruche dans le monde
     *
     * @param "RenderTarget" sur laquelle le cercle est dessiné
     * @param position du curseur à vérifier et où afficher le carré
     */
    void drawHiveableZone(sf::RenderTarget&, Vec2d const&) const;

    /*!
     * @brief recherche une ruche de l'environnement en collision avec un collider donné
     *
     * @param un collider dont on veut tester les collisions
     *
     * @return un pointeur sur la ruche correspondante, ou "nullptr" si aucun résultat
     */
    Hive* getCollidingHive(Collider const&) const;


// Stats

    /*!
     * @brief actualise les données pour un graphe de titre donné
     *
     * @param le titre du graphe
     *
     * @return un tableau contenant les données, indexé par les titres des séries du graphe
     */
    Data fetchData(std::string) const;

    /*!
     * @brief répertorie les identifiants des ruches pour créer les statistiques correspondantes
     *
     * @return un tableau contenant les noms des ruches
     */
    std::vector<std::string> getHivesIds() const;


private:

    World terrain;
    Flowers flowers;
    Hives hives;

    FlowerGenerator generator;

    // Fonctions d'implémentation

    /*!
     * @brief Détruit les attributs de Env alloués dynamiquement (fleurs et ruches)
     */
    void destroyAll();

    /*!
     * @brief Actualise les fleurs
     *
     * @param temps dt sur lequel il faut actualiser les fleurs
     */
    void updateFlowers(sf::Time dt);

    /*!
     * @brief Actualise les ruches
     *
     * @param temps dt sur lequel il faut actualiser les ruches
     */
    void updateHives(sf::Time dt);

    /*!
     * @brief Affiche le carré de création de la ruche dans un monde torique
     */
    void drawToricHivable(sf::RenderTarget&, Vec2d const&, Vec2d const&, sf::Color) const;

    /*!
     * @brief Getter du nombre d'éclaireuses
     *
     * @return le nombre total d'éclaireuses à travers les ruches
     */
    size_t getHivesScoutNumber() const;

    /*!
     * @brief Getter du nombre de butineuses
     *
     * @return le nombre total de butineuses à travers les ruches
     */
    size_t getHivesWorkerNumber() const;

};
