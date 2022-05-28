#include <Env/Hive.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Random/Random.hpp>
// Inclusions à cause de la prédéclaration
#include <Env/Bee.hpp>
#include <Env/ScoutBee.hpp>
#include <Env/WorkerBee.hpp>


// Constructeur et destructeur

Hive::Hive(Vec2d const& cen, double rad)
    : Collider(cen, rad), pollen(getAppConfig().hive_initial_nectar), scoutNumber(0), workerNumber(0)
{}

Hive::~Hive(){
    for (auto& bee : bees){
        delete bee;
        bee = nullptr;
    }
    bees.clear();
}


// Getters

bool Hive::isDead() const{
    return (bees.empty() and pollen == 0);
}

double Hive::getPollen() const{
    return pollen;
}

size_t Hive::getScoutNumber() const{
    return scoutNumber;
}

size_t Hive::getWorkerNumber() const{
    return workerNumber;
}


// Méthodes pures

void Hive::drawOn(sf::RenderTarget& target) const{
    auto const& texture = getAppTexture(getAppConfig().hive_texture);
    auto hiveSprite = buildSprite(getPosition(), 2.5*getRadius(), texture);
    target.draw(hiveSprite);
    if (isDebugOn()){
        Vec2d affichage(getPosition().x(),getPosition().y()-30);
        auto const text = buildText(to_nice_string(pollen), affichage, getAppFont(), 15, sf::Color::Red);
        target.draw(text);
    }

    for (auto bee : bees){
        bee->drawOn(target);
    }
}


void Hive::update(sf::Time dt){   
    updateBees(dt);
    interactBees();
    reproductBees();
}


// Bees

Bee* Hive::addBee(double scoutProb){
    Vec2d pos(getPosition() + Vec2d::fromRandomAngle()*uniform(0.1, getRadius()-0.1));
    if (bernoulli(scoutProb)){
        Bee* newBee(new ScoutBee(*this, pos));
        bees.push_back(newBee);
        return newBee;
    }
    Bee* newBee(new WorkerBee(*this, pos));
    bees.push_back(newBee);
    return newBee;
}



// Pollen

double Hive::dropPollen(double qte){
    pollen += qte;
    return qte;
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



void Hive::changeScoutNumber(bool add){
    if (add){
        ++scoutNumber;
        return;
    }
    --scoutNumber;
}

void Hive::changeWorkerNumber(bool add){
    if (add){
        ++workerNumber;
        return;
    }
    --workerNumber;
}


// Fonctions d'implémentations

void Hive::updateBees(sf::Time dt){
    for (auto& bee : bees){
        bee->update(dt);
        if (bee->isDead()){
            delete bee;
            bee = nullptr;
        }
    }
    bees.erase(std::remove(bees.begin(), bees.end(), nullptr), bees.end());
}

void Hive::interactBees(bool state){

    // Répertorie les abeilles à l'intérieur de la ruche pour les faire interragir entre elles
    Bees inside;

    if (state){
        for (auto& bee : bees){
            if (bee->isStateHive()){
                inside.push_back(bee);
            }
        }
    } else {
        for (auto& bee : bees){
            if (*this|*bee){
                inside.push_back(bee);
            }
        }
    }
    // Fait interragir les abeilles à l'intérieur
    for (auto& bee1 : inside){
        for (auto& bee2 : inside){
            if (bee1 != bee2){
                bee1->interact(bee2);
            }
        }
    }
}

void Hive::reproductBees(){
    double reproduction(getAppConfig().hive_reproduction_nectar_threshold);
    double maxBees(getAppConfig().hive_reproduction_max_bees);
    double ratioScout(scoutNumber/maxBees);

    if ((pollen >= reproduction) and bees.size() < maxBees){
        // Adapte la probabilité d'une scout selon la population actuelle (pour garder un équilibre)
        addBee(1-ratioScout);
    }
}
