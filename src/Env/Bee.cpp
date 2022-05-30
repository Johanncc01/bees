#include <Env/Bee.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Random/Random.hpp>


// Constructeur et destructeur

Bee::Bee(Hive& h, Vec2d const& pos, States s, double rad, double en, double vit)
    : Collider(pos, rad), CFSM(s), hive(h), vitesse(Vec2d::fromRandomAngle()*vit), energy(en), memory(nullptr), target(nullptr), mode(Mode::repos)
{}

Bee::~Bee()
{
    delete memory;
    delete target;
}


// Getters

bool Bee::isDead(){
    return (energy <= 0);
}

j::Value const& Bee::getConfig() const{
    return getValueConfig();
}

// Getters et setters pour les classes héritées

Hive& Bee::getHive() const{
    return hive;
}

double Bee::getEnergy() const{
    return energy;
}

void Bee::setEnergy(double newEnergy){
    energy = newEnergy;
}

Vec2d const* Bee::getMemory() const{
    return memory;
}

void Bee::setMemory(Vec2d const* newMemory){
    memory = newMemory;
}

Vec2d const* Bee::getTarget() const{
    return target;
}

void Bee::setTarget(Vec2d const* newTarget){
    target = newTarget;
}

void Bee::setMode(Mode newMode){
    mode = newMode;
}


// Méthodes pures

void Bee::drawOn(sf::RenderTarget& target) const{
    // Dessine la texture associée à l'abeille courante
    auto const& texture = getAppTexture(getConfig()["texture"].toString());
    auto beeSprite = buildSprite(getPosition(), getRadius(), texture);

    float alpha(vitesse.Vec2d::angle());
    if ((alpha >= PI/2) or (alpha <= -PI/2)){
        beeSprite.scale(1, -1);
    }
    beeSprite.rotate(alpha/DEG_TO_RAD);

    target.draw(beeSprite);

    if (isDebugOn()){
        if (mode == Mode::random){
            auto shape = buildAnnulus(getPosition(), getRadius(), sf::Color::Black, 3.0);
            target.draw(shape);
        } else if (mode == Mode::target){
            auto shape = buildAnnulus(getPosition(), getRadius(), sf::Color::Blue, 3.0);
            target.draw(shape);
        }
        // Pour afficher le debug memory, target et computeEnergy() (voir README)
        // advancedDebugText(target);
    }
}

void Bee::update(sf::Time dt){

    // Fait agir l'abeille en fonction de son état
    action(dt);
    // Fait bouger l'abeille en fonction de son mode de déplacement
    move(dt);
    // Fait diminuer l'énergie en fonction du mode de déplaacement
    updateEnergy(dt);
}


// Déplacement

void Bee::move(sf::Time dt){
    switch (static_cast<short>(mode)){
    case 0 : return;
    case 1 : randomMove(dt);
             return;
    case 2 : targetMove(dt);
             return;
    }
}

void Bee::randomMove(sf::Time dt){

    double rotaProba(getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble());
    double rotaMaxAngle(getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());

    if (bernoulli(rotaProba)){
        double alpha(uniform(-rotaMaxAngle, rotaMaxAngle));
        vitesse.rotate(alpha);
    }

    if (getAppEnv().isWorldFlyable(getPosition() + vitesse*dt.asSeconds())){
        // appelle Collider::move(dx), qui s'occupe du clamping
        *this += vitesse*dt.asSeconds();
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

void Bee::targetMove(sf::Time dt){

    if (avoidanceClock_ < sf::Time::Zero){
        Vec2d normalised(directionTo(*target)/distanceTo(*target));
        vitesse = normalised * vitesse.length();
    } else {
        avoidanceClock_ -= dt;
    }

    if (getAppEnv().isWorldFlyable(getPosition() + vitesse*dt.asSeconds())){
        // appelle Collider::move(dx), qui s'occupe du clamp
        *this += vitesse*dt.asSeconds();
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

double Bee::computeEnergy() const{
    double dist(distanceTo(getHive().getPosition()));
    double rate(getConfig()["energy"]["consumption rates"]["moving"].toDouble());
    return rate*(dist/vitesse.length()) + 1;

}


// Interactions

void Bee::learnFlowerLocation(Vec2d const& flowerPosition){
    memory = new Vec2d(flowerPosition);
}


// Fonctions d'implémentation

void Bee::updateEnergy(sf::Time dt){
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

void Bee::advancedDebugText(sf::RenderTarget& target) const{
    // Pour afficher l'état de la mémoire
    Vec2d renderPos1(getPosition().x(), getPosition().y() - getAppConfig().scout_size*2);
    // Pour afficher l'état de la cible
    Vec2d renderPos2(renderPos1.x(), renderPos1.y() - 10);
    // Pour afficher l'énergie nécessaire pour rentrer à la ruche
    Vec2d renderPos3(renderPos2.x(), renderPos2.y() - 10);

    if (memory == nullptr){
        auto const text = buildText("memory : empty", renderPos1, getAppFont(), 10, sf::Color::Red);
        target.draw(text);
    } else {
        auto const text = buildText("memory : full", renderPos1, getAppFont(), 10, sf::Color::Green);
        target.draw(text);
    }

    if (Bee::target == nullptr){
        auto const text = buildText("target : empty", renderPos2, getAppFont(), 10, sf::Color::Red);
        target.draw(text);
    } else {
        auto const text = buildText("target : full", renderPos2, getAppFont(), 10, sf::Color::Green);
        target.draw(text);
    }

    auto const text = buildText("energy to return home : "+ to_nice_string(computeEnergy()), renderPos3, getAppFont(), 10, sf::Color::Black);
    target.draw(text);
}


