#include "Env.hpp"
#include <Application.hpp>
#include "Random/Random.hpp"
#include "Utility/Utility.hpp"

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

void Env::destroyAll(){
    for (auto flower : flowers){
        delete flower;
    }
    flowers.clear();

    for (auto hive : hives){
        delete hive;
    }
    hives.clear();
}

void Env::update(sf::Time dt){
    generator.update(dt);
    for (size_t i(0); i < flowers.size(); ++i){
        flowers[i]->update(dt);
        if (!(flowers[i]->hasPollen())){
            delete flowers[i];
            flowers[i] = nullptr;
        }
    }
    flowers.erase(std::remove(flowers.begin(), flowers.end(), nullptr), flowers.end());
}

void Env::drawOn(sf::RenderTarget& target) const{
    terrain.drawOn(target);
    if (!getAppConfig().showHumidity()){
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

void Env::loadWorldFromFile(){
    destroyAll();
    terrain.loadFromFile();
}

void Env::saveWorldToFile(){
    terrain.saveToFile();
}


float Env::getSize() const{
    return terrain.getSize();
}

void Env::resetControls()
{

}


bool Env::addFlowerAt(Vec2d const& p){
    if ((flowers.size() < getAppConfig().flower_max_number) and (terrain.isGrowable(p))){
        double rad(getAppConfig().flower_manual_size);
        double pollen(uniform(getAppConfig().flower_nectar_min, getAppConfig().flower_nectar_max));
        flowers.push_back(new Flower(p, rad, pollen));
        return true;
    }
    return false;
}


void Env::drawFlowerZone(sf::RenderTarget& target, Vec2d const& pos){
    double size(getAppConfig().flower_manual_size);
    if (terrain.isGrowable(pos) and (flowers.size() < getAppConfig().flower_max_number)){
        auto shape = buildAnnulus(pos, size, sf::Color::Green, 3.0);
        target.draw(shape);
    } else {
    auto shape = buildAnnulus(pos, size, sf::Color::Red, 3.0);
    target.draw(shape);
    }
}


double Env::get_world_humidity(Vec2d const& pos) const{
    return terrain.get_humidity(pos);
}




bool Env::addHiveAt(Vec2d const& p){
    double size(getAppConfig().hive_manual_size);
    Collider hive(p, size);
    if ((getCollidingHive(hive) == nullptr) and (getCollidingFlower(hive) == nullptr)){
        hives.push_back(new Hive(p, size));
        return true;
    }
    return false;
}


Hive* Env::getCollidingHive(const Collider& body){
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


Flower* Env::getCollidingFlower(const Collider& body){
    for (auto flower : flowers){
        if (*flower|body){
            return flower;
        }
    }
    return nullptr;
}






























