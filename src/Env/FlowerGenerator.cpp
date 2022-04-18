#include <Env/FlowerGenerator.hpp>
#include <Application.hpp>
#include <Random/Random.hpp>

FlowerGenerator::FlowerGenerator()
    : compteur(sf::Time::Zero)
{

}

void FlowerGenerator::update(sf::Time dt){
    compteur += dt;
    if ((compteur > sf::seconds(getAppConfig().flower_generator_delay)) and getAppConfig().active_flower_generator){
        compteur = sf::Time::Zero;
        double x(getApp().getEnvSize().x());
        double y(getApp().getEnvSize().y());
        int i(0);
        Vec2d pos;
        do {
        Vec2d rand(uniform(0.0, x), uniform(0.0, y));
        pos = rand;
        ++i;
        } while (!(getAppEnv().addFlowerAt(pos)) and i < 100);
    }

}

void FlowerGenerator::reset(){
    compteur = sf::Time::Zero;
}
