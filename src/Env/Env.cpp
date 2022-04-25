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
    bool libre((getCollidingHive(hive) == nullptr) and (getCollidingFlower(hive) == nullptr));
    if (terrain.isHiveable(p, size) and libre){
        hives.push_back(new Hive(p, size));
        return true;
    }
    return false;
}

void Env::drawHiveableZone(sf::RenderTarget& target, Vec2d const& pos) const{

    double factor(getAppConfig().hiveable_factor);
    double size(getAppConfig().hive_manual_size);
    double cote(size*factor);

    if ((pos.x() < 0 - cote/2) or (pos.x() > terrain.getSize() +  cote/2) or (pos.y() < 0 - cote/2) or (pos.y() > terrain.getSize() + cote/2)){
        return;
    }

    Vec2d topLeft(pos - Vec2d(cote/2, cote/2));
    Vec2d bottomRight(pos + Vec2d(cote/2, cote/2));

    Collider hive(pos, size);
    bool libre((getCollidingHive(hive) == nullptr) and (getCollidingFlower(hive) == nullptr));
    bool herbe(terrain.isHiveable(pos, size));

    if (!libre){
        toricHivable(target, topLeft, bottomRight, sf::Color::Blue, cote);
    } else if (libre and !herbe){
        toricHivable(target, topLeft, bottomRight, sf::Color::Red, cote);
    } else {
        toricHivable(target, topLeft, bottomRight, sf::Color::Green, cote);
    }
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


void Env::toricHivable(sf::RenderTarget& target, Vec2d const& topLeft, Vec2d const& bottomRight, sf::Color color, double cote) const{
    Vec2d topLeftClamp(topLeft);
    Vec2d bottomRightClamp(bottomRight);

    terrain.toricClamp(topLeftClamp);
    terrain.toricClamp(bottomRightClamp);

    if (topLeftClamp != topLeft) {
        if (topLeftClamp.y() < bottomRightClamp.y()) {
            sf::RectangleShape shape(buildRectangle(topLeftClamp,(Vec2d(terrain.getSize(), topLeftClamp.y() + cote )), color, 5.0));
            sf::RectangleShape shape2(buildRectangle(bottomRightClamp,(Vec2d(0, bottomRightClamp.y() - cote) ), color, 5.0));
            target.draw(shape);
            target.draw(shape2);
        }
        else if (topLeftClamp.x() < bottomRightClamp.x()){
            sf::RectangleShape shape(buildRectangle(topLeftClamp, Vec2d(topLeftClamp.x() + cote, terrain.getSize()), color, 5.0));
            sf::RectangleShape shape2(buildRectangle(bottomRightClamp, Vec2d(bottomRightClamp.x() - cote , bottomRightClamp.y() - cote) , color, 5.0));
            target.draw(shape);
            target.draw(shape2);
        }
        else {
            sf::RectangleShape shape(buildRectangle(topLeftClamp, Vec2d(topLeftClamp.x() + cote, terrain.getSize()), color, 5.0));
            sf::RectangleShape shape2(buildRectangle(bottomRightClamp, Vec2d(bottomRightClamp.x() - cote , bottomRightClamp.y() - cote) , color, 5.0));
            sf::RectangleShape shape3(buildRectangle(Vec2d(0, terrain.getSize()), Vec2d(bottomRightClamp.x(), topLeftClamp.y()), color, 5.0));
            sf::RectangleShape shape4(buildRectangle(Vec2d(terrain.getSize(),0), Vec2d(topLeftClamp.x(), bottomRightClamp.y()), color, 5.0));
            target.draw(shape);
            target.draw(shape2);
            target.draw(shape3);
            target.draw(shape4);
        }
    }

    if (bottomRightClamp != bottomRight) {
        if (topLeftClamp.y() < bottomRightClamp.y()) {
            sf::RectangleShape shape(buildRectangle(topLeftClamp,(Vec2d(terrain.getSize(), topLeftClamp.y() + cote )), color, 5.0));
            sf::RectangleShape shape2(buildRectangle(bottomRightClamp,(Vec2d(0, bottomRightClamp.y() - cote) ), color, 5.0));
            target.draw(shape);
            target.draw(shape2);

        }
        else if (topLeftClamp.x() < bottomRightClamp.x()){
            sf::RectangleShape shape(buildRectangle(topLeftClamp, Vec2d(topLeftClamp.x() + cote, terrain.getSize()), color, 5.0));
            sf::RectangleShape shape2(buildRectangle(bottomRightClamp, Vec2d(bottomRightClamp.x() - cote , bottomRightClamp.y() - cote) , color, 5.0));
            target.draw(shape);
            target.draw(shape2);
        }
        else {
            sf::RectangleShape shape(buildRectangle(topLeftClamp, Vec2d(terrain.getSize(), terrain.getSize()), color, 5.0));
            sf::RectangleShape shape2(buildRectangle(bottomRightClamp, Vec2d(0,0) , color, 5.0));
            sf::RectangleShape shape3(buildRectangle(Vec2d(0,terrain.getSize()), Vec2d(bottomRightClamp.x(), topLeftClamp.y()), color, 5.0));
            sf::RectangleShape shape4(buildRectangle(Vec2d(terrain.getSize(),0), Vec2d(topLeftClamp.x(), bottomRightClamp.y()), color, 5.0));
            target.draw(shape);
            target.draw(shape2);
            target.draw(shape3);
            target.draw(shape4);
        }
    }

    sf::RectangleShape shape(buildRectangle(topLeft, bottomRight, color, 5.0));
    target.draw(shape);
}






















