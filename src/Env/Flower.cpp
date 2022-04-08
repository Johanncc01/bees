#include "Flower.hpp"
#include <Application.hpp>
#include "Utility/Utility.hpp"
#include <Random/Random.hpp>

Flower::Flower(Vec2d const& cen, double rad, double pollen)
    : Collider(cen, rad), pollen_quantity(pollen)
{
    auto const& textures(getAppConfig().flower_textures);
    int taille(textures.size());
    int random(uniform(0, taille-1));
    auto const& new_texture = getAppTexture(textures[random].toString());
    texture = new_texture;
}

double Flower::takePollen(double asked){
    if (pollen_quantity >= asked){
        pollen_quantity -= asked;
        return asked;
    } else {
        double left(pollen_quantity);
        pollen_quantity = 0;
        return left;
    }
}

void Flower::drawOn(sf::RenderTarget& target, double world_size){
    Vec2d pos(center.x()*world_size, center.y()*world_size);
    auto flowerSprite = buildSprite(pos, radius, texture);
    target.draw(flowerSprite);
}

