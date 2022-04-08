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
}

void Env::drawOn(sf::RenderTarget& target) const{
    terrain.drawOn(target);
    if (!getAppConfig().showHumidity()){
        double size(getAppConfig().world_size/getAppConfig().world_cells);
        for (auto fleur : flowers){
            fleur->drawOn(target, size);
        }
    }
}

void Env::reset(){
    destroyAll();
    terrain.reset();
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
        Vec2d cen(terrain.coords_from_pos(p));
        double pollen(uniform(getAppConfig().flower_nectar_min, getAppConfig().flower_nectar_max));
        flowers.push_back(new Flower(cen, rad, pollen));
        return true;
    }
    return false;
}


void Env::drawFlowerZone(sf::RenderTarget& target, Vec2d const& pos){
    double size(getAppConfig().flower_manual_size);
    if (terrain.isGrowable(pos)){
        auto shape = buildAnnulus(pos, size, sf::Color::Green, 3.0);
        target.draw(shape);
    } else {
    auto shape = buildAnnulus(pos, size, sf::Color::Red, 3.0);
    target.draw(shape);
    }
}
