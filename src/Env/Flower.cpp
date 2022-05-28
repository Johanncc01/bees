#include <Env/Flower.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Random/Random.hpp>


// Constructeur

Flower::Flower(Vec2d const& cen, double rad, double pol)
    : Collider(cen, rad), pollen(pol)
{
    // Attribution de la texture aléatoire dans le constructeur, pour que la fleur ait toujours la même
    auto const& textures(getAppConfig().flower_textures);
    int taille(textures.size());
    int random(uniform(0, taille-1));
    texture = (sf::Texture const&)(getAppTexture(textures[random].toString()));
}


// Getter

bool Flower::hasPollen() const{
    return (pollen > 0);
}


// Méthodes pures

void Flower::drawOn(sf::RenderTarget& target) const{
    auto flowerSprite = buildSprite(getPosition(), getRadius(), texture);
    target.draw(flowerSprite);
    if (isDebugOn()){
        // Pour afficher le debug pollen (voir README)
        // advancedDebugText(target);
    }
}

void Flower::update(sf::Time dt){
    double seuil(getAppConfig().flower_growth_threshold);
    // Utilisation du getter de l'humidité (la fleur n'a pas accès aux attributs privés de World)
    double humidity(getAppEnv().getWorldHumidity(getPosition()));

    pollen += (dt.asSeconds() * log(humidity/seuil));

    if (pollen > getAppConfig().flower_growth_split){
        pollen /= 2;
        Vec2d new_pos;
        int i(0);
        do {
            double d(uniform(1.5*getRadius(), 2.5*getRadius()));
            new_pos = getPosition() + Vec2d::fromRandomAngle()*d;
            ++i;
        } while (!(getAppEnv().addFlowerAt(new_pos)) and i < getAppConfig().flowers_max_failures);
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


// Fonction d'implémentation

void Flower::advancedDebugText(sf::RenderTarget& target) const{
    Vec2d affichage(getPosition().x(),getPosition().y()-10);
    auto const text = buildText(to_nice_string(pollen), affichage, getAppFont(), 8, sf::Color::Black);
    target.draw(text);
}
