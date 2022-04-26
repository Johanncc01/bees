#include <Env/Hive.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Env/Bee.hpp>


// Constructeur et destructeur

Hive::Hive(Vec2d const& cen, double rad)
    : Collider(cen, rad), pollen(getAppConfig().hive_initial_nectar)
{ }

Hive::~Hive(){
    for (auto bee : bees){
        delete bee;
        bee = nullptr;
    }
    bees.clear();
}


// Méthodes pures

void Hive::drawOn(sf::RenderTarget& target) const{
    auto const& texture = getAppTexture(getAppConfig().hive_texture);
    auto hiveSprite = buildSprite(center, 2.5*radius, texture);
    target.draw(hiveSprite);
    if (isDebugOn()){
        sf::Vector2i affichage(center.x(),center.y()-30);
        auto const text = buildText(to_nice_string(pollen), affichage, getAppFont(), 30, sf::Color::Red);
        target.draw(text);
    }

    for (auto bee : bees){
        bee->drawOn(target);
    }
}


void Hive::update(sf::Time dt){
    for (auto& bee : bees){
        bee->update(dt);
        if (bee->isDead()){
            delete bee;
            bee = nullptr;
        }
    }
    bees.erase(std::remove(bees.begin(), bees.end(), nullptr), bees.end());
}



// Bees

void Hive::addBee(){
    bees.push_back(new Bee(*this, {center.x(), center.y() + 10}, 10.0, 10.0, 10.0));
}



// Pollen

void Hive::dropPollen(double qte){
    pollen += qte;
}


double Hive::takePollen(double qte){
    if (pollen >= qte){
        pollen -= qte;
        return qte;
    } else {
        double left(pollen);
        pollen = 0;
        return left;
    }
}

