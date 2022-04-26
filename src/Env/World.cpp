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

World::~World()
{}

// Getters

float World::getSize() const{
    return nb_cells * cell_size;
}

double World::getHumidity(Vec2d const& pos) const{
    Vec2d coords(coords_from_pos(pos));
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
    double maxHum(*max_element(humidity_.begin(),humidity_.end()));                     // Itérateur retournant la valeur maximale du tableau humidity_
    double minHum(*min_element(humidity_.begin(),humidity_.end()));                     // Itérateur retournant la valeur minimale du tableau humidity_

    for (int x(0); x < nb_cells; ++x) {
        for (int y(0); y < nb_cells; ++y) {
            size_t id(getId(x,y));
            std::vector<size_t> indexes(indexesForCellVertexes(x,y, nb_cells));
            double niveau_bleu((humidity_[id] - minHum) /(maxHum - minHum) * 255);      // Calcule le niveau de bleu relatif à afficher
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

    size_t w_seed(getAppConfig().world_nb_water_seeds);
    for (size_t i(0); i < seeds_.size(); ++i){
        seeds_[i].coords = sf::Vector2i(uniform(0, nb_cells-1), uniform(0, nb_cells-1));        // Associe une position aléatoire à la graine
        size_t id(getId(seeds_[i].coords.x, seeds_[i].coords.y));
        if (i < w_seed){                                                                        // Génère d'abord des graines d'eau, puis une fois la limite atteinte, passe aux graines d'herbe
            seeds_[i].type = Kind::eau;
            humidityImpact(id);                                                                 // Calcule l'impact de la nouvelle cellule d'eau sur les autres
        } else {
            seeds_[i].type = Kind::herbe;
        }
        if (cells_[id] != Kind::eau) {                                                          // Modifie la cellule uniquement si ce n'est pas de l'eau
            cells_[id] = seeds_[i].type;
        }
    }

    if (regenerate){                                                                            // Si la régénération est activée, appelle steps() et smooths() selon la configuration
        steps(getAppConfig().world_generation_steps);
        smooths(getAppConfig().world_generation_smoothness_level);
    }
    updateCache();
}

void World::drawOn(sf::RenderTarget& target) const{
    if (!getAppConfig().showHumidity()) {                                                       // Affichage normal du monde
        sf::Sprite cache(renderingCache_.getTexture());
        target.draw(cache);
    } else {                                                                                    // Affichage des niveaux d'humidité
        target.draw(humidityVertexes_.data(), humidityVertexes_.size(), sf::Quads);
        if (isDebugOn()){                                                                       // Affichage du debug des niveaux d'humidité
            sf::Vector2i curseur(getApp().getCursorPositionInView());
            sf::Vector2i position(curseur.x/cell_size,curseur.y/cell_size);
            sf::Vector2i affichage(curseur.x,curseur.y-30);
            auto const text = buildText(to_nice_string(humidity_[getId(position.x,position.y)]), affichage, getAppFont(), 30, sf::Color::Red);
            target.draw(text);
        }
    }
}


// Génération aléatoire

void World::step(){
    for (auto& seed : seeds_){
        if (seed.type == Kind::herbe){
            seed.coords += sf::Vector2i(randomDir());                                           // Evolution dans une direction aléatoire
            clamp(seed.coords);
        } else {
            if (bernoulli(getAppConfig().water_seeds_teleport_proba) == 0){
                seed.coords += sf::Vector2i(randomDir());                                       // Evolution dans une direction aléatoire
                clamp(seed.coords);
            } else {
                seed.coords = sf::Vector2i(uniform(0, nb_cells-1), uniform(0, nb_cells-1));     // Téléportation de la graine d'eau à une position aléatoire
            }
        }
        size_t id(getId(seed.coords.x, seed.coords.y));
        if (cells_[id] != Kind::eau) {
            cells_[id] = seed.type;                         // Transmission du type de la graine à la cellule
            if (seed.type == Kind::eau) {
                humidityImpact(id);                         // Calcul de l'impact d'humidité si graine d'eau
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
        int x(getX(id));
        int y(getY(id));
        double neighbour_counter(0);
        if (copie_de_cells_[id] != Kind::eau){
            double water_counter(0);
            double grass_counter(0);
            for (int a(-1); a <= 1 ; ++a){                                  // La double boucle parcourt toutes les cellules autour de celle qu'on veut
                for (int b(-1); b <= 1 ; ++b){
                    sf::Vector2i index(x+a,y+b);
                    sf::Vector2i copie(index);
                    clamp(index);
                    if (index == copie){                                    // Si les nouvelles coordonées sont toujours les mêmes après un clamping, alors la cellule voisine existe (++neighbour_counter)
                        size_t new_id(getId(index.x, index.y));
                        ++neighbour_counter;
                        if (copie_de_cells_[new_id] == Kind::eau){
                            ++water_counter;
                        } else if ((copie_de_cells_[new_id] == Kind::herbe) and (copie_de_cells_[id] == Kind::roche)){
                            ++grass_counter;
                        }
                    }
                }
            }
            --neighbour_counter;                                            // La cellule elle même n'est pas une voisine, il faut retirer ce cas
            if (water_counter/neighbour_counter > seuil_w){
                copie_de_cells_[id] = Kind::eau;
                humidityImpact(id);
            } else if (grass_counter/neighbour_counter > seuil_g){
                copie_de_cells_[id] = Kind::herbe;
            }
        }
    }
    std::swap(cells_, copie_de_cells_);                                     // Quand le lissage est fini, on copie copie_de_cells_ dans cell_ (le swap est une optimisation).
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
    if (entree.fail()){                                                 // Vérification de l'état du flot et lancement d'une erreur le cas échéant
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
        cell = static_cast<Kind>(lu);                                   // Transformation de short à Kind
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
    if (sortie.fail()){                                                 // Vérification de l'état du flot et lancement d'une erreur le cas échéant
        sortie.close();
        std::string error("impossible d'écrire le fichier sous ");
        throw std::runtime_error(error+path);
        return;
    } else {
        std::cout << "Ecriture dans : " << path << std::endl;
        sortie << nb_cells << std::endl << cell_size << std::endl;
        for (auto cell : cells_){
            short lu(static_cast<short>(cell));                         // Transformation de Kind à short
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

    sf::Vector2i range_x(x - humidityRange, x + humidityRange + 1);
    sf::Vector2i range_y(y - humidityRange, y + humidityRange + 1);
    clamp(range_x);
    clamp(range_y);

    for (int i(range_x.x) ; i <= (range_x.y) ; ++i) {
        for (int j(range_y.x) ; j <= (range_y.y) ; ++j) {
            int id2(getId(i,j));
            double dist(std::hypot(x-i,y-j));
            humidity_[id2] += ( cte1 * exp(-dist / cte2));
        }
    }
}

// Tests

bool World::isGrowable(Vec2d const& pos) const{
    Vec2d coords(coords_from_pos(pos));
    bool herbe(cells_[getId(coords.x(), coords.y())] == Kind::herbe);
    bool hive(getAppEnv().getCollidingHive(Collider(pos, getAppConfig().flower_manual_size)) == nullptr);
    return (herbe and hive);
}

bool World::isHiveable(Vec2d const& pos, double rad) const{
    double factor(getAppConfig().hiveable_factor);
    double cote(rad*factor);
    Vec2d topLeft(pos - Vec2d(cote/2, cote/2));
    Vec2d bottomRight(pos + Vec2d(cote/2, cote/2));
    toricClamp(topLeft);
    toricClamp(bottomRight);


    std::vector<std::size_t> field(indexesForRect(topLeft, bottomRight));

    for (auto id : field){
        if (cells_[id] != Kind::herbe){
            return false;
        }
    }
    return true;
}


// Fonctions d'implémentation

sf::Vector2i World::randomDir() const{
    switch (uniform(0,3)) {
    case 0 : return sf::Vector2i(-1,0);
    case 1 : return sf::Vector2i(1,0);
    case 2 : return sf::Vector2i(0,-1);
    case 3 : return sf::Vector2i(0,1);
    }
    return sf::Vector2i(0,0);
}

void World::clamp(sf::Vector2i& vect) const{
    if (vect.x > nb_cells-1){
        vect.x = nb_cells-1;
    }
    if (vect.y > nb_cells-1){
        vect.y = nb_cells-1;
    }
    if (vect.x < 0){
        vect.x = 0;
    }
    if (vect.y < 0){
        vect.y = 0;
    }
}

Vec2d World::coords_from_pos(Vec2d const& pos) const{
    return Vec2d(pos.x()/cell_size, pos.y()/cell_size);;
}

std::vector<std::size_t> World::indexesForRect(Vec2d const& top, Vec2d const& bot) const{
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
    // Remembering the axes:
    //
    //   +---> x
    //   |
    //   |
    //   Ë…
    //   y
    //

    // Case 1 :

    std::vector<std::size_t> ids;
    Vec2d topCoords(coords_from_pos(top));
    Vec2d botCoords(coords_from_pos(bot));

    if ((topCoords.x() < botCoords.x()) and (topCoords.y() < botCoords.y())){
        for (size_t i(topCoords.x()); i <= botCoords.x() ; ++i){
            for (size_t j(topCoords.y()); j <= botCoords.y() ; ++j){
                ids.push_back(getId(i, j));
            }
        }
        return ids;
    }

    // Case 2 :

    if ((topCoords.x() > botCoords.x()) and (topCoords.y() > botCoords.y())){
        for (size_t i(0); i <= botCoords.x(); ++i){
            for (size_t j(0); j <= botCoords.y(); ++j){
                ids.push_back(getId(i, j));
            }
        }

        for (size_t i(0); i <= botCoords.x(); ++i){
            for (int j(topCoords.y()); j <= (nb_cells-1); ++j){
                ids.push_back(getId(i, j));
            }
        }

        for (int i(topCoords.x()); i <= (nb_cells-1); ++i){
            for (size_t j(0); j <= botCoords.y(); ++j){
                ids.push_back(getId(i, j));
            }
        }

        for (int i(topCoords.x()); i <= (nb_cells-1) ; ++i){
            for (int j(topCoords.y()); j <= (nb_cells-1) ; ++j){
                ids.push_back(getId(i, j));
            }
        }
        return ids;
    }

    // Case 3 :

    if ((topCoords.x() > botCoords.x()) and (topCoords.y() < botCoords.y())){
        for (size_t i(0); i <= botCoords.x(); ++i){
            for (size_t j(topCoords.y()); j <= botCoords.y(); ++j){
                ids.push_back(getId(i, j));
            }
        }

        for (int i(topCoords.x()); i <= (nb_cells-1); ++i){
            for (size_t j(topCoords.y()); j <= botCoords.y(); ++j){
                ids.push_back(getId(i, j));
            }
        }
        return ids;
    }

    // Case 4 :

    if ((topCoords.x() < botCoords.x()) and (topCoords.y() > botCoords.y())){
        for (size_t i(topCoords.x()); i <= botCoords.x(); ++i){
            for (size_t j(0); j <= botCoords.y(); ++j){
                ids.push_back(getId(i, j));
            }
        }

        for (size_t i(topCoords.x()); i <= botCoords.x(); ++i){
            for (int j(topCoords.y()); j <= (nb_cells-1); ++j){
                ids.push_back(getId(i, j));
            }
        }
        return ids;
    }

    return ids;
}


void World::toricClamp(Vec2d& vect) const{
    auto worldSize = getApp().getEnvSize();
    auto width  = worldSize.x();                        // Récupération des dimensions prédéfinies
    auto height = worldSize.y();

    double reste_x(fmod(vect.x(), width));            // Reste de la division afin de contrôler si toujours dans le monde torique
    double reste_y(fmod(vect.y(), height));

    if (reste_x < 0){
        reste_x += width;                               // Recalage dans les limites si besoin
    }
    if (reste_y < 0){
        reste_y += height;
    }


    vect = Vec2d(reste_x, reste_y);
}

