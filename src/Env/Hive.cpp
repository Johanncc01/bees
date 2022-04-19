#include <Env/Hive.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>


// Constructeur et destructeur

Hive::Hive(Vec2d const& cen, double rad)
    : Collider(cen, rad), pollen(getAppConfig().hive_initial_nectar)
{ }

Hive::~Hive(){
    for (auto bee : bees){
        delete bee;
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
}


// Bees

/*
void Hive::update(sf::Time dt){

}
*/

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

