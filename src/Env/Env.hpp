#pragma once
#include <SFML/Graphics.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/World.hpp>
#include <Env/Flower.hpp>
#include <Env/FlowerGenerator.hpp>
#include <Env/Hive.hpp>
#include <vector>


typedef std::vector<Flower*> Flowers;
typedef std::vector<Hive*> Hives;

class Env : public Drawable , public Updatable

{

public:

    Env();
    Env(Env const&) = delete;
    Env operator=(Env const&) = delete;

    ~Env();
    float getSize() const;
    double getWorldHumidity(Vec2d const&) const;

    void update(sf::Time dt);
    void drawOn(sf::RenderTarget& target) const;
    void reset();

    void loadWorldFromFile();
    void saveWorldToFile();
    void resetControls();

    bool addFlowerAt(Vec2d const&);
    void drawFlowerZone(sf::RenderTarget&, Vec2d const&);

    bool addHiveAt(Vec2d const&);
    Hive* getCollidingHive(const Collider&);
    Flower* getCollidingFlower(const Collider&);

private:

    World terrain;
    Flowers flowers;
    Hives hives;

    FlowerGenerator generator;

    void destroyAll();

};
