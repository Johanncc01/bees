#include <Env/Hive.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Random/Random.hpp>
#include <Env/Bee.hpp>
#include <Env/ScoutBee.hpp>
#include <Env/WorkerBee.hpp>


// Constructeur et destructeur

Hive::Hive(Vec2d const& cen, double rad)
    : Collider(cen, rad), pollen(getAppConfig().hive_initial_nectar)
{}

Hive::~Hive(){
    for (auto bee : bees){
        delete bee;
        bee = nullptr;
    }
    bees.clear();
}


// Méthodes pures

void Hive::drawOn(sf::RenderTarget& target) const{
    auto const& texture = getAppTexture(getAppConfig().hive_texture);
    auto hiveSprite = buildSprite(center, 2.5*radius, texture);
    target.draw(hiveSprite);
    if (isDebugOn()){
        Vec2d affichage(center.x(),center.y()-30);
        auto const text = buildText(to_nice_string(pollen), affichage, getAppFont(), 15, sf::Color::Red);
        target.draw(text);
    }

    for (auto bee : bees){
        bee->drawOn(target);
    }
}


void Hive::update(sf::Time dt){
    for (auto& bee : bees){
        bee->update(dt);
        if (bee->isDead()){
            delete bee;
            bee = nullptr;
        }
    }
    bees.erase(std::remove(bees.begin(), bees.end(), nullptr), bees.end());


    // Interaction systématique : fonctionne

    for (auto& bee1 : bees){
        for (auto& bee2 : bees){
            if (bee1 != bee2){
                bee1->interact(bee2);
            }
        }
    }

    // Test de si dans la ruche : ne fonctionne pas encore :(((
    /*
    Bees inside;

    for (auto& bee : bees){
        if (*this>*bee){
            inside.push_back(bee);
        }
    }

    for (auto& bee1 : inside){
        for (auto& bee2 : inside){
            if (bee1 != bee2){
                bee1->interact(bee2);
            }
        }
    }
    */

    double reproduction(getAppConfig().hive_reproduction_nectar_threshold);
    double maxBees(getAppConfig().hive_reproduction_max_bees);

    if ((pollen >= reproduction) and bees.size() < maxBees){
        addBee(getAppConfig().hive_reproduction_scout_proba);
    }

}



// Bees

Bee* Hive::addBee(double scoutProb){
    Vec2d pos(center + Vec2d::fromRandomAngle()*uniform(0.1, radius-0.1));
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

