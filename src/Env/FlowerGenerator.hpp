#pragma once
#include <SFML/Graphics.hpp>
#include <Interface/Updatable.hpp>

class FlowerGenerator : public Updatable
{
public:
    FlowerGenerator();
    void update(sf::Time dt);
    void reset();

private:
    sf::Time compteur;

};

