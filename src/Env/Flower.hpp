#pragma once
#ifndef FLOWER_HPP
#define FLOWER_HPP
#include <SFML/Graphics.hpp>
#include "Collider.hpp"
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

class Flower : public Collider , public Drawable , public Updatable
{
public:
    Flower(Vec2d const&, double, double);
    double takePollen(double);
    void drawOn(sf::RenderTarget&) const;
    void update(sf::Time);

    bool hasPollen() const;

private:
    double pollen_quantity;
    sf::Texture texture;
};

#endif // FLOWER_HPP
