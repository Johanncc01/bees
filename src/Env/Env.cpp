#include <Env/Env.hpp>
#include <Application.hpp>
#include <Random/Random.hpp>
#include <Utility/Utility.hpp>


// Constructeur et destructeur

Env::Env()
{
    try {
        loadWorldFromFile();
    }
    catch (std::runtime_error error){
        std::cerr << "Error : file not found " << std::endl << "Generating new world... " << std::endl;
        reset();
    }
}

Env::~Env(){
    destroyAll();
}


// Getters

float Env::getSize() const{
    return terrain.getSize();
}

double Env::getWorldHumidity(Vec2d const& pos) const{
    return terrain.getHumidity(pos);
}


// Méthodes pures

void Env::update(sf::Time dt){
    generator.update(dt);
    for (auto& flower : flowers){
        flower->update(dt);
        if (!(flower->hasPollen())){                    // Suppression des fleurs sans pollen
            delete flower;
            flower = nullptr;
        }
    }
    flowers.erase(std::remove(flowers.begin(), flowers.end(), nullptr), flowers.end());
}

void Env::drawOn(sf::RenderTarget& target) const{
    terrain.drawOn(target);
    if (!getAppConfig().showHumidity()){                // Affichage des fleurs et ruches uniquement si pas en mode humidité
        for (auto fleur : flowers){
            fleur->drawOn(target);
        }
        for (auto hive : hives){
            hive->drawOn(target);
        }
    }
}

void Env::reset(){
    destroyAll();
    terrain.reset();
    generator.reset();
}

void Env::resetControls(){

}


// World

void Env::loadWorldFromFile(){
    destroyAll();
    terrain.loadFromFile();
}

void Env::saveWorldToFile(){
    terrain.saveToFile();
}


// Flower

bool Env::addFlowerAt(Vec2d const& p){
    if ((flowers.size() < getAppConfig().flower_max_number) and (terrain.isGrowable(p))){
        double rad(getAppConfig().flower_manual_size);
        double pollen(uniform(getAppConfig().flower_nectar_min, getAppConfig().flower_nectar_max));
        flowers.push_back(new Flower(p, rad, pollen));
        return true;
    }
    return false;
}

void Env::drawFlowerZone(sf::RenderTarget& target, Vec2d const& pos) const{
    double size(getAppConfig().flower_manual_size);
    if (terrain.isGrowable(pos) and (flowers.size() < getAppConfig().flower_max_number)){
        auto shape = buildAnnulus(pos, size, sf::Color::Green, 3.0);
        target.draw(shape);
    } else {
        auto shape = buildAnnulus(pos, size, sf::Color::Red, 3.0);
        target.draw(shape);
    }
}

Flower* Env::getCollidingFlower(Collider const& body) const{
    for (auto flower : flowers){
        if (*flower|body){
            return flower;
        }
    }
    return nullptr;
}


// Hive

bool Env::addHiveAt(Vec2d const& p){
    double size(getAppConfig().hive_manual_size);
    Collider hive(p, size);
    if ((getCollidingHive(hive) == nullptr) and (getCollidingFlower(hive) == nullptr)){
        hives.push_back(new Hive(p, size));
        return true;
    }
    return false;
}


Hive* Env::getCollidingHive(Collider const& body) const{
    double size(getAppConfig().hive_manual_size);
    double factor(getAppConfig().hiveable_factor);

    for (auto hive : hives){
        Collider factoredHive(hive->getPosition(), size*factor);
        if (factoredHive|body){
            return hive;
        }
    }
    return nullptr;
}


// Fonctions d'implémentation

void Env::destroyAll(){
    for (auto& flower : flowers){
        delete flower;
    }
    flowers.clear();

    for (auto& hive : hives){
        delete hive;
    }
    hives.clear();
}
























