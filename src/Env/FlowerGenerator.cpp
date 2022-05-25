#include <Env/FlowerGenerator.hpp>
#include <Application.hpp>
#include <Random/Random.hpp>


// Constructeur

FlowerGenerator::FlowerGenerator()
    : compteur(sf::Time::Zero)
{}


// Gestion du générateur

void FlowerGenerator::update(sf::Time dt){
    compteur += dt;
    if ((compteur > sf::seconds(getAppConfig().flower_generator_delay)) and getAppConfig().active_flower_generator){
        compteur = sf::Time::Zero;
        double x(getApp().getEnvSize().x());
        double y(getApp().getEnvSize().y());
        int i(0);
        do {
            ++i;
        } while (!(getAppEnv().addFlowerAt(Vec2d(uniform(0.0, x), uniform(0.0, y)))) and i < 100);
    }
}

void FlowerGenerator::reset(){
    compteur = sf::Time::Zero;
}
