#include <Env/Bee.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vec2d.hpp>
#include <Random/Random.hpp>


// Constructeur et destructeur

Bee::Bee(Hive& h, Vec2d const& pos, double rad, double en, double vit)
    : Collider(pos, rad), hive(h), energy(en)
{
    vitesse = Vec2d::fromRandomAngle()*vit;
}

bool Bee::isDead(){
    return (energy <= 0);
}

j::Value const& Bee::getConfig() const{
    return getValueConfig()["simulation"]["bees"]["scout"];
}


void Bee::drawOn(sf::RenderTarget& target) const{
    auto const& texture = getAppTexture(getConfig()["texture"].toString());
    auto beeSprite = buildSprite(center, radius, texture);

    float alpha(vitesse.Vec2d::angle());
    if ((alpha >= PI/2) or (alpha <= -PI/2)){
        beeSprite.scale(1, -1);
    }
    beeSprite.rotate(alpha/DEG_TO_RAD);

    target.draw(beeSprite);
}



void Bee::update(sf::Time dt){

    double rotaProba(getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble());
    double rotaMaxAngle(getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());

    if (bernoulli(rotaProba)){
        double alpha(uniform(-rotaMaxAngle, rotaMaxAngle));
        vitesse.rotate(alpha);
    }

    Vec2d posCandidate(center + vitesse*dt.asSeconds());

    if (getAppEnv().isWorldFlyable(posCandidate)){
        center = posCandidate;
    } else {
        double beta;
        if (bernoulli(0.5)){
            beta = PI/4;
        } else {
            beta = -PI/4;
        }
        vitesse.rotate(beta);
    }

    clamp();

    energy -= 0.1*dt.asSeconds();
    if (energy < 0){
        energy = 0;
    }
}







