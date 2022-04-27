#pragma once
#include <SFML/Graphics.hpp>
#include <Env/Collider.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/Hive.hpp>


class Bee : public Collider, public Drawable , public Updatable
{
public:
// Constructeur et destructeur

    /*!
     * @brief Constructeur de base d'une abeille
     *
     * @param référence à une ruche
     * @param position de l'abeille
     * @param rayon de l'abeille
     * @param énergie initiale de l'abeille
     * @param amplitude initiale de la vitesse de l'abeille
     */
    Bee(Hive&, Vec2d const&, double, double, double);

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
    virtual j::Value const& getConfig() const;

// Méthodes pures

    /*!
     * @brief Dessine l'abeille selon ses caractéristiques sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle la ruche est dessinée
     */
    void drawOn(sf::RenderTarget&) const;

    /*!
     * @brief Actualise l'abeille et ses caractéristiques sur un temps donné
     *
     * @param temps dt sur lequel il faut actualiser la ruche
     */
    void update(sf::Time);

private:

    Hive& hive;
    Vec2d vitesse;
    double energy;

};

