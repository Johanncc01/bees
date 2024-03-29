#include <Env/World.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vertex.hpp>
#include <Random/Random.hpp>

#include <iostream>
#include <fstream>


// Constructeur

World::World()
{ reset(); };

// Getters

float World::getSize() const{
    return nb_cells * cell_size;
}

double World::getHumidity(Vec2d const& pos) const{
    Vec2d coords(coordsFromPos(pos));
    return humidity_[getId(coords.x(), coords.y())];
}

int World::getId(int x, int y) const{
    return x + y*nb_cells;
}

int World::getX(int id) const{
    return id % nb_cells;
}

int World::getY(int id) const{
    return id / nb_cells;
}


// Fonctions graphiques

void World::reloadConfig(){
    nb_cells = getAppConfig().world_cells;
    cell_size = (getAppConfig().world_size / nb_cells);

    cells_ = Cells((nb_cells*nb_cells), Kind::roche);
    seeds_ = Seeds(getAppConfig().world_nb_water_seeds + getAppConfig().world_nb_grass_seeds);
    humidity_ = Humidities((nb_cells*nb_cells), 0);

    double cte1(getAppConfig().world_humidity_init_level);
    double cte2(getAppConfig().world_humidity_decay_rate);
    double threshold(getAppConfig().world_humidity_threshold);
    humidityRange = 0;

    while ((cte1 * exp(- humidityRange / cte2)) > threshold){
        humidityRange += 2;
    }
}

void World::reloadCacheStructure(){
    Vertexes vertexes(generateVertexes(getAppConfig().world_textures, nb_cells, cell_size));
    grassVertexes_ = vertexes;
    waterVertexes_ = vertexes;
    rockVertexes_ = vertexes;
    humidityVertexes_ = vertexes;
    renderingCache_.create(nb_cells*cell_size, nb_cells*cell_size);
}

void World::updateCache(){
    renderingCache_.clear();

    // Itérateur (max_element) pointant vers la valeur maximale du tableau humidity_
    double maxHum(*max_element(humidity_.begin(),humidity_.end()));
    // Itérateur (min_element) pointant vers la valeur minimale du tableau humidity_
    double minHum(*min_element(humidity_.begin(),humidity_.end()));

    for (int x(0); x < nb_cells; ++x) {
        for (int y(0); y < nb_cells; ++y) {
            updateVertexes(x, y, maxHum, minHum);
        }
    }
    sf::RenderStates rs_g;
    rs_g.texture = &getAppTexture(getAppConfig().grass_texture);                            // texture liée à l'herbe
    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rs_g);

    sf::RenderStates rs_w;
    rs_w.texture = &getAppTexture(getAppConfig().water_texture);                            // texture liée à l'eau
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rs_w);

    sf::RenderStates rs_r;
    rs_r.texture = &getAppTexture(getAppConfig().rock_texture);                             // texture liée à la roche
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs_r);

    renderingCache_.display();
}

void World::reset(bool regenerate){
    reloadConfig();
    reloadCacheStructure();
    seedsReset();

    // Si la régénération est activée, appelle steps() et smooths() selon la configuration
    if (regenerate){
        steps(getAppConfig().world_generation_steps);
        smooths(getAppConfig().world_generation_smoothness_level);
    }
    updateCache();
}

void World::drawOn(sf::RenderTarget& target) const{
    if (!getAppConfig().showHumidity()) {
        sf::Sprite cache(renderingCache_.getTexture());
        target.draw(cache);
    } else {
        // Affichage des niveaux d'humidité
        target.draw(humidityVertexes_.data(), humidityVertexes_.size(), sf::Quads);
        if (isDebugOn()){
            // Affichage du debug des niveaux d'humidité
            Vec2d curseur(getApp().getCursorPositionInView());
            Vec2d position(curseur.x()/cell_size,curseur.y()/cell_size);
            Vec2d affichage(curseur.x(),curseur.y()-30);
            auto const text = buildText(to_nice_string(humidity_[getId(position.x(),position.y())]), affichage, getAppFont(), 30, sf::Color::Red);
            target.draw(text);
        }
    }
}


// Génération aléatoire

void World::step(){
    for (auto& seed : seeds_){
        if (bernoulli(getAppConfig().water_seeds_teleport_proba) and seed.type == Kind::eau){
            // Téléportation de la graine d'eau à une position aléatoire
            seed.coords = Vec2d(uniform(0, nb_cells-1), uniform(0, nb_cells-1));
        } else {
            // Evolution de la graine dans une direction aléatoire
            seed.coords += randomDir();
            seed.coords = regularClamp(seed.coords);
        }
        size_t id(getId(seed.coords.x(), seed.coords.y()));
        if (cells_[id] != Kind::eau) {
            // Transmission du type de la graine à la cellule
            cells_[id] = seed.type;
            if (seed.type == Kind::eau) {
                // Calcul de l'impact d'humidité si graine d'eau
                humidityImpact(id);
            }
        }
    }
}


void World::steps(int nb, bool regenerate){
    for (int i(0); i < nb ; ++i){
        step();
    }
    if (regenerate){
        updateCache();
    }
}

void World::smooth(){
    auto copie_de_cells_ = cells_;
    double seuil_w(getAppConfig().smoothness_water_neighbor_ratio);
    double seuil_g(getAppConfig().smoothness_grass_neighbor_ratio);
    for (size_t id(0); id < copie_de_cells_.size(); ++id){
        double neighbour_counter(0);
        double water_counter(0);
        double grass_counter(0);
        if (copie_de_cells_[id] != Kind::eau){
            // Calcule les données pour la cellule id
            cellCounter(id, neighbour_counter, water_counter, grass_counter);
            if (water_counter/neighbour_counter > seuil_w){
                copie_de_cells_[id] = Kind::eau;
                humidityImpact(id);
            } else if (grass_counter/neighbour_counter > seuil_g){
                copie_de_cells_[id] = Kind::herbe;
            }
        }
    }
    // Quand le lissage est fini, on copie copie_de_cells_ dans cell_ (le swap est une optimisation).
    std::swap(cells_, copie_de_cells_);
}


void World::smooths(int nb, bool regenerate){
    for (int i(0); i < nb ; ++i){
        smooth();
    }
    if (regenerate){
        updateCache();
    }
}


// Sauvegarde

void World::loadFromFile(){
    std::string path(getApp().getResPath()+getAppConfig().world_init_file);
    std::ifstream entree(path);
    // Vérification de l'état du flot et lancement d'une erreur le cas échéant
    if (entree.fail()){
        entree.close();
        std::string error("impossible de lire le fichier ");
        throw std::runtime_error(error+path);
        return;
    }
    std::cout << "Fichier chargé : " << path << std::endl;
    entree >> nb_cells >> std::ws >> cell_size >> std::ws;

    cells_ = Cells(nb_cells*nb_cells);
    short lu(0);
    for (auto& cell : cells_){
        entree >> lu;
        // Transformation de short à Kind
        cell = static_cast<Kind>(lu);
    }

    humidity_ = Humidities(nb_cells*nb_cells);
    entree >> std::ws;
    double lulu(0);
    for (auto& humidity : humidity_){
        entree >> lulu;
        humidity = lulu;
    }

    entree.close();
    reloadCacheStructure();
    updateCache();
}

void World::saveToFile(){
    std::string path(getApp().getResPath()+getAppConfig().world_init_file);
    std::ofstream sortie(path);
    // Vérification de l'état du flot et lancement d'une erreur le cas échéant
    if (sortie.fail()){
        sortie.close();
        std::string error("impossible d'écrire le fichier sous ");
        throw std::runtime_error(error+path);
        return;
    } else {
        std::cout << "Ecriture dans : " << path << std::endl;
        sortie << nb_cells << std::endl << cell_size << std::endl;
        for (auto cell : cells_){
            // Transformation de Kind à short
            short lu(static_cast<short>(cell));
            sortie << lu << " ";
        }
        sortie << std::endl;
        for (auto humidity : humidity_){
            sortie << humidity << " ";
        }
        std::cout << "Opération terminée!" << std::endl;
        sortie.close();
    }
}


// Humidity

void World::humidityImpact(size_t id){
    int x(getX(id));
    int y(getY(id));

    double cte1(getAppConfig().world_humidity_init_level);
    double cte2(getAppConfig().world_humidity_decay_rate);

    Vec2d range_x(regularClamp(Vec2d(x - humidityRange, x + humidityRange + 1)));
    Vec2d range_y(regularClamp(Vec2d(y - humidityRange, y + humidityRange + 1)));

    for (int i(range_x.x()) ; i <= (range_x.y()) ; ++i) {
        for (int j(range_y.x()) ; j <= (range_y.y()) ; ++j) {
            int id2(getId(i,j));
            double dist(std::hypot(x-i,y-j));
            humidity_[id2] += ( cte1 * exp(-dist / cte2));
        }
    }
}

// Tests

bool World::isGrowable(Vec2d const& pos) const{
    Vec2d coords(coordsFromPos(pos));
    bool herbe(cells_[getId(coords.x(), coords.y())] == Kind::herbe);
    bool hive(getAppEnv().getCollidingHive(Collider(pos, getAppConfig().flower_manual_size)) == nullptr);
    return (herbe and hive);
}

bool World::isHiveable(Vec2d const& pos, double rad) const{
    double factor(getAppConfig().hiveable_factor);
    double cote(rad*factor);
    Vec2d topLeft(toricClamp(pos - Vec2d(cote/2, cote/2)));
    Vec2d bottomRight(toricClamp(pos + Vec2d(cote/2, cote/2)));

    Ids field(indexesForRect(topLeft, bottomRight));

    for (auto id : field){
        if (cells_[id] != Kind::herbe){
            return false;
        }
    }
    return true;
}

bool World::isFlyable(Vec2d const& pos) const{
    Vec2d coords(coordsFromPos(toricClamp(pos)));
    bool roche(cells_[getId(coords.x(), coords.y())] == Kind::roche);
    return !roche;
}

// Fonctions d'implémentation

void World::updateVertexes(int x, int y, double maxHum, double minHum){
    size_t id(getId(x,y));
    Ids indexes(indexesForCellVertexes(x,y, nb_cells));
    // Calcule le niveau de bleu relatif à afficher
    double niveau_bleu((humidity_[id] - minHum) /(maxHum - minHum) * 255);
    if (cells_[id] == Kind::herbe){
        for (auto var : indexes){
            grassVertexes_[var].color.a = 255;
            waterVertexes_[var].color.a = 0;
            rockVertexes_[var].color.a = 0;
            humidityVertexes_[var].color = sf::Color(0 , 0, niveau_bleu);
        }
    } else if (cells_[id] == Kind::eau){
        for (auto var : indexes){
            grassVertexes_[var].color.a = 0;
            waterVertexes_[var].color.a = 255;
            rockVertexes_[var].color.a = 0;
            humidityVertexes_[var].color = sf::Color(0 , 0, niveau_bleu);
        }
    } else {
        for (auto var : indexes){
            grassVertexes_[var].color.a = 0;
            waterVertexes_[var].color.a = 0;
            rockVertexes_[var].color.a = 255;
            humidityVertexes_[var].color = sf::Color(0 , 0, niveau_bleu);
        }
    }
}

void World::seedsReset(){
    size_t w_seed(getAppConfig().world_nb_water_seeds);
    for (size_t i(0); i < seeds_.size(); ++i){
        // Associe une position aléatoire à la graine
        seeds_[i].coords = Vec2d(uniform(0, nb_cells-1), uniform(0, nb_cells-1));
        size_t id(getId(seeds_[i].coords.x(), seeds_[i].coords.y()));
        // Génère d'abord des graines d'eau, puis une fois la limite atteinte, passe aux graines d'herbe
        if (i < w_seed){
            seeds_[i].type = Kind::eau;
            // Calcule l'impact de la nouvelle cellule d'eau sur les autres
            humidityImpact(id);
        } else {
            seeds_[i].type = Kind::herbe;
        }
        if (cells_[id] != Kind::eau) {
            // Modifie la cellule uniquement si ce n'est pas de l'eau
            cells_[id] = seeds_[i].type;
        }
    }
}


void World::cellCounter(size_t id, double& neighbour, double& water, double& grass) const{
    int x(getX(id));
    int y(getY(id));
    // La double boucle parcourt toutes les cellules autour de celle qu'on veut
    for (int a(-1); a <= 1 ; ++a){
        for (int b(-1); b <= 1 ; ++b){
            Vec2d index(x+a,y+b);
            Vec2d clamp(regularClamp(index));
            if (index == clamp){
                // Si les nouvelles coordonées sont toujours les mêmes après un clamping, alors la cellule voisine existe (++neighbour_counter)
                size_t new_id(getId(index.x(), index.y()));
                ++neighbour;
                if (cells_[new_id] == Kind::eau){
                    ++water;
                } else if ((cells_[new_id] == Kind::herbe) and (cells_[id] == Kind::roche)){
                    ++grass;
                }
            }
        }
    }
    // La cellule elle même n'est pas une voisine, il faut retirer ce cas
    --neighbour;
}

Vec2d World::randomDir() const{
    switch (uniform(0,3)) {
    case 0 : return {-1,0};
    case 1 : return {1,0};
    case 2 : return {0,-1};
    case 3 : return {0,1};
    }
    return {0,0};
}

Vec2d World::regularClamp(Vec2d const& vect) const{
    double x(vect.x());
    double y(vect.y());

    if (x > nb_cells-1){
        x = nb_cells-1;
    }
    if (y > nb_cells-1){
        y = nb_cells-1;
    }
    if (x < 0){
        x = 0;
    }
    if (y < 0){
        y = 0;
    }

    return {x,y};
}

Vec2d World::coordsFromPos(Vec2d const& pos) const{
    return Vec2d(pos.x()/cell_size, pos.y()/cell_size);;
}

Ids World::indexesForRect(Vec2d const& top, Vec2d const& bot) const{
    // Handle toric world coordinates for rectangles:
    //
    // case 1) if topLeft and bottomRight are really what they should be,
    //         then the rectangle is not wrapped around the toric world.
    // case 2) if topLeft and bottomRight are swapped,
    //         then bottomRight was actually outside.
    // case 3) if the left and right sides are swapped,
    //         then the rectangle is wrapped on the right side of the world.
    // case 4) if the top and bottom sides are swapped,
    //         then the rectangle is swapped on the bottom side of the world.
    //
    // Graphically, where `*` is topLeft and `%` is bottomRight
    // and `o` and `x` are the area covered by the rectangle:
    //
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

    Ids ids;
    Vec2d topCoords(coordsFromPos(top));
    Vec2d botCoords(coordsFromPos(bot));

    if ((topCoords.x() < botCoords.x()) and (topCoords.y() < botCoords.y())){
        doubleForLoop(topCoords.x(), botCoords.x(), topCoords.y(), botCoords.y(), ids);
        return ids;
    }

    // Case 2 :

    if ((topCoords.x() > botCoords.x()) and (topCoords.y() > botCoords.y())){
        doubleForLoop(0, botCoords.x(), 0, botCoords.y(), ids);
        doubleForLoop(0, botCoords.x(), topCoords.y(), nb_cells-1, ids);
        doubleForLoop(topCoords.x(), nb_cells-1, 0, botCoords.y(), ids);
        doubleForLoop(topCoords.x(), nb_cells-1, topCoords.y(), nb_cells-1, ids);
        return ids;
    }

    // Case 3 :

    if ((topCoords.x() > botCoords.x()) and (topCoords.y() < botCoords.y())){      
        doubleForLoop(0, botCoords.x(), topCoords.y(), botCoords.y(), ids);
        doubleForLoop(topCoords.x(), nb_cells-1, topCoords.y(), botCoords.y(), ids);
        return ids;
    }

    // Case 4 :

    if ((topCoords.x() < botCoords.x()) and (topCoords.y() > botCoords.y())){
        doubleForLoop(topCoords.x(), botCoords.x(), 0, botCoords.y(), ids);
        doubleForLoop(topCoords.x(), botCoords.x(), topCoords.y(), nb_cells-1, ids);
        return ids;
    }

    return ids;
}

void World::doubleForLoop(size_t a, size_t b, size_t c, size_t d, Ids& tab) const{
    for (size_t i(a); i <= b; ++i){
        for (size_t j(c); j <= d; ++j){
            tab.push_back(getId(i, j));
        }
    }
}

Vec2d World::toricClamp(Vec2d const& vect) const{
    double size(nb_cells*cell_size);

    // Reste de la division afin de contrôler si toujours dans le monde torique
    double reste_x(fmod(vect.x(), size));
    double reste_y(fmod(vect.y(), size));

    // Recalage dans les limites si besoin
    if (reste_x < 0){
        reste_x += size;
    }
    if (reste_y < 0){
        reste_y += size;
    }

    return Vec2d(reste_x, reste_y);
}






