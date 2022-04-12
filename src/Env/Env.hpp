#pragma once
#ifndef ENV_HPP
#define ENV_HPP
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "Flower.hpp"
#include "FlowerGenerator.hpp"
#include <vector>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>



typedef std::vector<Flower*> Flowers;

class Env : public Drawable , public Updatable

{

public:

    Env();
    Env(Env const&) = delete;
    Env operator=(Env const&) = delete;

    ~Env();
    float getSize() const;
    double get_world_humidity(Vec2d const&) const;

    void update(sf::Time dt);
    void drawOn(sf::RenderTarget& target) const;
    void reset();

    void loadWorldFromFile();
    void saveWorldToFile();
    void resetControls();

    bool addFlowerAt(Vec2d const&);
    void drawFlowerZone(sf::RenderTarget&, Vec2d const&);

private:

    World terrain;
    Flowers flowers;

    FlowerGenerator generator;

    void destroyAll();

};

#endif // ENV_HPP
