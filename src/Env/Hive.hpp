#pragma once
#include <SFML/Graphics.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/Collider.hpp>
#include <Env/Bee.hpp>


typedef std::vector<Bee*> Bees;

class Hive : public Collider, public Drawable //, public Updatable
{
public:
    Hive(Vec2d const&, double);
    Hive(Hive const&) = delete;
    Hive& operator=(Hive const&) = delete;
    ~Hive();

    void addBee();
    //void update(sf::Time);
    void drawOn(sf::RenderTarget&) const;
    void dropPollen(double);
    double takeNectar(double);


private:
    Bees bees;
    double nectar;

};

