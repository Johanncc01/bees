#pragma once
#ifndef FLOWERGENERATOR_HPP
#define FLOWERGENERATOR_HPP
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

#endif // FLOWERGENERATOR_HPP
