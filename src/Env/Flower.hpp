#pragma once
#include <SFML/Graphics.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/Collider.hpp>


class Flower : public Collider , public Drawable , public Updatable
{
public:
    Flower(Vec2d const&, double, double);
    double takePollen(double);
    void drawOn(sf::RenderTarget&) const;
    void update(sf::Time);

    bool hasPollen() const;

private:
    double pollen;
    sf::Texture texture;
};
