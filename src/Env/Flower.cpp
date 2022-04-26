#include <Env/Flower.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Random/Random.hpp>


// Constructeur

Flower::Flower(Vec2d const& cen, double rad, double pol)
    : Collider(cen, rad), pollen(pol)
{
    auto const& textures(getAppConfig().flower_textures);
    int taille(textures.size());
    int random(uniform(0, taille-1));
    texture = (sf::Texture const&)(getAppTexture(textures[random].toString()));     // Attribution de la texture aléatoire dans le constructeur, pour que la fleur ait toujours la même
}

Flower::~Flower() {
};
// Méthodes pures

void Flower::drawOn(sf::RenderTarget& target) const{
    auto flowerSprite = buildSprite(center, radius, texture);
    target.draw(flowerSprite);
}

void Flower::update(sf::Time dt){
    double seuil(getAppConfig().flower_growth_threshold);
    double humidity(getAppEnv().getWorldHumidity(center));          // Utilisation du getter de l'humidité (la fleur n'a pas accès aux attributs privés de World)
    pollen += (dt.asSeconds() * log(humidity/seuil));
    int i(0);
    Vec2d new_pos;
    if (pollen > getAppConfig().flower_growth_split){
        pollen /= 2;
        do {
            double d(uniform(1.5*radius, 2.5*radius));              // size min , size_max ?
            new_pos = center + Vec2d::fromRandomAngle()*d;
            ++i;
        } while (!(getAppEnv().addFlowerAt(new_pos)) and i < 100);  // i = max failures ???
    }
}


// Gestion du pollen

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

bool Flower::hasPollen() const{
    return (pollen > 0);
}
