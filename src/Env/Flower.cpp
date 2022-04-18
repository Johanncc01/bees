#include <Env/Flower.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Random/Random.hpp>

Flower::Flower(Vec2d const& cen, double rad, double pol)
    : Collider(cen, rad), pollen(pol)
{
    auto const& textures(getAppConfig().flower_textures);
    int taille(textures.size());
    int random(uniform(0, taille-1));
    auto const& new_texture = getAppTexture(textures[random].toString());
    texture = new_texture;
}

double Flower::takePollen(double asked){
    if (pollen >= asked){
        pollen -= asked;
        return asked;
    } else {
        double left(pollen);
        pollen = 0;
        return left;
    }
}

void Flower::drawOn(sf::RenderTarget& target) const{
    auto flowerSprite = buildSprite(center, radius, texture);
    target.draw(flowerSprite);
}

void Flower::update(sf::Time dt){
    double seuil(getAppConfig().flower_growth_threshold);
    double humidity(getAppEnv().get_world_humidity(center));
    pollen += (dt.asSeconds() * log(humidity/seuil));
    int i(0);
    Vec2d new_pos;
    if (pollen > getAppConfig().flower_growth_split){
        pollen /= 2;
        do {
        double d(uniform(1.5*radius, 2.5*radius));  // size min , size_max ?
        new_pos = center+ Vec2d::fromRandomAngle()*d;
        ++i;
        } while (!(getAppEnv().addFlowerAt(new_pos)) and i < 100); // max failures ???
    }
}


bool Flower::hasPollen() const{
    return (pollen > 0);
}
