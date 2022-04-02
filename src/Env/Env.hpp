#pragma once
#ifndef ENV_HPP
#define ENV_HPP
#include "World.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class Env

{

public:

    Env();
    void update(sf::Time dt);
    void drawOn(sf::RenderTarget& target);
    void reset();

    void loadWorldFromFile();
    void saveWorldToFile();

private:

    World terrain;

};

#endif // ENV_HPP
