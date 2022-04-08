#pragma once
#ifndef FLOWER_HPP
#define FLOWER_HPP
#include <SFML/Graphics.hpp>
#include "Collider.hpp"

class Flower : public Collider
{
public:
    Flower(Vec2d const&, double, double);
    double takePollen(double);
    void drawOn(sf::RenderTarget&, double);
private:
    double pollen_quantity;
    sf::Texture texture;
};

#endif // FLOWER_HPP
