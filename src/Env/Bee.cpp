#include <Env/Bee.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vec2d.hpp>
#include <Random/Random.hpp>


// Constructeur et destructeur

Bee::Bee(Hive& h, Vec2d const& pos, States s, double rad, double en, double vit)
    : Collider(pos, rad), CFSM(s), hive(h), energy(en), memory(nullptr), target(nullptr), mode(Mode::repos)
{
    vitesse = Vec2d::fromRandomAngle()*vit;
}

Bee::~Bee()
{}


// Getters

bool Bee::isDead(){
    return (energy <= 0);
}

j::Value const& Bee::getConfig() const{
    return getValueConfig();
}


// Méthodes pures

void Bee::drawOn(sf::RenderTarget& target) const{
    auto const& texture = getAppTexture(getConfig()["texture"].toString());
    auto beeSprite = buildSprite(center, radius, texture);

    float alpha(vitesse.Vec2d::angle());
    if ((alpha >= PI/2) or (alpha <= -PI/2)){
        beeSprite.scale(1, -1);
    }
    beeSprite.rotate(alpha/DEG_TO_RAD);

    target.draw(beeSprite);

    if (isDebugOn()){
        if (mode == Mode::random){
            auto shape = buildAnnulus(center, radius, sf::Color::Black, 5.0);
            target.draw(shape);
        } else if (mode == Mode::target){
            auto shape = buildAnnulus(center, radius, sf::Color::Blue, 3.0);
            target.draw(shape);
        }
    }
}

void Bee::update(sf::Time dt){

    action(dt);
    move(dt);
    clampCenter();

    switch (static_cast<short>(mode)){
    case 0 : energy -= double(getConfig()["energy"]["consumption rates"]["idle"].toDouble())*dt.asSeconds();
             break;
    case 1 :
    case 2 : energy -= double(getConfig()["energy"]["consumption rates"]["moving"].toDouble())*dt.asSeconds();
             break;
    }

    if (energy < 0){
        energy = 0;
    }
}

void Bee::move(sf::Time dt){
    switch (static_cast<short>(mode)){
    case 0 : return;
    case 1 : randomMove(dt);
             return;
    case 2 : targetMove(dt);
             return;
    }
}



void Bee::targetMove(sf::Time dt){
    if (avoidanceClock_ < sf::Time::Zero){
        Vec2d normalised(directionTo(*target)/distanceTo(*target));
        vitesse = normalised * vitesse.length();
    } else {
        avoidanceClock_ -= dt;
    }

    if (getAppEnv().isWorldFlyable(center + vitesse*dt.asSeconds())){
        center += vitesse*dt.asSeconds();
    } else {
        avoidanceClock_ = sf::seconds(getConfig()["moving behaviour"]["target"]["avoidance delay"].toDouble());
        double beta;
        if (bernoulli(0.5)){
            beta = PI/4;
        } else {
            beta = -PI/4;
        }
        vitesse.rotate(beta);
    }
}



void Bee::randomMove(sf::Time dt){

    double rotaProba(getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble());
    double rotaMaxAngle(getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());

    if (bernoulli(rotaProba)){
        double alpha(uniform(-rotaMaxAngle, rotaMaxAngle));
        vitesse.rotate(alpha);
    }

    if (getAppEnv().isWorldFlyable(center + vitesse*dt.asSeconds())){
        center += vitesse*dt.asSeconds();
    } else {
        double beta;
        if (bernoulli(0.5)){
            beta = PI/4;
        } else {
            beta = -PI/4;
        }
        vitesse.rotate(beta);
    }
}





