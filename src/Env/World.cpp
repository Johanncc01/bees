#include <Env/World.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vertex.hpp>
#include <Random/Random.hpp>
#include <iostream>
#include <fstream>

// Constructeurs
    World::World()
    {
        reset();

    };


// Getters

float World::getSize() const{
    return nb_cells * cell_size;
}

int World::get_id(int x, int y) const{
    return x + y*nb_cells;
}

int World::get_x(int id) const{
    return id % nb_cells;
}

int World::get_y(int id) const{
    return id / nb_cells;
}


// Fonctions graphiques

void World::reloadConfig(){
    size_t world_size(getAppConfig().world_size);
    nb_cells = getAppConfig().world_cells;
    cell_size = (world_size / nb_cells);

    Grille grid((nb_cells*nb_cells), Kind::roche);
    cells_ = grid;

    nb_wseed = getAppConfig().world_nb_water_seeds;
    nb_gseed = getAppConfig().world_nb_grass_seeds;
    Seeds seeds_init(nb_wseed + nb_gseed);
    seeds_=seeds_init;

    double humid(0);
    double cte1(getAppConfig().world_humidity_init_level);
    double cte2(getAppConfig().world_humidity_decay_rate);
    double threshold(getAppConfig().world_humidity_threshold);

    while((cte1 * exp(- humid / cte2)) > threshold){
        humid += 2;
    }
    humidityRange = humid;
    Humids new_humidity_lvls(cells_.size(), 0);
    humidity_lvls = new_humidity_lvls;

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
    double maxHum(*max_element(humidity_lvls.begin(),humidity_lvls.end()));
    double minHum(*min_element(humidity_lvls.begin(),humidity_lvls.end()));
    for (int x(0); x < nb_cells; ++x) {
         for (int y(0); y < nb_cells; ++y) {
             size_t id(get_id(x,y));
             std::vector<size_t> indexes(indexesForCellVertexes(x,y, nb_cells));
             double niveau_bleu((humidity_lvls[id] - minHum) /(maxHum - minHum) * 255);
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
    rs_g.texture = &getAppTexture(getAppConfig().grass_texture); // texture liée à l'herbe
    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rs_g);

    sf::RenderStates rs_w;
    rs_w.texture = &getAppTexture(getAppConfig().water_texture); // texture liée à l'eau
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rs_w);

    sf::RenderStates rs_r;
    rs_r.texture = &getAppTexture(getAppConfig().rock_texture); // textugre liée à la roche
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs_r);

    renderingCache_.display();
}

void World::reset(bool regenerate){
    reloadConfig();
    reloadCacheStructure();
    size_t temp_wseed(nb_wseed);
    for (auto& seed : seeds_) {
        int cells(nb_cells);
        size_t x(uniform(0 , cells-1));
        size_t y(uniform(0 , cells-1));
        size_t id(get_id(x,y));
        sf::Vector2i temp(x,y);
        seed.coords = temp;

        if (temp_wseed > 0) {
            --temp_wseed;
            seed.type = Kind::eau;
            humidityImpact(id);
        } else {
            seed.type = Kind::herbe;
        }
        if (cells_[id] != Kind::eau) {
            cells_[id] = seed.type;
        }
    }
    if (regenerate){
        steps(getAppConfig().world_generation_steps);
        smooths(getAppConfig().world_generation_smoothness_level);
    }

    /*
    for (size_t i(0); i < cells_.size(); ++i){
        if (cells_[i] == Kind::eau){
            humidityImpact(i);
        }
    }
    */

    updateCache();
}

void World::drawOn(sf::RenderTarget& target) const{
    if (getAppConfig().showHumidity()) {
        target.draw(humidityVertexes_.data(), humidityVertexes_.size(), sf::Quads);
        if (isDebugOn()){
            sf::Vector2i curseur(getApp().getCursorPositionInView());
            sf::Vector2i position(curseur.x/cell_size,curseur.y/cell_size);
            sf::Vector2i affichage(curseur.x,curseur.y-30);
            auto const text = buildText(to_nice_string(humidity_lvls[get_id(position.x,position.y)]), affichage, getAppFont(), 30, sf::Color::Red);
            target.draw(text);
        }
    } else {
        sf::Sprite cache(renderingCache_.getTexture());
        target.draw(cache);
    }
}


// Génération aléatoire

void World::step(){
    for (auto& seed : seeds_){
        if (seed.type == Kind::herbe){
            sf::Vector2i temp(randomN());
            seed.coords += temp;
            clamp(seed.coords);
        } else {
            if (bernoulli(getAppConfig().water_seeds_teleport_proba) == 0){
                sf::Vector2i temp(randomN());
                seed.coords += temp;
                clamp(seed.coords);

            } else {
                int x(uniform(0 , nb_cells-1));
                int y(uniform(0 , nb_cells-1));
                sf::Vector2i temp(x,y);
                seed.coords = temp;

            }
        }
        size_t i(get_id(seed.coords.x , seed.coords.y));
        if (cells_[i] != Kind::eau) {
            cells_[i] = seed.type;
            if (seed.type == Kind::eau) {
                humidityImpact(i);

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
        int x(get_x(id));
        int y(get_y(id));
        double neighbour_counter(0);
        if (copie_de_cells_[id] != Kind::eau){
            double water_counter(0);
            double grass_counter(0);

           for (int a(-1); a <= 1 ; ++a){
               for (int b(-1); b <= 1 ; ++b){
                   sf::Vector2i index(x+a,y+b);
                   sf::Vector2i copie(index);
                   clamp(index);
                   if (index == copie){
                       ++neighbour_counter;
                       if (copie_de_cells_[get_id(x+a, y+b)] == Kind::eau){
                           ++water_counter;
                       } else if ((copie_de_cells_[get_id(x+a, y+b)] == Kind::herbe) and (copie_de_cells_[id] == Kind::roche)){
                           ++grass_counter;
                       }
                   }
               }
           }
           --neighbour_counter;
           if (water_counter/neighbour_counter > seuil_w){
               copie_de_cells_[id] = Kind::eau;
               humidityImpact(id);
           } else if (grass_counter/neighbour_counter > seuil_g){
               copie_de_cells_[id] = Kind::herbe;
           }
        }
    }
    std::swap(cells_, copie_de_cells_); // quand le lissage est fini on copie copie_de_cells_ dans cell_ (le swap est une optimisation).
}


void World::smooths(int nb, bool regenerate){
    for (int i(0); i < nb ; ++i){
        smooth();
    }
    if (regenerate){
        updateCache();
    }
}

// Humidity

void World::humidityImpact(size_t id){
    int x(get_x(id));
    int y(get_y(id));

    double cte1(getAppConfig().world_humidity_init_level);
    double cte2(getAppConfig().world_humidity_decay_rate);

    sf::Vector2i range_x(x - humidityRange, x + humidityRange + 1);
    sf::Vector2i range_y(y - humidityRange, y + humidityRange + 1);
    clamp(range_x);
    clamp(range_y);

    for (int i(range_x.x) ; i <= (range_x.y) ; i++) {
        for (int j(range_y.x) ; j <= (range_y.y) ; j++) {
            int id2(get_id(i,j));
            double dist(std::hypot(x-i,y-j));
            humidity_lvls[id2] += ( cte1 * exp(-dist / cte2));
        }
    }    
}



// Sauvegarde

void World::loadFromFile(){
    std::string path(getApp().getResPath()+getAppConfig().world_init_file);
    std::ifstream entree(path);
    if (entree.fail()){
        entree.close();
        std::string error("impossible de lire le fichier ");
        throw std::runtime_error(error+path);
        return;
    } else {
        std::cout << "Fichier chargé : " << path << std::endl;
        entree >> nb_cells >> std::ws >> cell_size >> std::ws;
        Grille grid(nb_cells*nb_cells);
        short lu(0);
        for (size_t i(0); i < grid.size(); ++i){
            entree >> lu;
            grid[i] = static_cast<Kind>(lu);
        }        
        cells_ = grid;


        Humids humidity(nb_cells*nb_cells);
        entree >> std::ws;
        double lulu(0);
        for (size_t i(0); i < humidity.size(); ++i){
            entree >> lulu;
            humidity[i] = lulu;
        }
        humidity_lvls = humidity;

        entree.close();
    }
    reloadCacheStructure();
    updateCache();
}

void World::saveToFile(){
    std::string path(getApp().getResPath()+getAppConfig().world_init_file);
    std::ofstream sortie(path);
    if (sortie.fail()){
        sortie.close();
        std::string error("impossible d'écrire le fichier sous ");
        throw std::runtime_error(error+path);
        return;
    } else {
        std::cout << "Ecriture dans : " << path << std::endl;
        sortie << nb_cells << std::endl << cell_size << std::endl;
        for (size_t i(0); i < cells_.size(); ++i){
            short lu(static_cast<short>(cells_[i]));
            sortie << lu << " ";
        }
        sortie << std::endl;
        for (size_t i(0); i < humidity_lvls.size(); ++i){
            sortie << humidity_lvls[i] << " ";
        }
        std::cout << "Opération terminée!" << std::endl;
        sortie.close();
    }
}



// Fonctions d'implémentation

sf::Vector2i World::randomN() const{
    int i(uniform(0,3));
    if (i == 0){
        sf::Vector2i temp(-1,0);
        return temp;
    } else if (i == 1){
        sf::Vector2i temp(1,0);
        return temp;
    } else if (i == 2){
        sf::Vector2i temp(0,-1);
        return temp;
    } else {
        sf::Vector2i temp(0,1);
        return temp;
    }
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


bool World::isGrowable(Vec2d const& pos) const{
    Vec2d coords(coords_from_pos(pos));
    bool herbe(cells_[get_id(coords.x(), coords.y())] == Kind::herbe);
    Collider temp(pos, getAppConfig().flower_manual_size);
    bool hive(getAppEnv().getCollidingHive(temp) == nullptr);
    return (herbe and hive);
}

Vec2d World::coords_from_pos(Vec2d const& pos) const{
    Vec2d coords(pos.x()/cell_size, pos.y()/cell_size);
    return coords;
}

double World::get_humidity(Vec2d const& pos) const{
    Vec2d coords(coords_from_pos(pos));
    return humidity_lvls[get_id(coords.x(), coords.y())];
}

bool World::isHiveable(const Vec2d& pos, double rad) const{
    //Vec2d coords(coords_from_pos(pos));
    return true;
}
