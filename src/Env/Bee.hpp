#pragma once
#include <SFML/Graphics.hpp>
#include <Env/Collider.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/Hive.hpp>


class Bee : public Collider, public Drawable , public Updatable
{
public:
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

