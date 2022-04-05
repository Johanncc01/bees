#include "Env.hpp"
#include <Application.hpp>

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


void Env::update(sf::Time dt){
    terrain.updateCache();
}

void Env::drawOn(sf::RenderTarget& target){
    terrain.drawOn(target);
}

void Env::reset(){
    terrain.reset();
}

void Env::loadWorldFromFile(){
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


