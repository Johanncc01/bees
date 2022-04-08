#pragma once
#ifndef ENV_HPP
#define ENV_HPP
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "Flower.hpp"
#include <vector>
//#include <memory>


class Env

{

public:

    Env();
    ~Env();
    float getSize() const;

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
    std::vector<Flower*> flowers;

    void destroyAll();

};

#endif // ENV_HPP
