#include "Hive.hpp"
#include <Application.hpp>
#include <Utility/Utility.hpp>

Hive::Hive(Vec2d const& cen, double rad)
    : Collider(cen, rad)
{
    nectar = getAppConfig().hive_initial_nectar;
}

Hive::~Hive(){
    for (auto bee : bees){
        delete bee;
    }
    bees.clear();
}


void Hive::drawOn(sf::RenderTarget& target) const{
    auto const& texture = getAppTexture(getAppConfig().hive_texture);
    auto hiveSprite = buildSprite(center, 2.5*radius, texture);
    target.draw(hiveSprite);
    if (isDebugOn()){
        sf::Vector2i affichage(center.x(),center.y()-30);
        auto const text = buildText(to_nice_string(nectar), affichage, getAppFont(), 30, sf::Color::Red);
        target.draw(text);
    }
}


void Hive::dropPollen(double qte){
    nectar += qte;
}


double Hive::takeNectar(double qte){
    if (nectar >= qte){
        nectar -= qte;
        return qte;
    } else {
        double left(nectar);
        nectar = 0;
        return left;
    }
}

