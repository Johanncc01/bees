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
     * @brief Constructeur de base
     *
     * @param position de l'abeille
     * @param rayon de l'abeille
     * @param énergie initiale de l'abeille
     * @param amplitude initiale de la vitesse de l'abeille
     */
    Bee(Hive&, Vec2d const&, double, double, double);

    bool isDead();

    virtual j::Value const& getConfig() const;

    void drawOn(sf::RenderTarget&) const;
    void update(sf::Time);



private:

    Hive& hive;
    Vec2d vitesse;
    double energy;




};

