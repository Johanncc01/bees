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
    for (auto fleur : flowers){
        delete fleur;
    }
    flowers.clear();
}

void Env::update(sf::Time dt){
    terrain.updateCache();
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
        if (terrain.get_humidity(p) < getAppConfig().flower_growth_threshold) {
        return false;
        }
        flowers.push_back(new Flower(p, rad, pollen));
        return true;
    }
    return false;
}


void Env::drawFlowerZone(sf::RenderTarget& target, Vec2d const& pos){
    double size(getAppConfig().flower_manual_size);
    if (terrain.isGrowable(pos) and (flowers.size() < getAppConfig().flower_max_number) and terrain.get_humidity(pos) >= getAppConfig().flower_growth_threshold){
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
