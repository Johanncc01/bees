#pragma once
#include <SFML/Graphics.hpp>
#include <Env/Collider.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/CFSM.hpp>
#include <Env/Hive.hpp>


class ScoutBee;
class WorkerBee;


enum class Mode : short { repos, random, target};


class Bee : public Collider, public Drawable , public Updatable, public CFSM
{
public:
// Constructeur et destructeur

    /*!
     * @brief Constructeur de base d'une abeille
     *
     * @param référence à une ruche
     * @param position de l'abeille
     * @param tableau des états possibles pour l'abeille
     * @param rayon de l'abeille
     * @param énergie initiale de l'abeille
     * @param amplitude initiale de la vitesse de l'abeille
     */
    Bee(Hive&, Vec2d const&, States, double, double, double);

    /*!
     * @brief Destructeur de l'abeille
     */
    ~Bee();

// Getters

    /*!
     * @brief Getter de la vie de l'abeille
     *
     * @return vrai si l'énergie n'est pas nulle ou négative
     */
    bool isDead();

    /*!
     * @brief Getter de la configuration en fonction de la sous-classe d'abeille
     *
     * @return un tableau contenant les valeurs coresspondantes au bon type dans le .json
     */
    virtual j::Value const& getConfig() const = 0;

// Méthodes pures

    /*!
     * @brief Dessine l'abeille selon ses caractéristiques sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle la ruche est dessinée
     */
    virtual void drawOn(sf::RenderTarget&) const;

    /*!
     * @brief Actualise l'abeille et ses caractéristiques sur un temps donné
     *
     * @param temps dt sur lequel il faut actualiser l'abeille
     */
    void update(sf::Time);

// Déplacement

    /*!
    * @brief Déplace l'abeille sur un temps donné en fonction du mode actuel (attribut)
    *
    * @param temps dt sur lequel le déplacement est calculé
    */
    void move(sf::Time);

    /*!
     * @brief Déplace l'abeille aléatoirement sur un temps donné
     *
     * @param temps dt sur lequel le déplacement est calculé
     */
    void randomMove(sf::Time);

    /*!
     * @brief Déplace l'abeille vers le but (attribut) sur un temps donné
     *
     * @param temps dt sur lequel le déplacement est calculé
     */
    void targetMove(sf::Time);



    void learnFlowerLocation(Vec2d const&);

// Interaction

    virtual void interact(Bee* other) = 0;
    virtual void interactWith(ScoutBee*) = 0;
    virtual void interactWith(WorkerBee*) = 0;


protected:

    Hive& hive;
    Vec2d vitesse;
    double energy;

    Vec2d const* memory;
    Vec2d const* target;

    Mode mode;
    bool isEating;



private:

    sf::Time avoidanceClock_;
};

