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

bool Env::isWorldFlyable(Vec2d const& pos) const{
    return terrain.isFlyable(pos);
}

// Méthodes pures

void Env::update(sf::Time dt){
    generator.update(dt);

    size_t taille(flowers.size());
    for (size_t i(0); i < taille; ++i){
        flowers[i]->update(dt);
        if (!(flowers[i]->hasPollen())){                    // Suppression des fleurs sans pollen
            delete flowers[i];
            flowers[i] = nullptr;
        }
    }
    flowers.erase(std::remove(flowers.begin(), flowers.end(), nullptr), flowers.end());

    for (auto& hive : hives){
        hive->update(dt);
        if (hive->isDead()){
            delete hive;
            hive = nullptr;
        }
    }
    hives.erase(std::remove(hives.begin(), hives.end(), nullptr), hives.end());
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

    // N'affiche pas la zone si elle est en dehors de la zone de simulation
    if ((pos.x() < 0) or (pos.x() > terrain.getSize()) or (pos.y() < 0) or (pos.y() > terrain.getSize())){
        return;
    }
    if (terrain.isGrowable(pos) and (flowers.size() < getAppConfig().flower_max_number)){
        auto shape = buildAnnulus(pos, size, sf::Color::Green, 3.0);
        target.draw(shape);
    } else {
        auto shape = buildAnnulus(pos, size, sf::Color::Red, 3.0);
        target.draw(shape);
    }
}

Flower* Env::getCollidingFlower(Collider const& body) const{
    for (auto& flower : flowers){
        if (*flower|body){
            return flower;
        }
    }
    return nullptr;
}

Vec2d const* Env::getCollidingFlowerPosition(Collider const& body) const{
    Flower* found(getCollidingFlower(body));

    if (found != nullptr){
        return new Vec2d(found->getPosition());
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

    // N'affiche pas la zone si elle est en dehors de la zone de simulation
    if ((pos.x() < 0-cote/2) or (pos.x() > terrain.getSize() + cote/2) or (pos.y() < 0-cote/2) or (pos.y() > terrain.getSize() + cote/2)){
        return;
    }

    Vec2d topLeft(pos - Vec2d(cote/2, cote/2));
    Vec2d bottomRight(pos + Vec2d(cote/2, cote/2));

    Collider hive(pos, size);
    bool libre((getCollidingHive(hive) == nullptr) and (getCollidingFlower(hive) == nullptr));
    bool herbe(terrain.isHiveable(pos, size));

    if (!libre){
        toricHivable(target, topLeft, bottomRight, sf::Color::Blue);
    } else if (libre and !herbe){
        toricHivable(target, topLeft, bottomRight, sf::Color::Red);
    } else {
        toricHivable(target, topLeft, bottomRight, sf::Color::Green);
    }
}


Hive* Env::getCollidingHive(Collider const& body) const{
    double size(getAppConfig().hive_manual_size);
    double factor(getAppConfig().hiveable_factor);

    for (auto& hive : hives){
        Collider factoredHive(hive->getPosition(), size*factor);
        if (factoredHive|body){
            return hive;
        }
    }
    return nullptr;
}


// Stats

Data Env::fetchData(std::string string) const{
    Data new_data;
    if (string == s::GENERAL){
        new_data[s::FLOWERS] = flowers.size();
        new_data[s::HIVES] = hives.size();
        new_data[s::SCOUTS] = double(getHivesScoutNumber());
        new_data[s::WORKERS] = double(getHivesWorkerNumber());
    } else if (string == s::HIVES){
        for (size_t i(0); i < hives.size(); ++i){
            new_data["hive #"+to_nice_string(i)] = hives[i]->getPollen();
        }
    }
    return new_data;
}

std::vector<std::string> Env::getHivesIds() const{
    std::vector<std::string> ids;
    for (size_t i(0); i < hives.size(); ++i){
        ids.push_back("hive #"+to_nice_string(i));
    }
    return ids;
}



// Fonctions d'implémentation

void Env::destroyAll(){
    for (auto& flower : flowers){
        delete flower;
        flower = nullptr;
    }
    flowers.clear();

    for (auto& hive : hives){
        delete hive;
        hive = nullptr;
    }
    hives.clear();
}



void Env::toricHivable(sf::RenderTarget& target, Vec2d const& top, Vec2d const& bot, sf::Color color) const{
    Vec2d topClamp(terrain.toricClamp(top));
    Vec2d botClamp(terrain.toricClamp(bot));

    float size(terrain.getSize());

    //       case 1                case 3
    //    +---------+           +---------+
    //    |         |           |         |
    //    | *--+    |        *--|-+    *--|
    //    | |xx|    |        |oo|x|    |xx|
    //    | +--%    |        +--|-%    +--|
    //    |         |           |         |
    //    +---------+           +---------+
    //
    // *----+
    // |oo o|case 2                case 4
    // |  +---------+           +---------+
    // |oo|x|    |xx|           |   |xx|  |
    // +--|-%    +--|           |   +--%  |
    //    |         |           |         |
    //    |-+    *--|           |   *--+  |
    //    |x|    |xx|           |   |xx|  |
    //    +---------+           +---------+
    //                              |oo|
    //                              +--%

    // Case 1 :

    if ((topClamp.x() < botClamp.x()) and (topClamp.y() < botClamp.y())){
        sf::RectangleShape defaultShape(buildRectangle(top, bot, color, 5.0));
        target.draw(defaultShape);
    }

    // Case 2 :

    if ((topClamp.x() > botClamp.x()) and (topClamp.y() > botClamp.y())){
        sf::RectangleShape shape(buildRectangle({0, 0}, botClamp, color, 5.0));
        sf::RectangleShape shape2(buildRectangle({0, topClamp.y()}, {botClamp.x(), size}, color, 5.0));
        sf::RectangleShape shape3(buildRectangle({topClamp.x(), 0}, {size, botClamp.y()}, color, 5.0));
        sf::RectangleShape shape4(buildRectangle(topClamp, {size, size}, color, 5.0));
        target.draw(shape);
        target.draw(shape2);
        target.draw(shape3);
        target.draw(shape4);
    }

    // Case 3 :

    if ((topClamp.x() > botClamp.x()) and (topClamp.y() < botClamp.y())){
        sf::RectangleShape shape(buildRectangle({0, topClamp.y()}, botClamp, color, 5.0));
        sf::RectangleShape shape2(buildRectangle(topClamp, {size, botClamp.y()}, color, 5.0));
        target.draw(shape);
        target.draw(shape2);
    }

    // Case 4 :

    if ((topClamp.x() < botClamp.x()) and (topClamp.y() > botClamp.y())){
        sf::RectangleShape shape(buildRectangle({topClamp.x(), 0}, botClamp, color, 5.0));
        sf::RectangleShape shape2(buildRectangle(topClamp, {botClamp.x(), size}, color, 5.0));
        target.draw(shape);
        target.draw(shape2);
    }
}



size_t Env::getHivesScoutNumber() const {
    size_t number(0);
    for (auto hive : hives){
        number += hive->getScoutNumber();
    }
    return number;
}

size_t Env::getHivesWorkerNumber() const {
    size_t number(0);
    for (auto hive : hives){
        number += hive->getWorkerNumber();
    }
    return number;
}



